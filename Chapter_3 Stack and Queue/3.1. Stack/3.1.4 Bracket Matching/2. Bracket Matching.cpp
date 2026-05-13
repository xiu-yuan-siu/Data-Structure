#include <iostream>
#include <stdexcept>
#include <string>  // 使用字符串而非字符数组
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
    // 8.括号匹配(自动初始化和析构销毁)
    bool checkBracket(const string &expre)  // 注意：这里传引用
    {
        this->clear();  // 每次清空传入的栈
        for (char ch : expre)  // 范围 for
        {
            if (ch == '(' || ch == '[')
            {
                this->push(ch);
            }
            else if (ch == ')' || ch == ']')
            {
                if (this->empty())
                {
                    return false;
                }
                char topChar = this->get_top();
                if ((topChar == '(' && ch == ')') || (topChar == '[' && ch == ']'))  // 匹配成功
                {
                    this->pop();
                }
                else  // 匹配失败
                {
                    return false;
                }
            }
        }
        return this->empty();  // 最后看是否栈空
    }
};

int main() {
    // 测试用例：字符串，预期结果
    struct TestCase {
        string str;
        bool expected;
        string desc;
    };

    TestCase tests[] = {
        {"([()])", true,  "完全匹配"},
        {"()[]",   true,  "并列匹配"},
        {"([)]",   false, "交叉嵌套错误"},
        {"(([])",  false, "左括号多余"},
        {"()]]",   false, "右括号多余"},
        {"",       true,  "空字符串"}
    };

    cout << "--- 开始 C++ 模板栈括号匹配测试 ---" << endl;

    SqStack<char> stackHandler; // 创建一个专门负责检查的栈对象, 注意 下括号里面为 char 即 SElemType 为 char 类型

    cout << "--- 开始 C++ 模板栈括号匹配测试 ---" << endl;

    try {
        for (const auto& t : tests) {
            // 修改点：通过对象调用成员函数
            bool result = stackHandler.checkBracket(t.str); 
            
            cout << "内容: [" << t.str << "] \t 预期: " << (t.expected ? "PASS" : "FAIL") 
                 << " \t 实际: " << (result ? "PASS" : "FAIL");
            
            if (result == t.expected) {
                cout << " √" << endl;
            } else {
                cout << " X (错误)" << endl;
            }
        }
    } 
    catch (const exception& e) {
        cerr << "发生异常: " << e.what() << endl;
    }
    // system("pause");
    return 0;
}