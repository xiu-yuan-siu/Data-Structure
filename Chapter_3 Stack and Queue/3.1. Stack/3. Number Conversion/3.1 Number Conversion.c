#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef int SElemType;

typedef struct
{
    SElemType *top;
    SElemType *base;
    int stacksize;
}SqStack;

Status InitStack(SqStack *S)
{
    S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S->base)
    {
        exit(OVERFLOW);
    }
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return OK;
}

Status DestroyStack(SqStack *S)
{
    if (S->base)
    {
        free(S->base);
        S->base = S->top = NULL;
        S->stacksize = 0;
    }
    return OK;
}

Status ClearStack(SqStack *S)
{
    S->top = S->base;
    return OK;
}

Status GetTop(SqStack S, SElemType *e)
{
    if (S.base == S.top)
    {
        return ERROR;
    }
    *e = *(S.top - 1);
    return OK;
}

Status Push(SqStack *S, SElemType e)
{
    // 扩容
    if (S->top - S->base >= S->stacksize)
    {
        S->base = (SElemType*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S->base)
        {
            exit(OVERFLOW);
        }
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *(S->top++) = e;
    return OK;
}

Status Pop(SqStack *S, SElemType *e)
{
    if (S->base == S->top)
    {
        return ERROR;
    }
    *e = *(--S->top);
    return OK;
}

bool StackEmpty(SqStack S)
{
    return S.base == S.top;
}

void conversion(int N, int d)
{
    SqStack S;
    InitStack(&S);
    while (N)
    {
        Push(&S, N % d);
        N /= d;
    }
    while (!StackEmpty(S))
    {
        SElemType e;
        Pop(&S, &e);
        printf("%d", e);  // 这里 SElemType 为 int
    }
    DestroyStack(&S);
}

int main()
{
    int N;
    printf("请输入要转换的十进制数: ");
    scanf("%d", &N);

    int d;
    printf("请输入要转成的数的进制数: ");
    scanf("%d", &d);

    conversion(N, d);
    printf("\n");
    
    // system("pause");
    return 0;
}