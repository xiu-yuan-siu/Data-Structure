#ifndef LINKLIST_H
#define LINKLIST_H

#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

typedef int ElemType;  // 这里可以自定义 ElemType

// 定义带头结点的 节点结构体
typedef struct LNode
{
    ElemType data;
    struct LNode * next;  // C 要写 struct, C++ 可以省略
}LNode, *LinkList;  // 这里节点用LNode定义, 链表用Linklist

// const LinkList L -- const 修饰的是指针(不能防止修改所指值), 为了防止修改传入的链表时, 这里重新定义一个名(满足用头指针LinkList命名链表)
typedef const LNode* constLinkList;

Status InitList(LinkList *L);  // 传 指向指针的指针
Status DestroyList(LinkList *L);
Status ClearList(LinkList L);
Status ListEmpty(constLinkList L);
Status GetElem(constLinkList L, int i, ElemType *e);  // ElemType 传参时取地址
int LocateElem(constLinkList L, ElemType *e);
Status PriorElem(constLinkList L, const ElemType *cur_e, ElemType *pre_e);
Status NextElem(constLinkList L, const ElemType *cur_e, ElemType *next_e);
Status ListInsert(LinkList L, int i, const ElemType *e);
Status ListDelete(LinkList L, int i, ElemType *e);
void HeadCreateList(LinkList *L, int n);  // 头插法
void TailCreateList(LinkList *L, int n);  // 尾插法
void ListMerge(LinkList *La, LinkList *Lb, LinkList *Lc);

#endif