#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

typedef int SElemType;

typedef struct StackNode
{
    SElemType data;
    struct StackNOde *next;
}StackNode, *LinkStack;

Status InitStack(LinkStack S)
{
    S = NULL;
    S->next = NULL;
    return OK;
}

Status GetTop(LinkStack S, SElemType *e)
{
    if (S == NULL)
    {
        return ERROR;
    }
    *e = S->data;
    return OK;
}

Status Push(LinkStack S, SElemType e)
{
    StackNode *p = (StackNode*)malloc(sizeof(StackNode));
    p->data = e;
    p->next = S;
    S = p;  // 更新头指针
    return OK;
}

Status Pop(LinkStack S, SElemType *e)
{
    if (S == NULL)  // 空栈
    {
        return ERROR;
    }
    StackNode *p = S;
    *e = S->data;
    S = S->next;
    free(p);
    return OK;
}

Status DestroyStack(LinkStack S)
{
    StackNode *p = S;
    while (p)
    {
        StackNode *q = p;
        free(q);
        p = p->next;
    }
    // 循环结束后 p 已经为 NULL，不需要单独释放
    free(S);  // 释放头指针
    return OK;
}

Status ClearStack(LinkStack S)
{
    StackNode *p = S;
    while (p)
    {
        StackNode *q = p;
        free(q);
        p = p->next;
    }
    return OK;
}

Status StackEmpty(LinkStack S)
{
    if (S->next == NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}