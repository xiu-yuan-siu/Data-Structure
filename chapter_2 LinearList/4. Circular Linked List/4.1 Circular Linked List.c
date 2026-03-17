#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

typedef int ElemType;

typedef struct LNode
{
    ElemType data;
    LNode * next;
}LNode, *LinkList;

Status InitList(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (!(*L))
    {
        exit(OVERFLOW);  // 内存不足，溢出
    }
    (*L)->next = *L;  // 尾指针 *L 指向头结点, 未初始化此时 *L 也是头结点
    return OK;
}

// 释放除头结点外的所有结点
Status ClearList(LinkList L)
{
    LNode *p, *q, *head;
    head = L->next;  // 头指针
    p = head->next;
    while (p != head)  // 循环链表 与 单链表 在 循环条件 判断上有所差别
    {
        q = p->next;
        free(p);
        p = q;
    }
    head->next = head;  // 头结点指向自身, 表示链表为空
    return OK;
}

Status DestroyList(LinkList *L)
{
    if (*L == NULL)
    {
        return OK;
    }
    LNode *head = (*L)->next;
    ClearList(*L);  // 清空所有数据结点, 还剩头结点
    free(head);  // 销毁头结点
    *L = NULL;  // 尾指针置空, 防止野指针
    return OK;
}

Status GetElem(LinkList L, int i, ElemType *e)
{
    if (i < 1)
    {
        return ERROR;
    }
    LNode *head = L->next;
    LNode *p = head->next;  // 首元结点
    int j = 1;
    while (p != head && j < i)
    {
        p = p->next;
        ++j;
    }
    if (p == head || j > i)  // i 不合理(偏大或偏小)
    {
        return ERROR;
    }
    *e = p->data;
    return OK;
}

int LocateElem(LinkList L, const ElemType *e)
{
    LNode *head = L->next;
    LNode *p = head->next;
    int j = 1;
    while (p != head)  // 循环链表的结束条件，查找元素时
    {
        if (p->data == *e)
        {
            return j;
        }
        p = p->next;
        ++j;
    }
    return 0;  // 未找到 或 空表
}

Status PriorElem(LinkList L, const ElemType *cur_e, ElemType *pre_e)
{
    LNode *head = L->next;
    LNode *p = head->next;
    if (p == head && p->data == *cur_e)  // 空表 或 无前驱
    {
        return ERROR;
    }
    LNode *q = p->next;  // 用q比较，用p返回, p 为 q 的前驱
    while (q != head)
    {
        if (q->data == *cur_e)
        {
            *pre_e = p->data;
            return OK;
        }
        p = q;  // ++p
        q = q->next;  // ++q
    }
    return ERROR;  // 未找到 cur_e
}

Status NextElem(LinkList L, const ElemType *cur_e, ElemType *next_e)
{
    LNode *head = L->next;
    LNode *p = head->next;
    while(p != head)
    {
        if (p->data == *cur_e && p->next != head)  // 要考虑有后继的情况
        {
            *next_e = p->next->data;
            return OK;
        }
        p = p->next;
    }
    return ERROR;
}

Status ListInsert(LinkList *L, int i, const ElemType *e)
{
    LNode *head = (*L)->next;
    LNode *p = head;  // 从头结点开始，与 j = 0对应
    int j = 0;
    while (p->next != head && j < i - 1)  // 1 <= i <= n+1
    {
        p = p->next;
        ++j;
    }
    if (j > i - 1)  // 插入时即使p是最后一个结点下一个节点是头结点，也可以追加插入，故不用判断 p->next == head(删除时判断)
    {
        return ERROR;
    }
    LNode *q = (LNode*)malloc(sizeof(LNode));
    q->data = *e;
    q->next = p->next;
    p->next = q;
    if (p == *L) // 如果在尾部插入，更新尾指针
    {
        *L = q;
    }
    return OK;

}

Status ListDelete(LinkList *L, int i, ElemType *e)
{
    LNode *head = (*L)->next;
    LNode *p = head;
    int j = 0;
    while (p->next != head && j < i - 1)  // 1 <= i <= n
    {
        p = p->next;
        ++j;
    }
    if (p->next == head || j > i - 1)
    {
        return ERROR;
    }
    LNode *q = p->next;
    *e = q->data;
    p->next = q->next;
    if (q == *L)  // 删除的是尾节点
    {
        *L = p;
    }
    free(q);
    return OK;
}

LinkList ListMerge(LinkList La, LinkList Lb)
{
    LNode *p = La->next;  // 保存La的头结点
    La->next = Lb->next->next;  // La尾指针指向Lb的首元结点
    free(Lb->next);  // 释放 Lb 的头结点
    Lb->next = p;  // Lb的尾指针指向新表的头结点(原La的头结点)
    return Lb;  // 返回合并表的尾指针
}

void PrintList(LinkList L) {
    if (!L) { printf("List is NULL\n"); return; }
    LNode *head = L->next;
    LNode *p = head->next;
    printf("List: [Head] -> ");
    while (p != head) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("[Head Loop]\n");
}

int main() {
    LinkList L1;
    ElemType e;

    // 1. 初始化测试
    printf("--- Test 1: InitList ---\n");
    InitList(&L1);
    PrintList(L1);

    // 2. 插入测试 (头、中、尾)
    printf("\n--- Test 2: ListInsert ---\n");
    int val1 = 10, val2 = 20, val3 = 30;
    ListInsert(&L1, 1, &val1); // 插入首位: [10]
    ListInsert(&L1, 2, &val2); // 插入末尾: [10, 20]
    ListInsert(&L1, 2, &val3); // 插入中间: [10, 30, 20]
    PrintList(L1);

    // 3. 获取元素测试
    printf("\n--- Test 3: GetElem ---\n");
    if (GetElem(L1, 2, &e)) printf("Element at pos 2: %d\n", e);
    if (GetElem(L1, 5, &e) == ERROR) printf("Pos 5: Correctly caught out of bounds.\n");

    // 4. 定位元素
    printf("\n--- Test 4: LocateElem ---\n");
    int target = 30;
    printf("Pos of value 30: %d\n", LocateElem(L1, &target));

    // 5. 前驱与后继
    printf("\n--- Test 5: Prior & Next ---\n");
    ElemType pre, nxt;
    int cur = 30;
    if (PriorElem(L1, &cur, &pre)) printf("Prior of 30: %d\n", pre);
    if (NextElem(L1, &cur, &nxt)) printf("Next of 30: %d\n", nxt);
    
    int first = 10;
    if (PriorElem(L1, &first, &pre) == ERROR) printf("10 has no prior (Correct)\n");

    // 6. 删除测试
    printf("\n--- Test 6: ListDelete ---\n");
    ListDelete(&L1, 1, &e); // 删除10
    printf("Deleted: %d. ", e);
    PrintList(L1);
    ListDelete(&L1, 2, &e); // 删除尾部20
    printf("Deleted: %d. ", e);
    PrintList(L1);

    // 7. 合并测试
    printf("\n--- Test 7: ListMerge ---\n");
    LinkList L2;
    InitList(&L2);
    int a = 100, b = 200;
    ListInsert(&L2, 1, &a);
    ListInsert(&L2, 2, &b);
    printf("L1: "); PrintList(L1);
    printf("L2: "); PrintList(L2);
    L1 = ListMerge(L1, L2);
    printf("Merged: "); PrintList(L1);

    // 8. 销毁
    printf("\n--- Test 8: Destroy ---\n");
    DestroyList(&L1);
    if (L1 == NULL) printf("List successfully destroyed.\n");

    system("pause");
    return 0;
}