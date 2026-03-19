#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef int SElemType;

typedef struct
{
    SElemType *base;  // 栈底指针，构造之前和销毁之后值为 NULL
    SElemType *top;  // 栈顶指针，指向栈顶元素的下一位置
    int stacksize;  // 当前已分配的存储空间，以元素为单位
}SqStack;

Status InitStack(SqStack *S)
{
    (*S).base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!(*S).base)
    {
        exit(OVERFLOW);  // 存储分配失败
    }
    (*S).base = (*S).top;  // 栈空
    (*S).stacksize = STACK_INIT_SIZE;
    return OK;
}

Status DestroyStack(SqStack *S)
{

}

Status ClearStack(SqStack *S)
{

}

Status GetTop(SqStack S, SElemType *e)
{
    // 若栈不为空，返回栈顶元素
    if (S.base == S.top)
    {
        return ERROR;
    }
    *e = *(S.top - 1);
    return OK;
}

Status Push(SqStack *S, SElemType e)
{
    // 先判断是否存储空间已满，已满则增加存储空间
    if ((*S).top - (*S).base >= (*S).stacksize)
    {
        (*S).base = (SElemType*)realloc((*S).base, (STACK_INIT_SIZE + STACKINCREMENT) * sizeof(SElemType));
        if (!(*S).base)
        {
            exit(OVERFLOW);  // 存储分配失败
        }
        (*S).top = (*S).base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    // 插入 e
    // 等价于
    // *(S->top) = e;
    // S->top++;
    *(S->top++) = e;
    return OK;
}

Status Pop(SqStack *S, SElemType *e)
{
    if (S->top == S->base)
    {
        return ERROR;
    }
    // --S->top;
    // *e = *(S->top);
    *e = *(--S->top);
    return OK;
}
