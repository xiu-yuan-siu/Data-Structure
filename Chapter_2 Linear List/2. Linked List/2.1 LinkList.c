#include "2.1 LinkList.h"
#include <assert.h>  // 调试

Status InitList(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (!(*L))
    {
        return ERROR;
    }
    (*L)->next = NULL;
    (*L)->data = 0;  // 用头结点的数据域存储 元素个数
    return OK;
}

Status DestroyList(LinkList *L)  // 每个节点都要销毁(包括头节点, 原因:存储空间不连续)
{
    if (L == NULL || *L == NULL)  // 先判断是否传参成功或链表已经为空
    {
        return OK;
    }
    LNode *p;  // p的生命周期随函数结束而结束, 不需要置空
    while (*L)
    {
        p = *L;
        *L = (*L)->next;
        free(p);
    }
    return OK;
}

Status ClearList(LinkList L)  // 头节点指针域置空, 其他每个节点销毁
{
    if (L == NULL)
    {
        return ERROR;
    }
    LNode * p = L->next;
    LNode * q;
    while (p)
    {
        q = p->next;
        free(p);
        p = q;
    }
    L->next = NULL;  // 头节点置空
    L->data = 0;
    return OK;  // 包括正确清空 和 头节点本身就为空 均返回OK
}

Status ListEmpty(constLinkList L)
{
    // 两种判断方式 1.L指针域为空(书中) 2.length == 0(我自己的代码)
    // return L->data == 0;
    if (!L->data)
    {
        return TRUE;
    }
    return FALSE;
}

Status GetElem(constLinkList L, int i, ElemType *e)
{
    LNode *p = L->next;
    int j = 1;
    while (p && j < i)  // p != NULL 且 j = i 之后终止
    {
        p = p->next;
        j++;  // C 循环时习惯后置递增
    }
    // 上述循环结束有两种情况(两种情况 i 的值都不合理): 
    // 1.p到了最后一个节点的指针域(NULL)  说明输入 i 值大于链表长度，i 偏大
    // 2.输入i为 0或负数, 这时 j > i(这种情况其实可以在最前面写，但是两个都返回ERROR的分支写在一起更美观), i 偏小
    if (!p || j > i)  
    {
        return ERROR;
    }
    *e = p->data;
    return OK;
}

int LocateElem(constLinkList L, ElemType *e)
{
    // 在链表中查找元素 e, 找到返回位序, 不存在返回 0
    LNode *p = L->next;
    int j = 1;
    while (p)
    {
        if (p->data == *e)
        {
            return j;
        }
        p = p->next;
        j++;
    }
    return 0;
}

Status PriorElem(constLinkList L, const ElemType *cur_e, ElemType *pre_e)
{
    LNode *p = L->next;
    if (!p || p->data == *cur_e)  // 空表 或 cur_e 为第一个元素，没有直接前驱
    {
        return ERROR;
    }
    LNode *q = NULL;  // 记录上一个元素的节点
    while (p)
    {
        if (p->data == *cur_e)
        {
            *pre_e = q->data;
            return OK;
        }
        q = p;
        p = p->next;
    }
    // 单个指针实现上述循环代码
    // while (p->next)
    // {
    //     if (p->next->data == cur_e)
    //     {
    //         *pre_e = p->data;
    //         return OK;
    //     }
    //     p = p->next;
    // }
    return ERROR;  // 遍历完链表仍未找到
}

Status NextElem(constLinkList L, const ElemType *cur_e, ElemType *next_e)
{
    LNode *p = L->next;
    while (p && p->next)  // p->next->next 会漏掉倒数第 2 个元素，这里写 p 防止空表
    {
        if (p->data == *cur_e)
        {
            *next_e = p->next->data;
            return OK;
        }
        p = p->next;
    }
    return ERROR;  // 空表 或者 无后继 返回 ERROR 
}

Status ListInsert(LinkList L, int i, const ElemType *e)
{
    LNode *p = L;  // 与 j = 0 对应
    int j = 0;  // 书上写的 j = 0, 但我认为 j = 1更好理解
    while (p && j < i - 1)  // 让 p 指向第(i-1)个结点, 这样才能改变第(i-1)个节点的指向(单链表的弊端)
    {
        p = p->next;
        j++;
    }
    if (!p || j > i - 1)  // i 偏大 或 偏小
    {
        return ERROR;
    }
    LNode *s = (LNode*)malloc(sizeof(LNode));
    s->data = *e;
    // 下面两步不能交换位置
    s->next = p->next;
    p->next = s;
    // 更新长度
    L->data++;
    return OK;
}

Status ListDelete(LinkList L, int i, ElemType *e)
{
    LNode *p = L;
    int j = 0;
    while (p->next && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (!p->next || j > i - 1)
    {
        return ERROR;
    }
    // 返回删除的元素, 书中用了一个指针 q，我觉得没有必要
    // *e = p->next->data;
    // p->next = p->next->next;
    LNode *q = p->next;
    *e = q->data;
    p->next = q->next;
    free(q);  // 释放 q 指向的要删除的结点的内存空间，p 指向的结点不能释放，不然就删除了两个元素了
    // 更新长度
    L->data--;
    return OK;
}

void HeadCreateList(LinkList *L, int n)  // 头插法, 逆序输入
{
    *L = (LinkList)malloc(sizeof(LNode));
    (*L)->next = NULL;
    (*L)->data = 0;
    for (int i = n; i > 0; i--)
    {
        LNode *p = (LNode*)malloc(sizeof(LNode));
        scanf("%d", &p->data);  // 这里的取地址符别掉了, 否则是值传递(拷贝), 本例中ElemType设置为 int, 故只接受 int 传入
        // 下面两步不能交换
        p->next = (*L)->next;
        (*L)->next = p;
        (*L)->data++;
    }
}

void TailCreateList(LinkList *L, int n)  // 尾插法
{
    // 方法：通过(头指针+尾指针)实现(通过两个指针实现, 一个永远指向表尾, 一个负责向堆区申请内存将数据搬进来)
    InitList(L);

    LNode *r = *L;  // 尾指针 r 永远指向 L的尾部
    for (int i = 0; i < n; i++)
    {
        LNode *p = (LNode*)malloc(sizeof(LNode));
        scanf("%d", &p->data);
        p->next = NULL;
        r->next = p;  // 第一次循环时就让头结点L指向了p, 之后r移动，但是头结点不动
        r = p;
        (*L)->data++;
    }
    // 实际上，还有其他方法，比如不使用尾指针，但是这样每次都要重新找尾指针，时间复杂度达到了O(n^2), 没有必要
}

void ListMerge(LinkList *La, LinkList *Lb, LinkList *Lc)
{
    LNode *pa = (*La)->next;  // 首元结点
    LNode *pb = (*Lb)->next;
    LNode *pc = *Lc = *La;
    (*Lc)->data = (*La)->data + (*Lb)->data;
    while (pa && pb)  // 结束条件：任意一个表遍历完
    {
        if (pa->data <= pb->data)
        {
            pc->next = pa;  // pc = pa
            pc = pa;  // pc++到pa位置
            pa = pa->next;  // pa++
        }
        else
        {
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
    }
    // 将未遍历完的表的剩余部分加入Lc
    pc->next = pa ? pa : pb;
    free(*Lb);  // 释放Lb, Lb为悬空指针, 访问程序会崩溃, 必须要置空
    *Lb = NULL;
    *La = NULL; // 实际上 La = Lc，二者均可以操作合并后的链表，但实际归并操作对原来的表La进行了破坏，故而也将La置空，避免使用者认为La还是原来的La
}

void PrintList(constLinkList L) {
    LNode *p = L->next;
    printf("List (Length: %d): ", L->data);
    while (p) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}

// 这里如果解除注释那么第四章 Word Index Table的 .c 文件就运行不了， 记得运行后重新注释

// int main() {
//     LinkList L;
//     ElemType e, pre, next;
//     Status s;

//     // 1. 测试 InitList
//     printf("--- Test InitList ---\n");
//     s = InitList(&L);
//     assert(s == OK && L->next == NULL && L->data == 0);
//     printf("InitList OK.\n");

//     // 2. 测试 ListEmpty
//     printf("\n--- Test ListEmpty ---\n");
//     assert(ListEmpty(L) == TRUE);
//     printf("List is empty (Correct).\n");

//     // 3. 测试 ListInsert (边界逻辑：头、中、尾、越界)
//     printf("\n--- Test ListInsert ---\n");
//     e = 10; ListInsert(L, 1, &e); // 插在第1个 [10]
//     e = 30; ListInsert(L, 2, &e); // 插在第2个 [10, 30]
//     e = 20; ListInsert(L, 2, &e); // 插在中间  [10, 20, 30]
//     PrintList(L);
//     assert(L->data == 3);
    
//     s = ListInsert(L, 5, &e);     // 越界测试
//     assert(s == ERROR);
//     printf("Out of bounds insert blocked (Correct).\n");

//     // 4. 测试 GetElem & LocateElem
//     printf("\n--- Test Get & Locate ---\n");
//     GetElem(L, 2, &e);
//     assert(e == 20);
//     printf("Get position 2: %d\n", e);
    
//     e = 30;
//     assert(LocateElem(L, &e) == 3);
//     e = 99;
//     assert(LocateElem(L, &e) == 0);
//     printf("Locate existing and non-existing OK.\n");

//     // 5. 测试 PriorElem & NextElem
//     printf("\n--- Test Prior & Next ---\n");
//     e = 20;
//     PriorElem(L, &e, &pre);
//     NextElem(L, &e, &next);
//     assert(pre == 10 && next == 30);
//     printf("20's Prior: %d, Next: %d\n", pre, next);

//     e = 10;
//     assert(PriorElem(L, &e, &pre) == ERROR); // 第一个没前驱
//     printf("First element has no prior (Correct).\n");

//     // 6. 测试 ListDelete
//     printf("\n--- Test ListDelete ---\n");
//     ListDelete(L, 2, &e); // 删除 20
//     assert(e == 20 && L->data == 2);
//     PrintList(L);

//     // 7. 测试 ClearList
//     printf("\n--- Test ClearList ---\n");
//     ClearList(L);
//     assert(ListEmpty(L) == TRUE && L->next == NULL);
//     printf("ClearList OK.\n");
//     DestroyList(&L);

//     // 8. 测试 HeadCreateList (逆序输入) 和 TailCreateList
//     printf("\n--- Test Head/Tail Create ---\n");
//     printf("Testing HeadCreateList (please input 2 integers, e.g., 1 2): ");
//     HeadCreateList(&L, 2); 
//     PrintList(L); // 输入1 2，应显示 2 -> 1
//     DestroyList(&L);

//     printf("Testing TailCreateList (please input 2 integers, e.g., 1 2): ");
//     TailCreateList(&L, 2); 
//     PrintList(L); // 输入1 2，应显示 1 -> 2

//     // 9. 测试 ListMerge (确保 La, Lb 有序)
//     printf("\n--- Test ListMerge (Sorted) ---\n");
//     LinkList Lb, Lc;
//     InitList(&L);  // 重新初始化 La
//     InitList(&Lb); // 初始化 Lb
    
//     // 构造有序 La: [1, 5, 10]
//     e = 1; ListInsert(L, 1, &e);
//     e = 5; ListInsert(L, 2, &e);
//     e = 10; ListInsert(L, 3, &e);
    
//     // 构造有序 Lb: [2, 6]
//     e = 2; ListInsert(Lb, 1, &e);
//     e = 6; ListInsert(Lb, 2, &e);
//     e = 10; ListInsert(Lb, 3, &e);
//     e = 12; ListInsert(Lb, 4, &e);
    
//     printf("La: "); PrintList(L);
//     printf("Lb: "); PrintList(Lb);
    
//     ListMerge(&L, &Lb, &Lc);
//     printf("Merged Lc: "); PrintList(Lc);
    
//     // 正确的断言：3 + 4 = 7
//     assert(Lc->data == 7); 
//     // 检查归并后的第一个和最后一个元素是否正确
//     GetElem(Lc, 1, &e); assert(e == 1);
//     GetElem(Lc, 7, &e); assert(e == 12);
    
//     assert(Lb == NULL); 
//     printf("Merge logic and length update OK.\n");

//     // 10. 销毁
//     DestroyList(&Lc);
//     printf("\n--- All Tests Passed! ---\n");
    
//     // system("pause");
//     return 0;
// }