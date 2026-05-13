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
    // 8.行编辑程序
    void lineEdit()
    {
        this->clear();  // 保证当前栈为空
        char ch;
        // 使用 cin.get() 逐个读取字符（包括空格）
        while ((ch = cin.get()) != EOF)
        {
            while (ch != EOF && ch != '\n')
            {
                if (ch == '#')
                {
                    if (!this->empty())
                    {
                        this->pop();  // 出栈/弹栈
                    }
                }
                else if (ch == '@')
                {
                    this->clear();  // 栈置空
                }
                else
                {
                    this->push(ch);  // 入栈
                }
                ch = cin.get();
            }
            for (char *p = base; p < top; ++p)
            {
                cout << *p;
            }
            cout << endl;
            this->clear();  // 处理完一行，清空进入下一行
            if (ch == EOF)
            {
                break;
            }
        }
    }
};

/*
* 测试点	    键盘输入内容	     预期输出（按回车后）	      说明
* 基础输入	    hello	            hello	                正常字符入栈输出
* 退格功能	    whli##ile	        while	                # 删除了前面的 i
* 连续退格	    computer###	        compu	                连续三个 # 删除了 ter
* 空栈退格	    a##b	            b	                    栈空时输入 # 应被忽略，不崩溃
* 全行删除	    wrong line@right	right	                @ 之前的全部被清空
* 混合操作	    abc#d@123#4	        124	                    结合了退格和清行

* 结束程序	Ctrl+Z (Windows)	(程序退出)	             测试 EOF 处理
*/

int main() {
    // 实例化一个字符类型的顺序栈
    SqStack<char> stackHandler;

    cout << "========================================" << endl;
    cout << "   C++ 顺序栈 - 行编辑模拟器 (LineEdit)   " << endl;
    cout << "========================================" << endl;
    cout << "使用说明:" << endl;
    cout << "1. 输入文字即可存入栈中" << endl;
    cout << "2. 输入 '#' 撤销上一个字符 (退格)" << endl;
    cout << "3. 输入 '@' 强制清除当前行 (清行)" << endl;
    cout << "4. 按 '回车' 确认并显示处理后的文字" << endl;
    cout << "5. Windows下按 'Ctrl+Z' 并回车结束程序" << endl;
    cout << "----------------------------------------" << endl;

    try {
        // 直接运行行编辑逻辑
        stackHandler.lineEdit();
    } 
    catch (const exception& e) {
        // 捕获构造函数或 push/pop 抛出的异常
        cerr << "\n[运行异常]: " << e.what() << endl;
    }

    cout << "\n--- 程序已安全退出 ---" << endl;
    return 0;
}