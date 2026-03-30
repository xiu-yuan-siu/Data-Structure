#include <iostream>
#include <stdexcept>
using namespace std;

template <class QElemType>
class LinkQueue
{
private:
    struct QNode
    {
        QElemType data;
        QNode *next;
        QNode(QNode *nextPtr = nullptr) : next(nextPtr) {}
        QNode(const QElemType &d, QNode *nextPtr = nullptr) : data(d), next(nextPtr) {}  // 有参构造，列表初始化
    };
    QNode *front;  // 队头指针，指向头结点(不是数据元素)
    QNode *rear;   // 队尾指针，指向最后一个元素
public:
    LinkQueue() 
    {
        front = rear = new QNode();
    }
    ~LinkQueue()
    {
        clear();
        delete front;
    }
    void clear()
    {
        while (front->next != nullptr)
        {
            QNode *temp = front->next;
            front->next = temp->next;
            delete temp;
        }
        // 最后队列为空，但要防止 rear 为野指针
        rear = front;
    }
    void enQueue(const QElemType &e)  // 尾插, 必须要加const才是右值，支持 q.enQueue(10); 这种写法
    {
        QNode *p = new QNode(e, nullptr);
        rear->next = p;
        rear = p;
    }
    QElemType deQueue()  // 头删
    {
        if (front == rear)
        {
            throw runtime_error("Queue is empty!");
        }
        QNode *p = front->next;
        QElemType e = p->data;
        front->next = p->next;
        if (rear == p)  // 删除的是最后一个结点, 删除后队列为空
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
            throw runtime_error("Queue is empty!");
        }
        return front->next->data;
    }
    bool empty() const
    {
        return front == rear;
    }
};

int main() {
    try {
        LinkQueue<int> q;

        cout << "--- 1. 入队测试 (10, 20, 30) ---" << endl;
        q.enQueue(10);
        q.enQueue(20);
        q.enQueue(30);

        cout << "当前队头: " << q.getHead() << endl;

        cout << "\n--- 2. 出队测试 ---" << endl;
        while (!q.empty()) {
            cout << "出队元素: " << q.deQueue() << endl;
        }

        cout << "\n--- 3. 异常报错测试 (空队列出队) ---" << endl;
        q.deQueue(); // 这里会抛出异常

    } catch (const exception& e) {
        cout << "捕获到异常: " << e.what() << endl;
    }
 
    system("pause");
    return 0;
}