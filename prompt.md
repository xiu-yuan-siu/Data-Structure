1. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错

2. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错（写一个成员函数打印一个main函数测试）

3. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int Status;

/*==================== 常量定义 ====================*/
#define CloseTime 480     // 银行营业总时间（分钟），如 8 小时 = 480 分钟
#define MaxQ 4            // 4 个服务窗口

/*==================== 数据类型定义 ====================*/
// 顾客信息（进入队列时记录到达时刻和服务时间）
typedef struct
{
    int ArrivalTime;   // 到达时刻
    int Duration;      // 办理业务所需时间
} QElemType;

// 事件类型：0 表示到达事件；1~4 表示第1~4窗口离开事件
typedef struct
{
    int OccurTime;     // 事件发生时刻
    int NType;         // 事件类型
} Event;

// 队列结点
typedef struct QNode
{
    QElemType data;
    struct QNode *next;
} QNode, *QueuePtr;

// 链队列
typedef struct
{
    QueuePtr front;
    QueuePtr rear;
} LinkQueue;

// 事件表结点（有序链表）
typedef struct LNode
{
    Event data;
    struct LNode *next;
} LNode, *LinkList;

/*==================== 全局变量 ====================*/
LinkList EventList;        // 事件表（按发生时间有序）
LinkQueue q[MaxQ];         // 4 个窗口队列
int TotalTime = 0;         // 总逗留时间
int CustomerNum = 0;       // 顾客总数

/*==================== 基本工具函数 ====================*/
void Error(char *msg)
{
    printf("%s\n", msg);
    exit(OVERFLOW);
}

// 生成 [x, y] 范围内随机整数
int Random(int x, int y)
{
    return rand() % (y - x + 1) + x;
}

/*==================== 链队列操作 ====================*/
Status InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
    if (!Q->front) return OVERFLOW;
    Q->front->next = NULL;
    return OK;
}

Status QueueEmpty(LinkQueue Q)
{
    return Q.front == Q.rear ? OK : ERROR;
}

int QueueLength(LinkQueue Q)
{
    int len = 0;
    QueuePtr p = Q.front->next;
    while (p)
    {
        len++;
        p = p->next;
    }
    return len;
}

Status EnQueue(LinkQueue *Q, QElemType e)
{
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if (!p) return OVERFLOW;
    p->data = e;
    p->next = NULL;
    Q->rear->next = p;
    Q->rear = p;
    return OK;
}

Status DeQueue(LinkQueue *Q, QElemType *e)
{
    if (Q->front == Q->rear) return ERROR;
    QueuePtr p = Q->front->next;
    *e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p)
        Q->rear = Q->front;
    free(p);
    return OK;
}

Status GetHead(LinkQueue Q, QElemType *e)
{
    if (Q.front == Q.rear) return ERROR;
    *e = Q.front->next->data;
    return OK;
}

/*==================== 事件表（有序链表）操作 ====================*/
Status InitList(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (!(*L)) return OVERFLOW;
    (*L)->next = NULL;
    return OK;
}

// 按事件发生时间递增插入
Status OrderInsert(LinkList *L, Event e)
{
    LinkList p = *L, s;
    while (p->next && p->next->data.OccurTime <= e.OccurTime)
        p = p->next;

    s = (LinkList)malloc(sizeof(LNode));
    if (!s) return OVERFLOW;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

// 删除第一个事件（最早发生的事件）
Status DelFirst(LinkList *L, Event *e)
{
    if (!(*L)->next) return ERROR;
    LinkList p = (*L)->next;
    *e = p->data;
    (*L)->next = p->next;
    free(p);
    return OK;
}

Status ListEmpty(LinkList L)
{
    return L->next == NULL ? OK : ERROR;
}

/*==================== 辅助函数 ====================*/
// 找到最短队列的窗口编号（0~3）
int MinimumQueue()
{
    int i, min = 0;
    int minLen = QueueLength(q[0]);

    for (i = 1; i < MaxQ; i++)
    {
        int len = QueueLength(q[i]);
        if (len < minLen)
        {
            minLen = len;
            min = i;
        }
    }
    return min;
}

/*==================== 业务逻辑 ====================*/
// 开门初始化
void OpenForDay()
{
    int i;
    EventList = NULL;
    if (InitList(&EventList) != OK)
        Error("初始化事件表失败！");

    for (i = 0; i < MaxQ; i++)
    {
        if (InitQueue(&q[i]) != OK)
            Error("初始化窗口队列失败！");
    }

    TotalTime = 0;
    CustomerNum = 0;

    // 插入第一个到达事件，发生时刻为0
    Event en;
    en.OccurTime = 0;
    en.NType = 0;   // 到达事件
    OrderInsert(&EventList, en);
}

// 处理顾客到达事件
void CustomerArrived(Event en)
{
    QElemType customer;
    Event newEvent;
    int intertime, durtime, i;

    CustomerNum++;

    // 随机生成下一位顾客到达间隔和本顾客办理时间
    intertime = Random(1, 5);   // 下一位顾客到达间隔 1~5 分钟
    durtime = Random(1, 30);    // 办理业务时间 1~30 分钟

    customer.ArrivalTime = en.OccurTime;
    customer.Duration = durtime;

    // 若下一位顾客还在营业时间内，则插入下一位到达事件
    if (en.OccurTime + intertime < CloseTime)
    {
        newEvent.OccurTime = en.OccurTime + intertime;
        newEvent.NType = 0;
        OrderInsert(&EventList, newEvent);
    }

    // 找最短队列进入
    i = MinimumQueue();
    EnQueue(&q[i], customer);

    // 若该窗口原来为空，则安排该顾客的离开事件
    if (QueueLength(q[i]) == 1)
    {
        newEvent.OccurTime = en.OccurTime + durtime;
        newEvent.NType = i + 1;   // 第 i+1 个窗口的离开事件
        OrderInsert(&EventList, newEvent);
    }
}

// 处理顾客离开事件
void CustomerDeparture(Event en)
{
    int i = en.NType - 1;   // 窗口编号 0~3
    QElemType customer;
    Event newEvent;

    // 队首顾客离开
    DeQueue(&q[i], &customer);

    // 累加该顾客在银行逗留时间
    TotalTime += en.OccurTime - customer.ArrivalTime;

    // 若队列中还有人，则安排下一位顾客离开事件
    if (QueueEmpty(q[i]) == ERROR)   // 非空
    {
        GetHead(q[i], &customer);
        newEvent.OccurTime = en.OccurTime + customer.Duration;
        newEvent.NType = i + 1;
        OrderInsert(&EventList, newEvent);
    }
}

// 银行模拟主过程
void Bank_Simulation()
{
    Event en;

    OpenForDay();

    while (ListEmpty(EventList) == ERROR)   // 事件表非空
    {
        DelFirst(&EventList, &en);

        if (en.NType == 0)
            CustomerArrived(en);    // 到达事件
        else
            CustomerDeparture(en);  // 离开事件
    }

    printf("====================================\n");
    printf("银行排队离散事件模拟结束\n");
    printf("顾客总人数: %d\n", CustomerNum);
    if (CustomerNum != 0)
        printf("顾客平均逗留时间: %.2f 分钟\n", (double)TotalTime / CustomerNum);
    else
        printf("顾客平均逗留时间: 0.00 分钟\n");
    printf("顾客总逗留时间: %d 分钟\n", TotalTime);
    printf("====================================\n");
}

/*==================== 主函数 ====================*/
int main()
{
    srand((unsigned)time(NULL));

    Bank_Simulation();

    return 0;
}
```

```cpp
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

/*==================== 常量定义 ====================*/
const int CloseTime = 480;   // 营业时间（分钟）
const int MaxQ = 4;          // 4 个服务窗口

/*==================== 数据类型定义 ====================*/
struct Customer
{
    int arrivalTime;   // 到达时刻
    int duration;      // 办理业务所需时间
};

struct Event
{
    int occurTime;     // 事件发生时刻
    int nType;         // 0 到达；1~4 离开
};

/*==================== 模板链队列 ====================*/
template <typename T>
class LinkedQueue
{
private:
    struct Node
    {
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* frontPtr;
    Node* rearPtr;
    int len;

public:
    LinkedQueue()
    {
        frontPtr = rearPtr = new Node(T());
        frontPtr->next = nullptr;
        len = 0;
    }

    ~LinkedQueue()
    {
        clear();
        delete frontPtr;
    }

    void clear()
    {
        Node* p = frontPtr->next;
        while (p)
        {
            Node* temp = p;
            p = p->next;
            delete temp;
        }
        frontPtr->next = nullptr;
        rearPtr = frontPtr;
        len = 0;
    }

    bool empty() const
    {
        return len == 0;
    }

    int size() const
    {
        return len;
    }

    void push(const T& e)
    {
        Node* p = new Node(e);
        rearPtr->next = p;
        rearPtr = p;
        len++;
    }

    bool pop(T& e)
    {
        if (empty()) return false;
        Node* p = frontPtr->next;
        e = p->data;
        frontPtr->next = p->next;
        if (rearPtr == p)
            rearPtr = frontPtr;
        delete p;
        len--;
        return true;
    }

    bool front(T& e) const
    {
        if (empty()) return false;
        e = frontPtr->next->data;
        return true;
    }
};

/*==================== 事件表（有序链表） ====================*/
class EventList
{
private:
    struct Node
    {
        Event data;
        Node* next;
        Node(const Event& e, Node* n = nullptr) : data(e), next(n) {}
    };

    Node* head;

public:
    EventList()
    {
        head = new Node({0, 0});
        head->next = nullptr;
    }

    ~EventList()
    {
        clear();
        delete head;
    }

    void clear()
    {
        Node* p = head->next;
        while (p)
        {
            Node* temp = p;
            p = p->next;
            delete temp;
        }
        head->next = nullptr;
    }

    bool empty() const
    {
        return head->next == nullptr;
    }

    void orderInsert(const Event& e)
    {
        Node* p = head;
        while (p->next && p->next->data.occurTime <= e.occurTime)
            p = p->next;

        Node* s = new Node(e, p->next);
        p->next = s;
    }

    bool delFirst(Event& e)
    {
        if (empty()) return false;
        Node* p = head->next;
        e = p->data;
        head->next = p->next;
        delete p;
        return true;
    }
};

/*==================== 银行模拟类 ====================*/
class BankSimulation
{
private:
    EventList eventList;
    LinkedQueue<Customer> q[MaxQ];
    int totalTime;
    int customerNum;

private:
    int Random(int x, int y)
    {
        return rand() % (y - x + 1) + x;
    }

    int minimumQueue()
    {
        int minIndex = 0;
        int minLen = q[0].size();

        for (int i = 1; i < MaxQ; i++)
        {
            if (q[i].size() < minLen)
            {
                minLen = q[i].size();
                minIndex = i;
            }
        }
        return minIndex;
    }

    void openForDay()
    {
        totalTime = 0;
        customerNum = 0;

        Event firstEvent = {0, 0};   // 第一个到达事件
        eventList.orderInsert(firstEvent);
    }

    void customerArrived(const Event& en)
    {
        customerNum++;

        int intertime = Random(1, 5);   // 到达间隔
        int durtime = Random(1, 30);    // 服务时间

        Customer customer;
        customer.arrivalTime = en.occurTime;
        customer.duration = durtime;

        // 插入下一位顾客到达事件
        if (en.occurTime + intertime < CloseTime)
        {
            Event newEvent = {en.occurTime + intertime, 0};
            eventList.orderInsert(newEvent);
        }

        // 进入最短队列
        int i = minimumQueue();
        q[i].push(customer);

        // 若该窗口原为空，则安排其离开事件
        if (q[i].size() == 1)
        {
            Event leaveEvent = {en.occurTime + durtime, i + 1};
            eventList.orderInsert(leaveEvent);
        }
    }

    void customerDeparture(const Event& en)
    {
        int i = en.nType - 1;
        Customer customer;

        q[i].pop(customer);

        // 累加逗留时间
        totalTime += en.occurTime - customer.arrivalTime;

        // 若队列非空，安排下一位离开
        if (!q[i].empty())
        {
            q[i].front(customer);
            Event leaveEvent = {en.occurTime + customer.duration, i + 1};
            eventList.orderInsert(leaveEvent);
        }
    }

public:
    BankSimulation()
    {
        totalTime = 0;
        customerNum = 0;
    }

    void run()
    {
        openForDay();

        Event en;
        while (!eventList.empty())
        {
            eventList.delFirst(en);

            if (en.nType == 0)
                customerArrived(en);
            else
                customerDeparture(en);
        }

        cout << "====================================\n";
        cout << "银行排队离散事件模拟结束\n";
        cout << "顾客总人数: " << customerNum << "\n";
        cout << "顾客总逗留时间: " << totalTime << " 分钟\n";
        cout << "顾客平均逗留时间: ";

        if (customerNum != 0)
            cout << fixed << setprecision(2)
                 << (double)totalTime / customerNum << " 分钟\n";
        else
            cout << "0.00 分钟\n";

        cout << "====================================\n";
    }
};

/*==================== 主函数 ====================*/
int main()
{
    srand((unsigned)time(nullptr));

    BankSimulation sim;
    sim.run();

    return 0;
}
```