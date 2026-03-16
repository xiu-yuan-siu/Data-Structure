#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

typedef int ElemType;

typedef struct LNode
{
    ElemType data;
    LNode * next;
}LNode, *LinkList;

Status InitList(LinkList *L);
Status DestroyList(LinkList *L);
Status ClearList(LinkList L);
Status GetElem(LinkList L, int i, ElemType *e);  // ElemType 传参时取地址
int LocateElem(LinkList L, ElemType *e);
Status PriorElem(LinkList L, const ElemType *cur_e, ElemType *pre_e);
Status NextElem(LinkList L, const ElemType *cur_e, ElemType *next_e);
Status ListInsert(LinkList L, int i, const ElemType *e);
Status ListDelete(LinkList L, int i, ElemType *e);
void ListMerge(LinkList *La, LinkList *Lb, LinkList *Lc);