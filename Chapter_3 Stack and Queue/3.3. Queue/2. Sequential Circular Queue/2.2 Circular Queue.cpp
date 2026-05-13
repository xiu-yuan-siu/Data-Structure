#include <iostream>
#include <stdexcept>
using namespace std;

template <class QElemType>
class SqQueue
{
private:
    QElemType *base;    
    int front;          // 头指针(指向队头元素)
    int rear;           // 尾指针(永远指向队尾元素的下一位置)
    int maxsize = 100;  // 数组最大容量

public:
    SqQueue()
    {
        base = new QElemType[maxsize];  // 数组分配空间用中括号, 内存分配失败会自动报错，不需要单独判断
        front = rear = 0;
    }
    ~SqQueue()
    {
        if (base != nullptr)
        {
            delete[] base;
            base = nullptr;
        }
        front = rear = 0;
    }
    void clear()
    {
        front = rear = 0;
    }
    bool empty() const 
    {
        return front == rear;
    }
    int length() const  // 这里必须加const后面的print才能正常调用
    {
        return (rear - front + maxsize) % maxsize;
    }
    QElemType getHead() const
    {
        if (front == rear)
        {
            throw underflow_error("队列为空，无法获取队头元素!");
        }
        return base[front];
    }
    void insert(const QElemType &e)
    {
        if ((rear + 1) % maxsize == front)  // 数组留一空位区分队空和队满
        {
            throw overflow_error("队列已满，无法插入进行尾插!");
        }
        base[rear] = e;
        rear = (rear + 1) % maxsize;
    }
    QElemType remove()
    {
        if (front == rear)
        {
            throw underflow_error("队列为空，无法删除队头元素!");
        }
        QElemType e = base[front];
        front = (front + 1) % maxsize;
        return e;
    }
    // 打印队列
    void PrintQueue() const
    {
        cout << "队列内容: [ ";
        int i = front;
        while (i != rear)
        {
            cout << base[i] << " ";
            i = (i + 1) % maxsize;
        }
        cout << "]" << endl;
        cout << "front = " << front
             << ", rear = " << rear
             << ", length = " << length() << endl;
    }
};

/* ========================= 测试代码 ========================= */

void TestInitAndEmpty()
{
    cout << "\n================ 测试1：初始化 + 空队列 =================" << endl;
    SqQueue<int> Q;

    cout << "初始化后：" << endl;
    Q.PrintQueue();

    try
    {
        Q.getHead();
        cout << "getHead 测试失败" << endl;
    }
    catch (const exception &e)
    {
        cout << "getHead 测试通过：" << e.what() << endl;
    }

    try
    {
        Q.remove();
        cout << "remove 测试失败" << endl;
    }
    catch (const exception &e)
    {
        cout << "remove 测试通过：" << e.what() << endl;
    }
}

void TestNormalEnremove()
{
    cout << "\n================ 测试2：正常入队/出队 =================" << endl;
    SqQueue<int> Q;

    Q.insert(10);
    Q.insert(20);
    Q.insert(30);

    cout << "入队 10,20,30 后：" << endl;
    Q.PrintQueue();

    cout << "队头元素 = " << Q.getHead() << endl;
    cout << "出队元素 = " << Q.remove() << endl;

    cout << "出队一次后：" << endl;
    Q.PrintQueue();

    cout << "新的队头元素 = " << Q.getHead() << endl;
}

void TestQueueFull()
{
    cout << "\n================ 测试3：队满报错 =================" << endl;
    SqQueue<int> Q; // 实际最多存99个

    try
    {
        for (int i = 1; i <= 99; i++)
        {
            Q.insert(i);
        }

        cout << "填满队列后：" << endl;
        Q.PrintQueue();

        Q.insert(999); // 应报错
        cout << "insert 测试失败：队满时竟然还能入队" << endl;
    }
    catch (const exception &e)
    {
        cout << "insert 测试通过：" << e.what() << endl;
    }
}

void TestCircularBehavior()
{
    cout << "\n================ 测试4：循环队列回绕 =================" << endl;
    SqQueue<int> Q;

    for (int i = 1; i <= 8; i++)
        Q.insert(i);

    cout << "初始入队 1~8：" << endl;
    Q.PrintQueue();

    for (int i = 0; i < 5; i++)
    {
        cout << "出队元素 = " << Q.remove() << endl;
    }

    cout << "出队 5 个后：" << endl;
    Q.PrintQueue();

    for (int i = 100; i <= 105; i++)
        Q.insert(i);

    cout << "再入队 100~105 后（测试循环回绕）：" << endl;
    Q.PrintQueue();
}

void Testclear()
{
    cout << "\n================ 测试5：清空队列 =================" << endl;
    SqQueue<int> Q;

    Q.insert(1);
    Q.insert(2);
    Q.insert(3);

    cout << "清空前：" << endl;
    Q.PrintQueue();

    Q.clear();

    cout << "清空后：" << endl;
    Q.PrintQueue();

    if (Q.empty())
        cout << "clear 测试通过" << endl;
    else
        cout << "clear 测试失败" << endl;
}

/* ========================= 主函数 ========================= */

int main()
{
    TestInitAndEmpty();
    TestNormalEnremove();
    TestQueueFull();
    TestCircularBehavior();
    Testclear();

    // system("pause");
    return 0;
}