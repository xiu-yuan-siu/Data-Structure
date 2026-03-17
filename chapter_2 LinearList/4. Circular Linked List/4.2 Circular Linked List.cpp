#include <iostream>
#include <stdexcept>
using namespace std;

template <class ElemType>
class CLinkList  // Circular Linked List
{
private:
    struct LNode
    {
        ElemType data;
        LNode *next;
        LNode(ElemType d = ElemType(), LNode* n = nullptr) : data(d), next(n) {}  // 默认列表初始化
    };
    LNode *rear;  // 尾指针

public:
    // 1.初始化
    CLinkList()
    {
        LNode* head = new LNode();
        head->next = head;
        rear = head;
    }
    // 2.销毁
    ~CLinkList()
    {
        clear();
        delete rear;  // 销毁头结点
    }
    // 3.清空
    void clear()
    {
        LNode* head = rear->next;
        LNode* p = head->next;
        while (p != head)
        {   
            LNode* temp = p;
            p = p->next;
            delete temp;
        }
        head->next = head;
        rear = head;
    }
    // 4.按位置获取元素
    bool getElem(int i, ElemType& e) const
    {
        LNode* head = rear->next;
        LNode* p = head->next;
        int j = 1;
        while (p != head && j < i)  // j = i 时退出循环
        {
            p = p->next;
            ++j;
        }
        if (p == head || j > i)
        {
            return false;
        }
        e = p->data;
        return true;
    }
    // 5.返回位序
    int LocateElem(const ElemType& e) const
    {
        LNode* head = rear->next;
        LNode* p = head->next;
        int j = 1;
        while (p != head)
        {
            if (p->data == e)
            {
                return j;
            }
            p = p->next;
            ++j;
        }
        return 0;
    }
    // 6.获取前驱
    bool getPrior(const ElemType& cur_e, ElemType& pre_e) const
    {
        LNode* head = rear->next;
        LNode* p = head->next;
        if (p == head || p->data == cur_e)
        {
            return false;
        }
        while (p->next != head)
        {
            if (p->next->data == cur_e)
            {
                pre_e = p->data;
                return true;
            }
            p = p->next;
        }
        return false;
    }
    // 7.获取后继
    bool getNext(const ElemType& cur_e, ElemType& next_e) const
    {
        LNode* head = rear->next;
        LNode* p = head->next;
        while (p != head)
        {
            if (p->data == cur_e && p->next != head)
            {
                next_e = p->next->data;
                return true;
            }
            p = p->next;
        }
        return false;
    }
    // 8.插入
    bool insert(int i, const ElemType& e)
    {
        LNode* head = rear->next;
        LNode* p = head;
        int j = 0;
        while (p->next != head && j < i - 1)  // 找 i 的前驱
        {
            p = p->next;
            ++j;
        }
        if (j > i - 1)
        {
            return false;
        }
        p->next = new LNode(e, p->next);  // 结构体初始化
        // 等价于
        // LNode* q = new LNode();
        // q->data = e;
        // q->next = p->next;
        // p->next = q;
        if (p == rear)  // 插入位置为n+1
        {
            rear = p->next;
        }
        return true;
    }
    // 9.删除
    bool remove(int i, ElemType& e)  // delete 关键字不能用
    {
        LNode* head = rear->next;
        LNode* p = head;
        int j = 0;
        while (p->next != head && j < i - 1)  // 找 i 的前驱
        {
            p = p->next;
            ++j;
        }
        if (p->next == head || j > i - 1)
        {
            return false;
        }
        LNode* temp = p->next;
        e = temp->data;
        p->next = temp->next;
        if (rear == temp)  // 删除的是尾部 n
        {
            rear = p;
        }
        delete temp;
        temp = nullptr;
        return true;
    }
    // 10.合并
    void merge(CLinkList<ElemType>& other)
    {
        if (rear->next == other.rear->next)  // 自身和自身合并直接返回
        {
            return;
        }
        if (other.rear == other.rear->next)  // other 为空表
        {
            return;
        }
        LNode* headA = this->rear->next;
        LNode* headB = other.rear->next;

        this->rear->next = headB->next;  // 把 A 的尾指针指向 B 的首元结点
        other.rear->next = headA;  // B 的尾指针指向 A 的头结点
        this->rear = other.rear;  // 更新合并表的尾指针(原 A 表 this)

        // 被合并的表置空表
        headB->next = headB;  
        other.rear = headB;
    }
    // 11.打印函数：方便观察
    void printList() const {
        LNode* head = rear->next;
        LNode* p = head->next;
        cout << "[Head] -> ";
        while (p != head) {
            cout << p->data << " -> ";
            p = p->next;
        }
        cout << "[Loop Back]" << endl;
    }
};

int main() {
    try {
        CLinkList<int> list1;
        cout << "--- Test 1: Insert ---" << endl;
        list1.insert(1, 10);
        list1.insert(2, 20);
        list1.insert(2, 15); // 中间插入
        list1.printList();   // 预期: 10 -> 15 -> 20

        cout << "\n--- Test 2: Error Handling ---" << endl;
        if (!list1.insert(5, 100)) cout << "Insert pos 5 failed (Correct)" << endl;
        int temp;
        if (!list1.getElem(10, temp)) cout << "Get pos 10 failed (Correct)" << endl;

        cout << "\n--- Test 3: Remove ---" << endl;
        list1.remove(2, temp);
        cout << "Removed: " << temp << endl;
        list1.printList();   // 预期: 10 -> 20

        cout << "\n--- Test 4: Merge ---" << endl;
        CLinkList<int> list2;
        list2.insert(1, 100);
        list2.insert(2, 200);
        cout << "List 1: "; list1.printList();
        cout << "List 2: "; list2.printList();
        list1.merge(list2);
        cout << "After Merge (List 1): "; list1.printList();
        cout << "After Merge (List 2 should be empty): "; list2.printList();

        cout << "\n--- Test 5: Clear and Destroy ---" << endl;
        list1.clear();
        list1.printList();
        cout << "Done without crashing!" << endl;

    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    system("pause");
    return 0;
}