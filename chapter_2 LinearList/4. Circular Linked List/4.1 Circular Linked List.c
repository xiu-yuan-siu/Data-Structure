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
Status DestroyList(LinkList *L)
{
    ClearList(*L);  // 销毁出头结点外的所有结点，还剩头结点(*L，也是尾指针)
    free(*L);  // 销毁头结点
    *L = NULL;  // 置空, 防止野指针
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

Status GetElem(LinkList L, int i, ElemType *e)
{
    LNode *p = L->next->next;  // 首元结点
    int j = 1;
    while (p != NULL && j < i)
    {
        p = p->next;
        ++j;
    }
    if (!p && j > i)  // i 不合理(偏大或偏小)
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
    if (p == head && p->data == cur_e)  // 空表 或 无前驱
    {
        return ERROR;
    }
    LNode *q = p->next;  // 用q比较，用p返回
    while (p != head)
    {
        if (q->data == cur_e)
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
    
}

Status ListInsert(LinkList *L, int i, const ElemType *e)
{

}

Status ListDelete(LinkList *L, int i, ElemType *e)
{

}

void ListMerge(LinkList *La, LinkList *Lb, LinkList *Lc)
{

}