#include <iostream>
#include <stdexcept>
using namespace std;

template <class SElemType>
class LinkStack
{
private:
    struct StackNode 
    {
        SElemType data;
        StackNode *next;
        StackNode (SElemType d = SElemType(), StackNode *n = nullptr) : data(d), next(n) {};  // 默认列表初始化
    };
    StackNode *head;  // 头指针
public:
    // 默认初始化
    LinkStack() : head(nullptr) {}
    // 析构函数
    ~LinkStack()
    {
        this->clear();  // 不带头结点 clear 就是放了所有节点
    }
    void clear()
    {
        StackNode *p = head;
        while (p)
        {
            StackNode *q = p;
            p = p->next;
            delete q;
        }
        head = nullptr;
    }
    bool empty()
    {
        return head == nullptr;
    }
    SElemType getTop() const
    {
        if (head == nullptr)
        {
            throw runtime_error("栈为空，无法获取栈顶元素");
        }
        return head->data;
    }
    void push(const SElemType &e)
    {
        StackNode *p = new StackNode(e, head);
        head = p;
    }
    SElemType pop()
    {
        if (head == nullptr)
        {
            throw runtime_error("栈为空，无法删除栈顶元素");
        }
        StackNode *p = head;
        head = head->next;
        SElemType temp = p->data;
        delete p;
        return temp;
    }
    void print() const
    {
        StackNode *p = head;
        cout << "栈顶 -> ";
        while (p)
        {
            cout << p->data << " ";
            p = p->next;
        }
        cout << "-> 栈底" << endl;
    }
};

int main()
{
    LinkStack<int> S;

    cout << "========== 初始化测试 ==========" << endl;
    cout << "empty(): " << (S.empty() ? "true" : "false") << endl;
    S.print();

    cout << "\n========== 空栈 getTop 异常测试 ==========" << endl;
    try
    {
        cout << "栈顶元素: " << S.getTop() << endl;
    }
    catch (const exception &e)
    {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== 空栈 pop 异常测试 ==========" << endl;
    try
    {
        cout << "出栈元素: " << S.pop() << endl;
    }
    catch (const exception &e)
    {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== push 测试 ==========" << endl;
    S.push(10);
    S.push(20);
    S.push(30);
    S.print();
    cout << "empty(): " << (S.empty() ? "true" : "false") << endl;

    cout << "\n========== getTop 正常测试 ==========" << endl;
    try
    {
        cout << "栈顶元素: " << S.getTop() << " (期望 30)" << endl;
    }
    catch (const exception &e)
    {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== pop 正常测试 ==========" << endl;
    try
    {
        cout << "出栈元素: " << S.pop() << " (期望 30)" << endl;
        S.print();

        cout << "出栈元素: " << S.pop() << " (期望 20)" << endl;
        S.print();
    }
    catch (const exception &e)
    {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== clear 测试 ==========" << endl;
    S.clear();
    cout << "empty(): " << (S.empty() ? "true" : "false") << endl;
    S.print();

    cout << "\n========== clear 后 pop 异常测试 ==========" << endl;
    try
    {
        cout << "出栈元素: " << S.pop() << endl;
    }
    catch (const exception &e)
    {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== clear 后 getTop 异常测试 ==========" << endl;
    try
    {
        cout << "栈顶元素: " << S.getTop() << endl;
    }
    catch (const exception &e)
    {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== 再次 push 测试 ==========" << endl;
    S.push(100);
    S.push(200);
    S.print();

    system("pause");
    return 0;
}