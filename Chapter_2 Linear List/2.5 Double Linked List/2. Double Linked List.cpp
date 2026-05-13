#include <iostream>
#include <stdexcept>
using namespace std;

template <class ElemType>
class DuLinkList
{
private:
    struct DuLNode
    {
        DuLNode *prior;
        ElemType data;
        DuLNode *next;
    };
    DuLNode *head;

    // 获取第 i 个元素的指针
    DuLNode* getElemP(int i) const
    {
        // 1 <= i <= n+1
        if (i < 1)  // i 太小
        {
            return nullptr;
        }
        DuLNode *p = head->next;
        int j = 1;
        while (p != head && j < i)
        {
            p = p->next;
            ++j;
        }
        // p == L 仍没有找到 j=i 的情况, 即 i 偏大
        if (j < i)
        {
            return nullptr;
        }
        return p;
    }

public:
    // 1.默认构造初始化
    DuLinkList()
    {
        head = new DuLNode();  // new 分配失败自动报错
        head->prior = head->next = head;
    }
    // 2.析构销毁
    ~DuLinkList()
    {
        if (head)
        {
            clear();
            delete head;
            head = nullptr;
        }
    }
    // 3.清空
    void clear()
    {
        DuLNode *p = head->next;
        while (p != head)
        {
            DuLNode *q = p->next;
            delete p;
            p = q;
        }
        head->prior = head->next = head;
    }
    // 4.获取元素
    ElemType getElem(int i) const
    {
        DuLNode *p = getElemP(i);
        if (!p || p == head)
        {
            throw out_of_range("链表位置越界");
        }
        return p->data;
    }
    // 5.获取位序
    int locateElem(const ElemType &e) const
    {
        DuLNode *p = head->next;
        int i = 1;
        while (p != head)
        {
            if (p->data == e)
            {
                return i;
            }
            p = p->next;
            ++i;
        }
        return 0;
    }
    // 6.获取前驱
    ElemType getPrior(const ElemType &cur_e) const
    {
        DuLNode *p = head->next;
        while (p != head && p->data != cur_e)
        {
            p = p->next;
        }
        if (p == head || p->prior == head)
        {
            throw runtime_error("输入元素不合法, 无前驱");
        }
        return p->prior->data;
    }
    // 7.获取后继
    ElemType getNext(const ElemType &cur_e) const
    {
        DuLNode *p = head->next;
        while (p != head && p->data != cur_e)
        {
            p = p->next;
        }
        if (p == head || p->next == head)
        {
            throw runtime_error("输入元素不合法, 无后继");
        }
        return p->next->data;
    }
    // 8.插入
    void insert(int i, const ElemType &e)
    {
        DuLNode *p = getElemP(i);
        if (!p)
        {
            throw out_of_range("插入位置越界或非法");
        }
        // 等价于
        // DuLNode *s = new DuLNode();
        // s->data = e;
        // s->prior = p->prior;
        // s->next = p;
        DuLNode *s = new DuLNode{p->prior, e, p};
        p->prior->next = s;
        p->prior = s;
    }
    // 9.删除
    void remove(int i)
    {
        DuLNode *p = getElemP(i);
        if (!p || p == head)
        {
            throw out_of_range("删除位置越界或非法");
        }
        p->prior->next = p->next;
        p->next->prior = p->prior;
        delete p;
    }
    // 10.合并
    static void merge(DuLinkList &La, DuLinkList &Lb, DuLinkList &Lc)
    {
        if (Lc.head)
        {
            Lc.clear();
            delete Lc.head;  // 这里要delete Lc的头结点, 之后方便赋值, 防止内存泄露(直接Lc.head = La.head)
        }
        DuLNode *pa = La.head->next;
        DuLNode *pb = Lb.head->next;
        Lc.head = La.head;
        DuLNode *pc = Lc.head;

        while (pa != La.head && pb != Lb.head)
        {
            if (pa->data <= pb->data)
            {
                pc->next = pa;
                pa->prior = pc;
                pc = pa;
                pa = pa->next;
            }
            else
            {
                pc->next = pb;
                pb->prior = pc;
                pc = pb;
                pb = pb->next;
            }
        }
        DuLNode *rest = (pa != La.head) ? pa : pb;  // 剩余部分头结点 遍历完一定会移动到 La.head/Lb.head, 没遍历完则不会
        DuLNode *targetHead = (pa != La.head) ? La.head : Lb.head;  // 目标头结点
        if (rest != targetHead)
        {
            DuLNode *restTail = targetHead->prior;
            pc->next = rest;
            rest->prior = pc;
            restTail->next = Lc.head;  // 尾指针闭环
            Lc.head->prior = restTail;
        }
        else  // 两个表都遍历完, Lc 自身闭合
        {
            pc->next = Lc.head;
            Lc.head->prior = pc;
        }
        if (La.head != Lb.head)
        {
            delete Lb.head;
        }
        La.head = nullptr;  // 防止误用, La已经是合并后的表了, 而不是原来的La表
        Lb.head = nullptr;  // 防止野指针
    }
    // 11.辅助打印函数
    void Print() const 
    {
        cout << "链表预览: [头]";
        DuLNode *p = head->next;
        if (p == head) cout << " <-> (空表)";
        while (p != head) {
            cout << " <-> " << p->data;
            p = p->next;
        }
        cout << " <-> [头]" << endl;
    }
    // 12.判空
    bool isEmpty() const 
    { 
        return head->next == head; 
    }
};

int main() {
    try {
        DuLinkList<int> L, Lb, Lc;

        cout << "================ 基础插入测试 ================" << endl;
        L.insert(1, 10);
        L.insert(2, 30);
        L.insert(2, 20); // [10,20,30]
        L.Print();

        cout << "\n================ getElem 测试 ================" << endl;
        cout << "第1个元素: " << L.getElem(1) << endl;
        cout << "第2个元素: " << L.getElem(2) << endl;

        cout << "\n================ locateElem 测试 ================" << endl;
        cout << "20的位置: " << L.locateElem(20) << endl;
        cout << "100的位置(不存在): " << L.locateElem(100) << endl;

        cout << "\n================ 前驱/后继测试 ================" << endl;
        cout << "20前驱: " << L.getPrior(20) << endl;
        cout << "20后继: " << L.getNext(20) << endl;

        cout << "\n================ 删除测试 ================" << endl;
        L.remove(2); // 删除20
        L.Print();

        cout << "\n================ 判空测试 ================" << endl;
        cout << "是否为空: " << L.isEmpty() << endl;

        cout << "\n================ 清空测试 ================" << endl;
        L.clear();
        L.Print();
        cout << "是否为空: " << L.isEmpty() << endl;

        cout << "\n================ 合并测试 ================" << endl;
        L.insert(1, 30);
        L.insert(1, 10); // [10,30]
        Lb.insert(1, 25);
        Lb.insert(1, 5); // [5,25]

        cout << "L : "; L.Print();
        cout << "Lb: "; Lb.Print();

        DuLinkList<int>::merge(L, Lb, Lc);
        cout << "Lc: "; Lc.Print();

        cout << "\n================ 异常测试 ================" << endl;

        // 1. getElem 越界
        try {
            Lc.getElem(100);
        } catch (const exception& e) {
            cout << "[getElem越界] " << e.what() << endl;
        }

        // 2. 插入越界
        try {
            Lc.insert(100, 999);
        } catch (const exception& e) {
            cout << "[insert越界] " << e.what() << endl;
        }

        // 3. 删除越界
        try {
            Lc.remove(100);
        } catch (const exception& e) {
            cout << "[remove越界] " << e.what() << endl;
        }

        // 4. 前驱异常（第一个元素）
        try {
            cout << Lc.getPrior(5) << endl;
        } catch (const exception& e) {
            cout << "[无前驱] " << e.what() << endl;
        }

        // 5. 后继异常（最后一个元素）
        try {
            cout << Lc.getNext(30) << endl;
        } catch (const exception& e) {
            cout << "[无后继] " << e.what() << endl;
        }

        // 6. 空表操作
        DuLinkList<int> Empty;
        try {
            Empty.getElem(1);
        } catch (const exception& e) {
            cout << "[空表getElem] " << e.what() << endl;
        }

        try {
            Empty.remove(1);
        } catch (const exception& e) {
            cout << "[空表remove] " << e.what() << endl;
        }

        cout << "\n================ 所有测试完成 ================" << endl;

    } catch (const bad_alloc& e) {
        cerr << "内存溢出: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "发生错误: " << e.what() << endl;
    }

    // system("pause");
    return 0;
}
