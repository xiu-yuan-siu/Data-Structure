#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef int SElemType;

typedef struct
{
    SElemType *base;        // 栈底指针，构造之前和销毁之后值为 NULL
    SElemType *top;         // 栈顶指针，指向栈顶元素的下一位置
    int stacksize;          // 当前已分配的存储空间，以元素为单位
} SqStack;

Status InitStack(SqStack *S);

Status DestroyStack(SqStack *S);

Status ClearStack(SqStack *S);

Status StackEmpty(SqStack S);

Status GetTop(SqStack S, SElemType *e);

Status Push(SqStack *S, SElemType e);

Status Pop(SqStack *S, SElemType *e);

void TestStack();
