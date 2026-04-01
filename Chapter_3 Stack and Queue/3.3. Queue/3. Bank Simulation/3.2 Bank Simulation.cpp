#include <iostream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
using namespace std;

/*==================== 常量定义 ====================*/
const int CloseTime = 480;  // 银行营业时间: 480 min = 8 hours
const int MaxQ = 4;         // 服务窗口数

/*==================== 数据类型定义 ====================*/
// 事件
struct Event
{
    int OccurTime;  // 事件发生时刻
    int NType;      // 0 到达事件; 1~4 -- 第 1~4 窗口离开时间

    // 有参构造
    Event(int t = 0, int n = 0) : OccurTime(t), NType(n) {}
};

// 顾客
struct Customer
{
    int ArrivalTime;  // 到达时刻
    int Duration;     // 办理业务所需时间(逗留时间)

    // 有参构造
    Customer(int a = 0, int d = 0) : ArrivalTime(a), Duration(d) {}
};

/*==================================================
=           模板链队列 LinkQueue<QElemType>         =
==================================================*/
template <class QElemType>
class LinkQueue
{
private:
    struct QNode
    {
        QElemType data;
        QNode *next;

        QNode(const QElemType &d = QElemType(), QNode *n = nullptr) : data(d), next(n) {}
    };

    QNode *front;  // 头指针，指向头结点
    QNode *rear;   // 尾指针, 指向队尾元素
public:
    LinkQueue()
    {
        front = rear = new QNode();  // 内存分配失败自动报错
    }
    ~LinkQueue()
    {
        clear();
        delete front;
    }
    void clear()
    {
        while (!empty())
        {
            deQueue();  // 只要队列不为空，一直进行头删, 直至 front = rear, 队列为空
        }
    }
    bool empty() const 
    {
        return front == rear;
    }
    int length() const
    {
        int len = 0;
        QNode *p = front->next;
        while (p)
        {
            ++len;
            p = p->next;
        }
        return len;
    }
    void enQueue(const QElemType &e)  // 尾插
    {
        QNode *s = new QNode(e);  // 有参构造, data = e, next = nullptr
        rear->next = s;
        rear = s;
    }
    QElemType deQueue()  // 头删
    {
        if (empty())
        {
            throw underflow_error("队列为空，无法进行出队!");
        }
        QNode *p = front->next;
        QElemType e = p->data;
        front->next = p->next;

        if (rear == p)  // 队列只有一个元素, 避免删除 p 后 rear 也变为野指针
        {
            rear = front;
        }

        delete p;
        return e;
    }
    QElemType getHead() const
    {
        if (empty())
        {
            throw underflow_error("队列为空, 无法获取队头元素!");
        }
        return front->next->data;
    }
    // 打印（调试用）
    void print() const
    {
        QNode *p = front->next;
        cout << "[ ";
        while (p)
        {
            cout << "(到达:" << p->data.ArrivalTime
                 << ",办理:" << p->data.Duration << ") ";
            p = p->next;
        }
        cout << "]";
    }
};

/*==================================================
=               事件有序链表 EventList              =
==================================================*/
class EventList
{
private:
    struct LNode
    {
        Event data;
        LNode *next;

        LNode(const Event &e = Event(), LNode *n = nullptr) : data(e), next(n) {}
    };
    LNode *head;  // 头指针 指向头结点
public:
    EventList()
    {
        head = new LNode();
    }
    ~EventList()
    {
        clear();
        delete head;
        head = nullptr;
    }
    void clear()
    {
        while (!empty())
        {
            delFirst();
        }
    }
    bool empty() const
    {
        return head->next == nullptr;
    }
    void orderInsert(const Event &e)
    {
        LNode *p = head;  // p->OccurTime <= e.OccurTime, p->next->OccurTime > e.OccurTime
        while (p->next && p->next->data.OccurTime <= e.OccurTime)
        {
            p = p->next;
        }
        LNode *s = new LNode(e, p->next);
        p->next = s;
    }
    Event delFirst()
    {
        if (empty())
        {
            throw underflow_error("事件表为空, 无法删除事件!");
        }
        LNode *p = head->next;
        Event e = p->data;
        head->next = p->next;
        delete p;
        return e;
    }
    // 打印事件表（调试用）
    void print() const
    {
        LNode *p = head->next;
        cout << "事件表: ";
        while (p)
        {
            cout << "[" << p->data.OccurTime << ", " << p->data.NType << "] ";
            p = p->next;
        }
        cout << endl;
    }
};

/*==================================================
=                    银行模拟类 Bank                =
==================================================*/
class Bank
{
private:
    EventList eventList;            // 事件表
    LinkQueue<Customer> q[MaxQ];    // 4 个窗口队列
    Event en;                       // 当前事件
    int TotalTime;                  // 总逗留时间
    int CustomerNum;                // 客户总人数

private:
    void Random(int &durTime, int &interTime)
    {
        durTime = 1 + rand() % 30;  // 办理事务时间 1~30 min
        interTime = 1 + rand() % 5; // 客户到达时间间隔 1~5 min
    }
    int Minimum()
    {
        int minIndex = 0;
        int min_len = q[0].length();
        for (int i = 1; i < MaxQ; ++i)
        {
            int len = q[i].length();
            if (len < min_len)
            {
                min_len = len;
                minIndex = i;
            }
        }
        return minIndex;
    }
    // 银行开门初始化
    void OpenForDay()
    {
        TotalTime = 0;
        CustomerNum = 0;

        // 插入第一个用户到达事件
        en = Event(0, 0);  // (OccurTime, NType)
        eventList.orderInsert(en);
    }
    // 处理用户到达事件
    void CustomerArrived()
    {
        int durtime, intertime;
        Random(durtime, intertime);

        int t = en.OccurTime + intertime;  // 下一顾客到达时间

        // 若下一位顾客到达时银行未关门，则插入新的到达事件
        // && 后面调试人数为 10 的整个业务流程
        if (t < CloseTime  && CustomerNum < 10)
        {
            Event newEvent(t, 0);  // (OccurTime, NType)
            eventList.orderInsert(newEvent);
        }

        // 当前顾客进入最短队列
        int i = Minimum();
        Customer customer(en.OccurTime, durtime);  // (ArrivalTime, durtime)
        ++CustomerNum;  
        q[i].enQueue(customer);

        // 如果该窗口原来为空，则该顾客立即开始办理，并安排起离开的事件
        if (q[i].length() == 1)
        {
            Event newEvent(en.OccurTime + durtime, i + 1);
            eventList.orderInsert(newEvent);
        }
    }

    // 处理顾客离开事件
    void CustomerDeparture()
    {
        int i = en.NType - 1;  // 窗口号转数组下标
        Customer customer = q[i].deQueue();  // 删除第 i+1 个窗口的队头客户
        TotalTime += en.OccurTime - customer.ArrivalTime;
        // 若该窗口还有顾客，安排下一位客户离开事件
        if (q[i].length())  // 队列长度不为 0 时
        {
            Customer nextCustomer = q[i].getHead();
            Event newEvent(en.OccurTime + nextCustomer.Duration, i + 1);
            eventList.orderInsert(newEvent);
        }
    }
    void printQueues()
    {
        for (int i = 0; i < MaxQ; ++i)
        {
            cout << "窗口 " << i + 1 << ": ";
            q[i].print();
            cout << endl;
        }
    }
public:
    Bank() : TotalTime(0), CustomerNum(0) {}

    // 主模拟流程
    void Simulation()
    {
        OpenForDay();

    while (!eventList.empty())
    {
        cout << "\n====================================" << endl;
        cout << "处理前的事件表：" << endl;
        eventList.print();

        en = eventList.delFirst();

        cout << "当前处理事件: ";
        if (en.NType == 0)
            cout << "顾客到达事件";
        else
            cout << "第 " << en.NType << " 号窗口顾客离开事件";
        cout << " (发生时刻 = " << en.OccurTime << ")" << endl;

        if (en.NType == 0)
        {
            CustomerArrived();
        }
        else
        {
            CustomerDeparture();
        }

        cout << "处理后的事件表：" << endl;
        eventList.print();

        cout << "当前各窗口排队情况：" << endl;
        printQueues();
}

        // 输出结果
        cout << "====================================" << endl;
        cout << "银行排队离散事件模拟结束" << endl;
        cout << "顾客总人数: " << CustomerNum << endl;

        if (CustomerNum != 0)
        {
            cout << "顾客平均逗留时间: "
                 << fixed << setprecision(2)
                 << (double)TotalTime / CustomerNum
                 << " 分钟" << endl;
        }
        else
        {
            cout << "顾客平均逗留时间: 0.00 分钟" << endl; 
        }
        cout << "顾客总逗留时间: " << TotalTime << " 分钟" << endl;
        cout << "====================================" << endl;
    }
};

/*==================== 主函数 ====================*/
int main()
{
    srand((unsigned)time(nullptr));

    Bank bank;
    bank.Simulation();

    system("pause");
    return 0;
}