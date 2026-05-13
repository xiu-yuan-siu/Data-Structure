#include <iostream>
#include <cassert>
#include <stdexcept>
using namespace std;

template <class ElemType>
class LinkList
{
private:
    // 嵌套结点结构体
    struct LNode
    {
        ElemType data;
        LNode * next;
        // 构造函数 -- 传常量引用，= ElemType() -- 未传参时默认初始化, : data(d), next(nullptr) -- 列表初始化 
        LNode(const ElemType &d = ElemType()) : data(d), next(nullptr) {};
    };
    
    LNode * head;  // 头指针
    int length;  // 防止 ElemType 为 string 时头结点的 data 无法存储长度
public:
    // 1.默认构造函数  = InitList()
    LinkList()
    {
        head = new LNode();  // new出来进行了默认初始化了，head指向的 next 已经是 nullptr;
        length = 0;
    }
    // 2.析构函数 = DestroyList()
    ~LinkList()
    {
        // 先清空其他所有结点
        ClearList();
        // 再释放并置空头结点
        delete head;
        head = nullptr;
    }
    // 拷贝构造 -- 实现深拷贝(直接头指针相等为浅拷贝)，防止两个对象指向同一片内存空间
    LinkList(const LinkList &other)
    {
        head = new LNode();
        head->next = nullptr;
        this->length = other.length;
        LNode *p = other.head->next;
        while (p)
        {
            this->TailInsert(p->data);  // 利用尾插法复制
            p = p->next;
        }
    }
    // 3.清空链表
    void ClearList()
    {
        LNode *p = head->next;
        while (p)
        {
            LNode *q = p;
            p = p->next;
            delete q;
        }
        head->next = nullptr;
    }
    bool ListEmpty() const
    {
        return head->next == nullptr;
        // return length == 0;
    }
    // 重载 []运算符 = GetElem()
    ElemType& operator[](int i)  // return pointer
    {
        LNode *p = head->next;
        int j = 1;
        while (p && j < i)
        {
            p = p->next;
            ++j;
        }
        // 判断i不合理的情况
        if (!p || j > i)
        {
            throw out_of_range("Index out of range");
        }
        return p->data;
    }

    int LocateElem(ElemType &e)
    {
        LNode *p = head->next;
        int j = 1;
        while (p)
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

    bool PriorElem(const ElemType &cur_e, ElemType &pre_e)
    {
        LNode *p = head->next;
        if (!p || p->data == cur_e)  // 空表 或 第一个元素无前驱
        {
            return false;
        }
        while (p->next)
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

    bool NextElem(const ElemType &cur_e, ElemType &next_e)
    {
        LNode *p = head->next;
        while (p && p->next)  // 下面if不作单独判断，直接返回false，故而这里要防止 p 为nullptr(空表)
        {
            if (p->data == cur_e)
            {
                next_e = p->next->data;
                return true;
            }
            p = p->next;
        }
        return false;  // 空表 或 没找到 cur_e
    }

    void ListInsert(int i, const ElemType &e)
    {
        LNode *p = head;
        int j = 0;
        while (p && j < i - 1)
        {
            p = p->next;
            ++j;
        }
        if (!p || j > i - 1)
        {
            throw out_of_range("Index out of range");
        }
        LNode *s = new LNode();
        s->data = e;
        s->next = p->next;
        p->next = s;
        // 更新长度
        ++length;
    }

    void ListDelete(int i, ElemType &e)
    {
        LNode *p = head;
        int j = 0;
        while (p->next && j < i - 1)
        {
            p = p->next;
            ++j;
        }
        if (!p || j > i - 1)
        {
            throw out_of_range("Index out of range");
        }
        LNode *q = p->next;
        e = q->data;
        p->next = q->next;
        delete q;
        // 更新长度
        --length;
        // 下面的代码会造成内存泄漏 -- p->next实际还存在，没有释放，并且不能找到了
        // e = p->next->data;
        // p->next = p->next->next;
    }

    void HeadCreateList(int n)  // 头插法
    {
        ClearList();  // 存在元素则进行清空，或者实例化时调用构造函数已经是空表了，不用new分配内存空间
        for (int i = n; i > 0; --i)
        {
            LNode *p = new LNode();
            cin >> p->data;
            p->next = head->next;
            head->next = p;
            // 更新表长
            ++length;
        }
    }

    void TailCreateList(int n)  // 尾插法
    {
        ClearList();
        LNode *r = head;  // r 为尾指针，永远指向表尾
        for (int i = 0; i < n; ++i)
        {
            LNode *p = new LNode();
            cin >> p->data;
            p->next = nullptr;
            r->next = p;
            r = p;  // = ++r 让 r 永远指向表尾
            // 更新表长
            ++length;
        }
    }

    static void ListMerge(LinkList &La, LinkList &Lb, LinkList &Lc)
    {
        LNode *pa = La.head->next;
        LNode *pb = Lb.head->next;
        LNode *pc = Lc.head;
        while (pa && pb)  // 任意表遍历完就退出循环
        {
            if (pa->data <= pb->data)
            {
                pc->next = pa;  // 赋值
                pc = pa;  // ++pc
                pa = pa->next;  // ++pa
            }
            else
            {
                pc->next = pb;
                pc = pb;
                pb = pb->next;
            }
        }
        // 接入剩下的一张表的剩余部分
        pc->next = pa ? pa : pb;
        // 更新长度
        Lc.length = La.length + Lb.length;
        // 这里不要delete头结点，由析构函数负责
        La.head->next = nullptr;
        La.length = 0;
        Lb.head->next = nullptr;
        Lb.length = 0;
    }

private:
    // 拷贝构造辅助函数 -- 帮助插入元素(TailCreatList是一次性插入), 这个是插入单个元素
    void TailInsert(ElemType &e)
    {
        LNode *p = head;
        while (p->next)
        {
            p = p->next;  // 找尾指针
        }
        p->next = new LNode(e);  // 调用了结构体中 LNode 的构造函数(next默认初始化为nullptr)
        ++length;
    }
};

// 外部打印函数：由于 head 和 LNode 是私有成员，这里利用重载的 [] 操作符遍历打印
// （注：实际工程中建议在 LinkList 类内添加一个 void PrintList() const 成员函数以提高遍历效率）
template <typename T>
void PrintList(LinkList<T>& L) {
    cout << "List: ";
    int i = 1;
    while (true) {
        try {
            // 利用重载的 [] 获取元素
            cout << L[i] << " -> ";
            i++;
        } catch (const out_of_range&) {
            // 捕获到越界异常，说明遍历到底了
            break; 
        }
    }
    cout << "NULL\n";
}

int main() {
    // 1. 实例化模板类，自动调用默认构造函数 (等价于原 InitList)
    LinkList<int> L;
    int e, pre, next;

    cout << "--- Test InitList ---" << endl;
    // 由于 length 和 head 封装在 private 中，这里通过判空和异常来验证
    assert(L.ListEmpty() == true);
    cout << "InitList OK.\n";

    // 2. 测试 ListEmpty
    cout << "\n--- Test ListEmpty ---" << endl;
    assert(L.ListEmpty() == true);
    cout << "List is empty (Correct).\n";

    // 3. 测试 ListInsert (边界逻辑：头、中、尾、越界)
    cout << "\n--- Test ListInsert ---" << endl;
    L.ListInsert(1, 10); // 插在第1个 [10]
    L.ListInsert(2, 30); // 插在第2个 [10, 30]
    L.ListInsert(2, 20); // 插在中间  [10, 20, 30]
    PrintList(L);
    // 验证插入结果
    assert(L[1] == 10 && L[2] == 20 && L[3] == 30);
    
    // 越界测试，捕获异常
    try {
        L.ListInsert(5, 99); 
        assert(false); // 如果没抛出异常，说明逻辑有误，断言失败
    } catch (const out_of_range& err) {
        cout << "Out of bounds insert blocked (Correct). " << err.what() << endl;
    }

    // 4. 测试 GetElem (由重载运算符 [] 代替) & LocateElem
    cout << "\n--- Test Get & Locate ---" << endl;
    e = L[2]; 
    assert(e == 20);
    cout << "Get position 2: " << e << endl;
    
    int target_exist = 30;
    assert(L.LocateElem(target_exist) == 3);
    int target_none = 99;
    assert(L.LocateElem(target_none) == 0);
    cout << "Locate existing and non-existing OK.\n";

    // 5. 测试 PriorElem & NextElem
    cout << "\n--- Test Prior & Next ---" << endl;
    int cur_e = 20;
    L.PriorElem(cur_e, pre);
    L.NextElem(cur_e, next);
    assert(pre == 10 && next == 30);
    cout << "20's Prior: " << pre << ", Next: " << next << endl;

    cur_e = 10;
    assert(L.PriorElem(cur_e, pre) == false); // 第一个没前驱
    cout << "First element has no prior (Correct).\n";

    // 6. 测试 ListDelete
    cout << "\n--- Test ListDelete ---" << endl;
    L.ListDelete(2, e); // 删除 20
    assert(e == 20);
    // 验证剩下的元素是 10 和 30
    assert(L[1] == 10 && L[2] == 30);
    PrintList(L);

    // 7. 测试 ClearList
    cout << "\n--- Test ClearList ---" << endl;
    L.ClearList();
    assert(L.ListEmpty() == true);
    cout << "ClearList OK.\n";
    // C++ 中析构函数会自动管理内存释放，无需显式调用 DestroyList

    // 8. 测试 HeadCreateList (逆序输入) 和 TailCreateList
    cout << "\n--- Test Head/Tail Create ---" << endl;
    cout << "Testing HeadCreateList (please input 2 integers, e.g., 1 2): ";
    L.HeadCreateList(2); 
    PrintList(L); // 输入1 2，应显示 2 -> 1
    
    L.ClearList(); // 先清空

    cout << "Testing TailCreateList (please input 2 integers, e.g., 1 2): ";
    L.TailCreateList(2); 
    PrintList(L); // 输入1 2，应显示 1 -> 2

    // 9. 测试 ListMerge (确保 La, Lb 有序)
    cout << "\n--- Test ListMerge (Sorted) ---" << endl;
    LinkList<int> La, Lb, Lc;
    
    // 构造有序 La: [1, 5, 10]
    La.ListInsert(1, 1);
    La.ListInsert(2, 5);
    La.ListInsert(3, 10);
    
    // 构造有序 Lb: [2, 6, 10, 12]
    Lb.ListInsert(1, 2);
    Lb.ListInsert(2, 6);
    Lb.ListInsert(3, 10);
    Lb.ListInsert(4, 12);
    
    cout << "La: "; PrintList(La);
    cout << "Lb: "; PrintList(Lb);
    
    // 调用静态合并函数
    LinkList<int>::ListMerge(La, Lb, Lc);
    cout << "Merged Lc: "; PrintList(Lc);
    
    // 检查归并后的第一个和最后一个元素是否正确
    assert(Lc[1] == 1);
    assert(Lc[7] == 12);
    
    // 归并后 La 和 Lb 应为空表
    assert(La.ListEmpty() == true); 
    assert(Lb.ListEmpty() == true); 
    cout << "Merge logic and length update OK.\n";

    // 10. 销毁 - 离开作用域时由 ~LinkList() 自动完成
    cout << "\n--- All Tests Passed! ---\n";
    
    // system("pause");
    return 0;
}