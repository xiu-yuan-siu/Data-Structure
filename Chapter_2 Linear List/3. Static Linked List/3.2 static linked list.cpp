#include <iostream>
#include <stdexcept>
using namespace std;

#define MAXSIZE 1000

template <class ElemType>
class SLinkList
{
private:
    struct Component {
        ElemType data;
        int cur;
    };
    Component space[MAXSIZE];  // 备用链表
    int S;  // 数据链表头结点下标 -- 头指针

    int Malloc_SL() {
        int i = space[0].cur;
        if (space[0].cur) {
            space[0].cur = space[i].cur;
        }
        return i;
    }
    void Free_SL(int k) {
        space[k].cur = space[0].cur;
        space[0].cur = k;
    }

public:
    SLinkList() 
    {
        for (int i = 0; i < MAXSIZE - 1; ++i)
        {
            space[i].cur = i + 1; 
        }
        space[MAXSIZE - 1].cur = 0;

        S = Malloc_SL();
        if (!S) 
        {
            throw runtime_error("静态池初始化失败: 空间不足");
        }
        space[S].cur = 0;
    }
    void ListInsert(int i, const ElemType &e)
    {
        int j = S;
        for (int k = 1; k < i; ++k)  // i-1
        {
            j = space[j].cur;
        }
        if (!j)  // j = 0
        {
            throw out_of_range("插入位置非法: 位序超出当前链表长度");
        }
        int l = Malloc_SL();
        if (!l)  // 内存分配是否成功
        {
            throw runtime_error("存储空间已满: 无法分配新结点");
        }
        space[l].data = e;
        space[l].cur = space[j].cur;
        space[j].cur = l;
    }
    ElemType ListDelete(int i)
    {
        int j = S;
        for (int k = 0; k < i; ++k)
        {
            j = space[j].cur;
        }
        
        int l = space[j].cur;  // 第 i 个结点
        if (!l)
        {
            throw out_of_range("删除位置非法: 该位置无元素");
        }
        ElemType e = space[l].data;
        space[j].cur = space[l].cur;
        Free_SL(l);
        return e;
    }
    void difference()
    {
        int m, n;
        cout << "请依次输入集合A、B元素的个数: ";
        cin >> m >> n;

        int r = S;  // 尾指针
        cout << "请输入集合 A 中的元素: ";
        for (int i = 0; i < m; ++i)
        {
            int j = Malloc_SL();
            if (!j)
            {
                throw runtime_error("Overflow");
            }
            cin >> space[j].data;
            space[r].cur = j;
            r = j;
        }
        space[r].cur = 0;

        cout << "请输入集合 B 中的元素: ";
        for (int i = 0; i < n; ++i)
        {
            ElemType b;
            cin >> b;
            int k = space[S].cur;  // 从数据链表第一个结点开始
            int p = S;  // p 为 k 的前驱，便于进行删除操作
            while (k && space[k].data != b)
            {
                p = k;
                k = space[k].cur;
            }
            if (!k)  // 无同增 
            {
                int j = Malloc_SL();
                if (!j)
                {
                    throw runtime_error("Overflow");
                }
                space[j].data = b;
                space[j].cur = space[r].cur;
                space[r].cur = j;
                r = j;  // 更新尾指针
            }
            else  // 有同删同
            {
                space[p].cur = space[k].cur;
                Free_SL(k);
                if (k == r)
                {
                    r = p;
                }
            }
        }
    }
    // 禁止修改成员变量 / 禁止调用非const成员函数(如不能调用 ListInsert 或 Malloc_SL, 因为那些函数可能会修改对象)
    int LocateElem(ElemType e) const  
    {
        int i = space[S].cur;  // 指向第一个数据节点
        while (i && space[i].data != e)
        {
            i = space[i].cur;
        }
        return i;
    }
    void Display() const
    {
        int p = space[S].cur;
        if (!p)
        {
            cout << "链表为空" << endl;
            return;
        }
        while (p)
        {
            cout << space[p].data << " ";
            p = space[p].cur;
        }
        cout << endl;
    }
};

int main() {
    try {
        SLinkList<int> L;
        cout << "--- 测试1: 基础插入与显示 ---" << endl;
        L.ListInsert(1, 10);
        L.ListInsert(2, 30);
        L.ListInsert(2, 20); // 10 20 30
        L.Display();

        cout << "--- 测试2: 查找 ---" << endl;
        cout << "元素20的下标: " << L.LocateElem(20) << endl;

        cout << "--- 测试3: 删除 ---" << endl;
        cout << "删除了: " << L.ListDelete(2) << endl;
        L.Display();

        cout << "--- 测试4: 报错触发 (越界插入) ---" << endl;
        L.ListInsert(10, 99); 

    } catch (const exception& e) {
        cerr << "捕获异常: " << e.what() << endl;
    }

    cout << "\n--- 测试5: 集合差运算 ---" << endl;
    try {
        SLinkList<int> L2;
        L2.difference(); // 输入 3 2 | 1 2 3 | 2 4
        L2.Display();    // 输出 1 3 4
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
    system("pause");
    return 0;
}