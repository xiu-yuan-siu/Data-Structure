#include <iostream>
#include <stdexcept>  // 错误处理
using namespace std;

#define LIST_INIT_SIZE 100
#define LIST_INCREMENT 10
// C++ 风格 -- 模板和类, 不返回状态码
template <class ElemType>
class SqList
{
private:
    ElemType * elem;
    int length;
    int list_size;
public:
    // 1.默认构造函数 -- 代替(=) InitList      在实例化类对象时进行初始化 
    SqList()
    {
        elem = new ElemType[LIST_INIT_SIZE];
        // 判断内存空间是否成功分配
        if (!elem)
        {
            throw bad_alloc();
        }
        length = 0;
        list_size = LIST_INIT_SIZE;
    }
    // 2.析构函数 -- = DestroyList
    ~SqList()
    {
        if (elem)
        {
            delete[] elem;
            elem = nullptr;  // 避免野指针
        }
        length = 0;
        list_size = 0;
    }
    // 3.拷贝构造函数(进行深拷贝) -- 避免两个对象指向同一片内存空间(浅拷贝)
    SqList(const SqList &other)
    {
        length = other.length;
        list_size = other.list_size;
        elem = new ElemType[list_size];
        if (!elem)
        {
            throw bad_alloc();
        }
        for (int i = 0; i < length; ++i)
        {
            elem[i] = other.elem[i];
        }
    }
    // 4.重载赋值运算符
    // SqList& -- 支持链式赋值, const -- 保证在重载时不小心修改源对象, SqList& -- 传引用, 避免额外的拷贝开销
    SqList& operator=(const SqList &other)
    {
        // 自赋值检查
        if (this == &other)  // 这里必须对 other取地址, this是指针, other是引用, 不能直接比较
        {
            return *this;
        }
        // 释放旧内存
        delete [] elem;  // 数组删除用 delete []
        length = other.length;
        list_size = other.list_size;
        elem = new ElemType[list_size];
        if (!elem)
        {
            throw bad_alloc();
        }
        for (int i = 0; i < length; ++i)
        {
            elem[i] = other.elem[i];
        }
        return *this;  // 支持链式赋值
    }
    // 5.清空顺序表  等价于(=)书中 ClearList()
    void clear()
    {
        length = 0;
    }
    // 6.判断顺序表是否为空 = ListEmpty(), 为空返回 true
    bool empty()
    {
        return length == 0;
    }
    // 7.重载[]元素安抚获取元素 = GetElem(), C++常用还有 at方法, 我这里不写了
    ElemType& operator[] (int index)
    {
        if (index < 0 || index >= length)
        {
            throw out_of_range("operator[] out of range");
        }
        return elem[index];
    }
    // 8.查找元素, 返回值为 e 的位序(1 - length), 未找到返回 0
    int locate(const ElemType &e) const  // 函数内返回右值int, 返回类型不能为左值引用
    {
        for (int i = 0; i < length; ++i)
        {
            if (elem[i] == e)
            {
                return i + 1;
            }
        }
        return 0;
    }
    // 9.获取前驱 = PriorElem()
    // 结尾的const作用(工程上不能省): 在这个函数内部，绝对不会修改当前对象（SqList）的任何成员变量（如 elem, length, list_size）
    bool prior_elem(const ElemType &cur_e, ElemType &pre_e) const // current e
    {
        // 空表 或 无前驱
        if (length == 0 || cur_e == elem[0])  // length不可能小于 0, 且应该先判空再访问数组, 避免空表时 elem[0] 无法访问
        {
            return false;
        }
        for (int i = 1; i < length; ++i)  // i直接从 1 开始, 0 没有前驱, 已经排除了
        {
            if (cur_e == elem[i])
            {
                pre_e = elem[i - 1];
                return true;
            }
        }
        return false;
    }
    // 10.获取后继 = NextElem()
    bool next_elem(const ElemType &cur_e, ElemType &next_e) const
    {
        // 空表 或 最后一个元素没有后继
        if (length == 0 || elem[length - 1] == cur_e)  
        {
            return false;
        }
        for (int i = 0; i < length - 1; ++i)  // i < length - 1 最后一个元素已经被排除了
        {
            if (elem[i] == cur_e)
            {
                next_e = elem[i + 1];
                return true;
            }
        }
        return false;
    }
    // 11.插入元素 = ListInsert() 把所给值插入到第 i 个元素的前面
    // 我在C风格代码中使用指针, 在C++风格代码中使用[]运算符
    void insert(int i, const ElemType &e)
    {
        // 检查插入位置是否超出索引
        if (i < 1 || i > length + 1)  // i: 1~length
        {
            throw out_of_range("index out of range");
        }
        // 检查容量是否足够
        if (length >= list_size)
        {
            ElemType * new_base = new ElemType[list_size + LIST_INCREMENT];
            if (!new_base)
            {
                throw bad_alloc();
            }
            for (int j = 0; j < length; ++j)
            {
                new_base[j] = elem[j];
            }
            delete[] elem;
            elem = new_base;
            list_size += LIST_INCREMENT;
        }
        // 从最后一个元素开始后移
        for (int k = length - 1; k >= i - 1; --k)  // k 从 length 开始, 即最后一个元素后移
        {
            elem[k + 1] = elem[k];
        }
        elem[i - 1] = e;
        // 更新长度
        ++length;
    }
    // 12.删除元素 = ListDelete()
    ElemType erase(int i)  // delete_val 为局部变量, 不能返回引用
    {
        if (i < 1 || i >= length)
        {
            throw out_of_range("index out of range");
        }
        // 要先记录返回删除的元素
        ElemType delete_val = elem[i - 1];
        // 从第 i+1 个元素开始前移, 下标为 i
        for (int j = i; j < length; ++j)
        {
            elem[j - 1] = elem[j]; 
        }
        // 更新长度
        --length;
        return delete_val;
    }
    // 13.合并顺序表(有重复元素) = MergeList(), 这里不能返回引用, 因为 Lc 为局部变量, 在栈上创建, 函数运行完成就析构了
    // 这里的 static 避免调用不好看, 比如: SqList L3 = L1.merge(L1, L2); --> SqList L3 = SqList<int>::merge(L1, L2);
    static SqList merge(const SqList &La, const SqList &Lb)
    {
        SqList Lc;
        // 预先分配最大存储空间, 并与初始化时分配的空间对比
        int Lc_len = La.length + Lb.length; 
        if (Lc_len > Lc.list_size)
        {
            delete[] Lc.elem;
            Lc.elem = new ElemType[Lc_len];
            // 更新 Lc的容量
            Lc.list_size = Lc_len;
        }
        // 将 La 和 Lb 中的元素添加到 Lc
        int i = 0, j = 0, k = 0;
        while (i < La.length && j < Lb.length)
        {
            if (La.elem[i] <= Lb.elem[j])
            {
                Lc.elem[k++] = La.elem[i++];  // 先赋值再递增 
            }
            else
            {
                Lc.elem[k++] = Lb.elem[j++];
            }
        }
        // 一个表已经遍历完成, 将另一个表的元素放入 Lc -- 下面两个元素只会运行一个
        while (i < La.length)
        {
            Lc.elem[k++] = La.elem[i++];
        }
        while (j < Lb.length)
        {   
            Lc.elem[k++] = Lb.elem[j++];
        }
        // 更新 Lc 的长度
        Lc.length = k; 
        return Lc;
    }
    // 14.两表求交集(无重复元素) = UnionList
    static SqList list_union(const SqList &La, const SqList &Lb)
    {
        SqList Lc;
        // 预先分配最大存储空间, 并与初始化时分配的空间对比
        int Lc_len = La.length + Lb.length;
        if (Lc_len > Lc.list_size)
        {
            delete[] Lc.elem;
            Lc.elem = new ElemType[Lc_len];
            Lc.list_size = Lc_len;
        }
        int i = 0, j = 0, k = 0;
        while (i < La.length && j < Lb.length)
        {
            // 这里要单独考虑 La.elem[i] == Lb.elem[j] 的情况
            if (La.elem[i] < Lb.elem[j])
            {
                Lc.elem[k++] = La.elem[i++];
            }
            else if (La.elem[i] == Lb.elem[j])
            {
                Lc.elem[k++] = La.elem[i++];  // 这里使用 Lb 的元素也可以
                ++j;  // 关键: 避免把此时下标为 j 的重复元素加入 Lc
            }
            else
            {
                Lc.elem[k++] = Lb.elem[j++];
            }
        }
        // 一个表已经遍历完成, 将另一个表的元素放入 Lc -- 下面两个元素只会运行一个
        while (i < La.length)
        {
            Lc.elem[k++] = La.elem[i++];
        }
        while (j < Lb.length)
        {
            Lc.elem[k++] = Lb.elem[j++];
        }
        // 更新 Lc 的长度, 注意: 这里 Lc 的长度 <= Lc_len(La, Lb有重复元素时 < )
        Lc.length = k; 
        return Lc; 
    }
};

int main()
{
    cout << "===== SqList 功能测试 =====" << endl;

    // 1 创建顺序表
    SqList<int> L;
    cout << "是否为空: " << L.empty() << endl;

    // 2 插入元素
    cout << "\n--- 插入元素 ---" << endl;
    L.insert(1, 10);
    L.insert(2, 20);
    L.insert(3, 30);
    L.insert(4, 40);

    cout << "当前顺序表: ";
    for (int i = 0; i < 4; ++i)
        cout << L[i] << " ";
    cout << endl;

    // 3 locate
    cout << "\n--- locate 测试 ---" << endl;
    cout << "20 的位置: " << L.locate(20) << endl;
    cout << "99 的位置: " << L.locate(99) << endl;

    // 4 prior_elem
    cout << "\n--- 前驱测试 ---" << endl;
    int pre;
    if (L.prior_elem(30, pre))
        cout << "30 的前驱: " << pre << endl;
    else
        cout << "没有前驱" << endl;

    // 5 next_elem
    cout << "\n--- 后继测试 ---" << endl;
    int next;
    if (L.next_elem(30, next))
        cout << "30 的后继: " << next << endl;
    else
        cout << "没有后继" << endl;

    // 6 删除元素
    cout << "\n--- 删除元素 ---" << endl;
    int del = L.erase(2);
    cout << "删除元素: " << del << endl;

    cout << "删除后顺序表: ";
    for (int i = 0; i < 3; ++i)
        cout << L[i] << " ";
    cout << endl;

    // 7 拷贝构造函数
    cout << "\n--- 拷贝构造测试 ---" << endl;
    SqList<int> L2(L);

    cout << "L2: ";
    for (int i = 0; i < 3; ++i)
        cout << L2[i] << " ";
    cout << endl;

    // 8 赋值运算符
    cout << "\n--- 赋值运算符测试 ---" << endl;
    SqList<int> L3;
    L3 = L;

    cout << "L3: ";
    for (int i = 0; i < 3; ++i)
        cout << L3[i] << " ";
    cout << endl;

    // 9 clear
    cout << "\n--- clear 测试 ---" << endl;
    L3.clear();
    cout << "L3 是否为空: " << L3.empty() << endl;

    // 10 merge
    cout << "\n--- merge 测试 ---" << endl;
    SqList<int> A;
    SqList<int> B;

    A.insert(1, 1);
    A.insert(2, 3);
    A.insert(3, 5);

    B.insert(1, 2);
    B.insert(2, 3);
    B.insert(3, 6);

    SqList<int> C = SqList<int>::merge(A, B);

    cout << "合并结果: ";
    for (int i = 0; i < 6; ++i)
        cout << C[i] << " ";
    cout << endl;

    // 11 union
    cout << "\n--- union 测试 ---" << endl;

    SqList<int> D;
    SqList<int> E;

    D.insert(1, 1);
    D.insert(2, 3);
    D.insert(3, 5);

    E.insert(1, 3);
    E.insert(2, 4);
    E.insert(3, 6);

    SqList<int> F = SqList<int>::list_union(D, E);

    cout << "并集结果: ";
    for (int i = 0; i < 5; ++i)
        cout << F[i] << " ";
    cout << endl;

    cout << "\n===== 测试完成 =====" << endl;

    system("pause");
    return 0;
}