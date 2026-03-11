#include <stdio.h>
#include <stdlib.h>
#include "../Status.h"

#define MAXSIZE 1000  // 链表最大长度

typedef int ElemType;

typedef struct 
{
    ElemType data;
    int cur;
}component, SLinkList[MAXSIZE];  // static linklist

void InitList(SLinkList space)
{
    for (int i = 0; i < MAXSIZE - 1; ++i)
    {
        space[i].cur = i + 1;
    }
    space[MAXSIZE - 1].cur = 0;  // 0 表示 空指针 NULL
}

int Malloc_SL(SLinkList space)
{
    // 若备用空间链表非空，则返回分配的结点下标，否则返回0
    int i = space[0].cur;

}

Status DestroyList(SLinkList *L);
Status ClearList(SLinkList L);
Status ListEmpty(SLinkList L);
Status GetElem(SLinkList L, int i, ElemType *e);  // ElemType 传参时取地址
int LocateElem(SLinkList L, ElemType *e);
Status PriorElem(SLinkList L, const ElemType *cur_e, ElemType *pre_e);
Status NextElem(SLinkList L, const ElemType *cur_e, ElemType *next_e);
Status ListInsert(SLinkList L, int i, const ElemType *e);
Status ListDelete(SLinkList L, int i, ElemType *e);
void HeadCreateList(SLinkList *L, int n);  // 头插法
void TailCreateList(SLinkList *L, int n);  // 尾插法
void ListMerge(SLinkList *La, SLinkList *Lb, SLinkList *Lc);


