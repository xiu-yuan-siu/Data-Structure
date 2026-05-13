#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

typedef int ElemType;

typedef struct DuLNode
{
    struct DuLNode *prior;
    ElemType data;
    struct DuLNode *next;
}DuLNode, *DuLinkList;

Status InitList_DuL(DuLinkList *L)
{
    *L = (DuLinkList)malloc(sizeof(DuLNode));
    if (!(*L))
    {
        exit(OVERFLOW);
    }
    (*L)->prior = (*L)->next = *L;  // 头结点的前驱指针和后继指针均指向自身
    return OK;
}

Status DestroyList_DuL(DuLinkList *L)
{
    DuLNode *p = (*L)->next, *q;
    while (p != *L)
    {
        q = p->next;
        free(p);
        p = q;
    }
    free(*L);  // 循环结束 p = q = *L;
    *L = NULL;
    return OK;
}

Status ClearList_DuL(DuLinkList L)
{
    DuLNode *p = L->next, *q;
    while (p != L)
    {
        q = p->next;
        free(p);
        p = q;
    }
    L->prior = L->next = L;
    return OK;
}

// 获取第 i 个元素的指针(内部辅助函数)
DuLinkList GetElemP_DuL(DuLinkList L, int i)
{
    if (i < 1) 
    {
        return NULL;
    }
    DuLNode *p = L->next;
    int j = 1;
    while (p != L && j < i)  // 找到第 i 个元素时退出循环
    {
        p = p->next;
        ++j;
    }
    // 如果循环结束时 j < i，说明链表已经遍历完（p==L）但还没到达指定位置，说明 i 超出了 表长+1
    if (j < i)
    {
        return NULL;
    }
    return p;
}

// 获取第 i 个元素的值
Status GetElem_DuL(DuLinkList L, int i, ElemType *e)
{
    DuLNode *p = GetElemP_DuL(L, i);
    if (!p || p == L)
    {
        return ERROR;  // i 不合理
    }
    *e = p->data;
    return OK;
}

int LocateElem_DuL(DuLinkList L, ElemType *e)
{
    DuLNode *p = L->next;
    int i = 1;
    while (p != L && p->data != *e)
    {
        p = p->next;
        ++i;
    }
    return (p != L) ? i : 0;
}

Status PriorElem_DuL(DuLinkList L, const ElemType cur_e, ElemType *pre_e)
{
    DuLNode *p = L->next;
    while (p != L && p->data != cur_e)
    {
        p = p->next;
    }
    if (p == L || p->prior == L)  // 没找到 cur_e 或 第一个元素无前驱
    {
        return ERROR;
    }
    *pre_e = p->prior->data;
    return OK;
}

Status NextElem_DuL(DuLinkList L, const ElemType cur_e, ElemType *next_e)
{
    DuLNode *p = L->next;
    while (p != L && p->data != cur_e)
    {
        p = p->next;
    }
    if (p == L || p->next == L)  // 没找到 cur_e 或 最后一个元素无后继
    {
        return ERROR;
    }
    *next_e = p->next->data;
    return OK;
}

Status ListInsert_DuL(DuLinkList L, int i, ElemType e)
{
    DuLNode *p = GetElemP_DuL(L, i);
    if (!p)
    {
        return ERROR;
    }
    DuLNode *s = (DuLNode*)malloc(sizeof(DuLNode));
    if (!s)
    {
        return ERROR;  // 内存分配失败
    }
    s->data = e;
    s->prior = p->prior;
    p->prior->next = s;
    p->prior = s;
    s->next = p;
    return OK;
}

Status ListDelete_DuL(DuLinkList L, int i, ElemType *e)
{
    DuLNode *p = GetElemP_DuL(L, i);
    if (!p || p == L)
    {
        return ERROR;
    }
    *e = p->data;
    p->prior->next = p->next;
    p->next->prior = p->prior;
    free(p);
    return OK;
}

Status MergeList_DuL(DuLinkList La, DuLinkList Lb, DuLinkList *Lc)
{
    DuLNode *pa = La->next;  // 首元结点
    DuLNode *pb = Lb->next;
    // 用 La 的头结点作为合并表 Lc 的头结点
    DuLNode *pc = La;  // pc 负责移动, 目前指向 头结点
    *Lc = La;
    // 先遍历完一个链表
    while (pa != La && pb != Lb)
    {
        if (pa->data <= pb->data)
        {
            pc->next = pa;
            pa->prior = pc;
            pc = pa;  // ++pc
            pa = pa->next;  // ++pa
        }
        else
        {
            pc->next = pb;
            pb->prior = pc;
            pc = pb;  
            pb = pb->next;
        }
    }
    // 插入剩余段
    DuLNode *rest = (pa != La) ? pa : pb;  // 剩余段 头结点
    DuLNode *targetHead = (pa != La) ? La : Lb;
    if (rest != targetHead)  // 只有存在数据结点才链接
    {
        DuLNode *restTail = targetHead->prior;  // 剩余段 尾节点

        pc->next = rest;
        rest->prior = pc;
        restTail->next = *Lc;
        (*Lc)->prior = restTail;
    }
    else  // 如果两个表都遍历完, 就让 Lc 自己闭环
    {
        pc->next = *Lc;
        (*Lc)->prior = pc;
    }
    if (La != Lb)
    {
        free(Lb);
    }
    return OK;
}

void PrintList(DuLinkList L) {
    if (!L) {
        printf("打印失败：链表指针为空。\n");
        return;
    }
    DuLinkList p = L->next;
    printf("链表预览: [头]");
    
    if (p == L) {
        printf(" <-> (空表)");
    } else {
        while (p != L) {
            printf(" <-> %d", p->data);
            // 内部校验：顺便检查双向指向是否正确
            if (p->next->prior != p) {
                printf(" [指针异常!] ");
            }
            p = p->next;
        }
    }
    printf(" <-> [头]\n");
}

int main() {
    DuLinkList L = NULL, Lb = NULL, Lc = NULL;
    ElemType e, pre, next;

    printf("=== 1. 初始化与基础插入测试 ===\n");
    InitList_DuL(&L);
    ListInsert_DuL(L, 1, 10); // [10]
    ListInsert_DuL(L, 2, 30); // [10, 30]
    ListInsert_DuL(L, 2, 20); // [10, 20, 30]
    PrintList(L);

    printf("\n=== 2. 边界/错误触发测试 ===\n");
    if (ListInsert_DuL(L, 0, 99) == ERROR) printf("[预期报错] 位置 0 插入非法\n");
    if (ListInsert_DuL(L, 5, 99) == ERROR) printf("[预期报错] 位置 5 插入越界\n");
    if (GetElem_DuL(L, 10, &e) == ERROR)   printf("[预期报错] 获取第 10 个元素不存在\n");

    printf("\n=== 3. 查找、前驱与后继测试 ===\n");
    ElemType target = 20; 
    int pos = LocateElem_DuL(L, &target); // 注意：原函数参数是指针
    printf("元素 20 的位置: %d\n", pos);
    
    if (PriorElem_DuL(L, 10, &pre) == ERROR) 
        printf("[预期报错] 首元素 10 没有前驱\n");
    
    if (NextElem_DuL(L, 30, &next) == ERROR) 
        printf("[预期报错] 尾元素 30 没有后继\n");

    if (PriorElem_DuL(L, 20, &pre) == OK) printf("20 的前驱是: %d\n", pre);
    if (NextElem_DuL(L, 20, &next) == OK) printf("20 的后继是: %d\n", next);

    printf("\n=== 4. 删除测试 ===\n");
    if (ListDelete_DuL(L, 2, &e) == OK) {
        printf("成功删除中间元素: %d\n", e);
        PrintList(L);
    }
    if (ListDelete_DuL(L, 5, &e) == ERROR) printf("[预期报错] 删除位置 5 越界\n");

    printf("\n=== 5. 合并测试 (Lc = L + Lb) ===\n");

    // --- 关键：先重置 L，排除之前边界测试的干扰 ---
    ClearList_DuL(L); 
    ListInsert_DuL(L, 1, 30);
    ListInsert_DuL(L, 1, 10); // 现在 L 是确定的 [10, 30]

    InitList_DuL(&Lb);
    ListInsert_DuL(Lb, 1, 25);
    ListInsert_DuL(Lb, 1, 5);  // Lb 是确定的 [5, 25]

    printf("表 L : "); PrintList(L);
    printf("表 Lb: "); PrintList(Lb);

    MergeList_DuL(L, Lb, &Lc);

    // 合并后 La 的头结点变为了 Lc 的头结点，Lb 的头结点被 free 了
    L = NULL;   
    Lb = NULL;
    printf("合并后 Lc: "); PrintList(Lc);

    printf("\n=== 6. 清空与销毁测试 ===\n");
    ClearList_DuL(Lc);
    printf("清空后: "); PrintList(Lc);
    DestroyList_DuL(&Lc);
    if (Lc == NULL) printf("链表 Lc 销毁成功，指针已置 NULL\n");

    #ifdef _WIN32
    // system("pause");
    #endif
    
    return 0;
}