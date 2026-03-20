#include <cstdio>
#include <cstdlib>
#include "../../Status.h"

#define LIST_INIT_SIZE 100   // 线性表存储空间的初始分配量
#define LISTINCREMENT 10     // 线性表存储空间的分配增量

typedef int ElemType;  // ElemType 可以定义成 int, char, float 等

typedef struct
{
    ElemType *elem;  // 存储基址 或 ElemType elem[LIST_INIT_SIZE];
    int length;      // 当前长度
    int list_size;   // 当前分配的存储容量
}SqList;

Status InitList(SqList *L)
{
    L->elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if (!L->elem)                    // 储存分配失败处理
    {
        exit(OVERFLOW);
    }
    L->length = 0;                   // 空表长度为 0
    L->list_size = LIST_INIT_SIZE;   // 初始存储容量
    return OK;
}

Status DestroyList(SqList *L)
{
    if (L->elem)  // 判断表是否存在
    {
        free(L->elem);  // 释放存储空间
        L->elem = NULL;  // NULL -- C风格避免野指针
    }
    return OK;
}

Status ClearList(SqList *L)
{
    if (L->elem)
    {
        L->length = 0;  // 逻辑清空
    }
    return OK;
}

Status ListEmpty(SqList *L)
{
    // 如果为空表返回true, 否则返回 false
    if (!L->length)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 这个函数不用写, 直接调用表 L 的属性 length 即可获取其长度
int ListLength(SqList *L)
{
    return L->length;
}

Status GetElem(SqList *L, int i, ElemType &e)
{
    if (i < 1 || i > L->length)  // 这里 i 最大是 L->length
    {
        return ERROR;
    }
    e = L->elem[i - 1];
    return OK;
}

Status LocateElem(SqList L, int e)
{
    // 返回值为 e 的元素的位序, 否则返回 0
    for (int i = 0; i < L.length; ++i)
    {
        if (e == L.elem[i])
        {
            return i + 1;  // 实际位序应该 + 1
        }
    }
    return 0;  // 未找到
}

Status PriorElem(SqList L, ElemType cur_e, ElemType &pre_e)
{
    ElemType *p = L.elem;
    // 若当前元素无前驱或表为空表
    if ((L.length > 0 && *p == cur_e) || L.length == 0)
    {
        return ERROR;
    }
    ElemType *last = L.elem + L.length - 1;
    while (p < last)
    {
        if (*p == cur_e)
        {
            pre_e = *(p - 1);
            return OK;
        }
        ++p;
    }
    return ERROR;  // 未找到该元素
}

Status NextElem(SqList L, ElemType cur_e, ElemType &next_e)
{
    ElemType *p = L.elem;
    ElemType *last = L.elem + L.length - 1;
    // 表为空或当前元素无后继
    if ((L.length == 0) || (L.length > 0 && *last == cur_e))
    {
        return ERROR;
    }
    for (Status i = 0; i < L.length; ++i)
    {
        if (L.elem[i] == cur_e)
        {
            next_e = L.elem[i + 1];
            return OK;
        }
    }
    return ERROR;  // 未找到该元素
}

Status ListInsert(SqList *L, int i, ElemType e)
{
    if (i < 1 || i > L->length + 1)  // i 可以等于L.length + 1, 即可以进行尾插(在最后一个元素后直接插入)
    {
        return ERROR;
    }
    // 通过指针实现, 第 i 个元素及之后的元素右移, 从最后一个元素开始右移
    if (L->length >= L->list_size)
    {
        // 长度已经达到最大，无法继续插入元素, 需重新进行扩容
        // L->list_size + LISTINCREMENT -- 基于当前容量进行扩容，而不是基于刚开始设置的最大容量 LIST_INIT_SIZE
        ElemType * newbase = (ElemType*)realloc(L->elem, (L->list_size + LISTINCREMENT) * sizeof(ElemType));  
        // 检查是否扩容成功
        if (!newbase)
        {
            exit(OVERFLOW);  // 未扩容成功, 不能进行后续操作, 直接退出程序
        }
        L->elem = newbase;
        // 更新容量
        L->list_size += LISTINCREMENT;
    }
    // p 从第最后一个元素开始, 直到第 i 个元素(包括), 不能直接比较地址大小
    ElemType *p_start = L->elem + i - 1;  // 等价于 &(L->elem[i - 1])
    ElemType *p_end = L->elem + L->length - 1;  // == &(L->elem[L->length - 1])
    for (ElemType * p = p_end; p >= p_start; --p)
    {
        *(p + 1) = *p;
    }
    *p_start = e;
    // 更新表长
    ++L->length;
    return OK;
}

Status ListDelete(SqList *L, Status i, ElemType &e)
{
    // 删除顺序表L的第 i 个元素, 并用 e 返回其值
    if (i < 1 || i > L->length)
    {
        return ERROR;
    }
    // 返回删除的元素值
    e = L->elem[i -1];
    // p 从第 i+1 个元素开始，不断向前移，直到最后一个元素
    // (&L->elem[L->length - 1]) 等价于 (L->elem + L->length - 1)
    ElemType * p_start = L->elem + i;
    ElemType * p_end = L->elem + L->length - 1;
    for (ElemType *p = p_start; p <= p_end; ++p)
    {
        *(p - 1) = *p;
    }
    // 返回要删除的元素的值
    e = *(p_start - 1);
    // 更新表长
    --L->length;
    return OK;
}

// 将表 La, Lb合并到 Lc, 通过提高空间复杂度O(m+n)，从而降低时间复杂度O(m+n)
Status MergeList(SqList La, SqList Lb, SqList *Lc)
{
    // La, Lb必须为值非递减序列
    ElemType * pa = La.elem;
    ElemType * pb = Lb.elem;
    // 先初始化长度, 再初始化Lc的容量 -- 以 La 和 Lb 互斥的情况为准，此时 Lc 最大
    Lc->length = La.length + Lb.length;
    Lc->list_size = Lc->length;  // 容量最少为其长度
    // 为 Lc 分配存储空间
    ElemType * pc = Lc->elem = (ElemType *)malloc(Lc->list_size * sizeof(ElemType));  // 注意这里 Lc->elem 也要更新
    // 获取末尾元素的指针 
    // 这里有更健壮的写法，防止La.length为0:  ElemType * pa_last = (La.length > 0) ? (La.elem + La.length - 1) : (NULL);
    ElemType * pa_last = La.elem + La.length - 1;
    ElemType * pb_last = Lb.elem + Lb.length - 1;
    while (pa <= pa_last && pb <= pb_last)
    {
        if (*pa <= *pb)
        {
            // 可以简写为 *pc++ = *pa++;  这里只能使用后置递增, 先赋值后递增
            *pc = *pa;
            ++pc;
            ++pa;
        }
        else
        {
            *pc++ = *pb++;
        }
    }
    // 当其中一个表元素已经全部遍历完，直接把另一个表的剩余元素加入Lc
    while (pa <= pa_last)
    {
        *pc++ = *pa++;
    }
    while (pb <= pb_last)
    {
        *pc++ = *pb++;
    }
    return OK;
}

// 时间复杂度 O(m+n)
Status UnionList(SqList La, SqList Lb, SqList *Lc)
{
    ElemType * pa = La.elem;
    ElemType * pb = Lb.elem;

    ElemType * pa_last = &La.elem[La.length - 1];
    ElemType * pb_last = &Lb.elem[Lb.length - 1];

    Lc->list_size = La.length + Lb.length;
    ElemType * pc = Lc->elem = (ElemType *)malloc(Lc->list_size * sizeof(ElemType));  // 写 ElemType * pc = 也可以，二者等价

    if (!Lc->elem)  // 如果 Lc->elem 为空, 即存储空间没有分配成功
    {
        exit(OVERFLOW);
    }

    while (pa <= pa_last && pb <= pb_last)
    {
        if (*pa == *pb)
        {
            *pc++ = *pa++;
            ++pb;  // 这里前置递增和后置递增都可以，pa加入后，pb也要自增以便面重复添加
        }
        else if (*pa < *pb)
        {
            *pc++ = *pa++;
        }
        else // *pa > *pb
        {
            *pc++ = *pb++;
        }
    }
    return OK;
}

// --- 辅助打印函数 ---
void PrintList(const char* name, SqList L) {
    printf("[%s]: ", name);
    if (L.length == 0) {
        printf("(Empty)\n");
        return;
    }
    printf("(");
    for (int i = 0; i < L.length; ++i) {
        printf("%d", L.elem[i]);
        if (i < L.length - 1) printf(", ");
    }
    printf(") 长度=%d\n", L.length);
}

// --- 主测试函数 ---
int main() {
    SqList L, La, Lb, Lc;
    ElemType e;
    Status status;

    printf("=== 1. 初始化与基本插入测试 ===\n");
    InitList(&L);
    printf("初始化后是否为空？%s\n", ListEmpty(&L) ? "是" : "否");
    
    // 尾插法插入 1, 2, 3
    for (int i = 1; i <= 3; ++i) {
        ListInsert(&L, i, i * 10); 
    }
    PrintList("初始插入后", L);

    printf("\n=== 2. 指定位置插入测试 ===\n");
    // 在第 2 个位置插入 99 -> 10, 99, 20, 30
    ListInsert(&L, 2, 99);
    PrintList("插入99后", L);

    printf("\n=== 3. 获取与查找测试 ===\n");
    if (GetElem(&L, 2, e) == OK) {
        printf("第 2 个元素的值是：%d\n", e);
    }
    int pos = LocateElem(L, 99);
    printf("元素 99 的位序是：%d\n", pos);
    
    printf("\n=== 4. 前驱与后继测试 ===\n");
    if (PriorElem(L, 99, e) == OK) {
        printf("99 的前驱是：%d\n", e);
    } else {
        printf("99 无前驱\n");
    }

    if (NextElem(L, 99, e) == OK) {
        printf("99 的后继是：%d\n", e);
    } else {
        printf("99 无后继\n");
    }

    printf("\n=== 5. 删除测试 ===\n");
    // 删除第 2 个元素 (99)
    if (ListDelete(&L, 2, e) == OK) {
        printf("删除的元素是：%d\n", e);
        PrintList("删除后", L);
    }

    printf("\n=== 6. 清空测试 ===\n");
    ClearList(&L);
    PrintList("清空后", L);
    printf("清空后长度为：%d\n", ListLength(&L));

    printf("\n=== 7. 归并测试 (MergeList) ===\n");
    InitList(&La); InitList(&Lb);
    // La: 1, 3, 5
    ListInsert(&La, 1, 1); ListInsert(&La, 2, 3); ListInsert(&La, 3, 5);
    // Lb: 2, 4, 6
    ListInsert(&Lb, 1, 2); ListInsert(&Lb, 2, 4); ListInsert(&Lb, 3, 6);
    
    PrintList("La", La);
    PrintList("Lb", Lb);
    
    MergeList(La, Lb, &Lc);
    PrintList("Merge(La, Lb)", Lc);

    printf("\n=== 8. 并集测试 (UnionList - 去重) ===\n");
    ClearList(&Lc);
    // La: 1, 3, 5, 7
    ClearList(&La);
    ListInsert(&La, 1, 1); ListInsert(&La, 2, 3); ListInsert(&La, 3, 5); ListInsert(&La, 4, 7);
    // Lb: 3, 5, 8, 9 (注意 3, 5 重复)
    ClearList(&Lb);
    ListInsert(&Lb, 1, 3); ListInsert(&Lb, 2, 5); ListInsert(&Lb, 3, 8); ListInsert(&Lb, 4, 9);

    PrintList("La (并集源)", La);
    PrintList("Lb (并集源)", Lb);

    UnionList(La, Lb, &Lc);
    PrintList("Union(La, Lb)", Lc);

    printf("\n=== 9. 销毁测试 ===\n");
    DestroyList(&La);
    DestroyList(&Lb);
    DestroyList(&Lc);
    DestroyList(&L);
    printf("所有列表已销毁，程序正常结束。\n");

    system("pause");
    return 0;
}