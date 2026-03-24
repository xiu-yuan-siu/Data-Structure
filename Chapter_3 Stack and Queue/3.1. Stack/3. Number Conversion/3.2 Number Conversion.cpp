#include <iostream>
#include <stdexcept>
using namespace std;

template <class SElemType>
class SqStack
{
private:
    // 不存在结点概念(无指针域，不需要结构体)
    SElemType *base;
    SElemType *top;
    int stacksize;

    static const int INIT_SIZE = 100;
    static const int INCREMENT = 10;

public:
    // 1.默认构造
    SqStack()
    {
        base = new SElemType[INIT_SIZE];  // 创建数组用中括号
        if (!base) 
        {
            throw runtime_error("内存分配失败");
        }
        top = base;  // 顺序不能变
        stacksize = INIT_SIZE;
        cout << "栈初始化成功, 容量:" << stacksize << endl;
    }
    // 2.析构函数
    ~SqStack()
    {
        if (base)
        {
            delete[] base;
            base = nullptr;
            top = nullptr;
        }
        cout << "栈已自动销毁" << endl;
    }
    // 3.获取栈顶元素
    SElemType get_top() const  // 限制 *this 指针，表示该函数不会修改类的任何成员变量（除非成员被声明为 mutable）
    {
        if (top == base)
        {
            throw runtime_error("栈空，无法获取栈顶元素");
        }
        return *(top - 1);
    }
    // 4.push 入栈
    void push(const SElemType &e)
    {
        if (top - base >= stacksize)  // top - base -- 元素个数 > 容量 --> 扩容
        {
            int new_size = stacksize + INCREMENT;
            SElemType *new_base = new SElemType[new_size];
            // 把 base 中的旧数据拷贝进 new_base
            for (int i = 0; i < stacksize; ++i)
            {
                new_base[i] = base[i];
            }
            delete[] base;
            base = new_base;
            top = base + stacksize;
            stacksize = new_size;
            cout << "栈已动态扩容，新容量:" << stacksize << endl;
        }
        // 插入栈顶，并将top移动到该元素的下一位置
        *top++ = e;
    }
    // 5.pop 出栈返回元素并把top后移
    SElemType pop()
    {
        if (base == top)
        {
            throw runtime_error("栈空，不能删除栈顶元素");
        }
        return *(--top);
    }
    // 6.清空栈
    void clear()
    {
        top = base;  // 顺序不能交换
    }
    // 7.判空
    bool empty()
    {
        return top == base;
    }
    // 8.数制转换(在类中自动初始化，自动析构)
    void conversion(int N, int d)
    {
        while (N)
        {
            this->push(N % d);
            N /= d;
        }
        while (!this->empty())
        {
            SElemType e;
            e = this->pop();
            cout << e;
        }
    }
};

int main()
{
    int N, d;
    cout << "请输入要转换的十进制数: ";
    cin >> N;
    cout << "请输入要转成的数的进制数: ";
    cin >> d;

    SqStack<int> S;
    S.conversion(N, d);
    cout << endl;

    system("pause");
    return 0;
}