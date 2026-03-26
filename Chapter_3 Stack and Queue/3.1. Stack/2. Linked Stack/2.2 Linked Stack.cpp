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
        StackNode (SElemType d = SElemType(), StackNode *n = nullptr) : data(d), next(nullptr) {};  // 默认列表初始化
    };
    StackNode *head;  // 头指针
public:
    // 默认初始化
    LinkStack()
    {
        head = new StackNode();
        if (!head)
        {
            throw bad_alloc("内存分配失败");
        }
    }
    // 析构函数
    ~LinkStack()
    {
        this->clear();
        delete head;
        head = nullptr;
    }
    void clear()
    {
        StackNode *p = head;
        while (p)
        {
            StackNode *q = p;
            delete q;
            p = p->next;
        }
    }
    SElemType getTop()
    {
        if (head == nullptr)
        {
            throw runtime_error("栈为空，无法获取栈顶元素");
        }
        return head->data;
    }
    void push(SElemType &e)
    {
        StackNode *p = new StackNode();
        p->data = e;
        p->next = head;
        head = p;
    }
    SElemType pop()
    {
        if (head == nullptr)
        {
            throw runtime_error("栈为空，无法删除栈顶元素");
        }
        StackNode *p = S;
        S = S->next;
        SElemType temp = p->data;
        delete p;
        p = nullptr;
        return temp;
    }
    bool empty()
    {
        return head == nullptr;
    }
};