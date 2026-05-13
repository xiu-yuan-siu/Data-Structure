#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef char SElemType;  // 注意这里 SElemType 应为 char 做括号匹配

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

Status CheckBracket(const char *expre)
{
    SqStack S;
    InitStack(&S);
    for (int i = 0; expre[i] != '\0'; ++i)
    {
        if (expre[i] == '(' || expre[i] == '[')  // 左括号入栈
        {
            Push(&S, expre[i]);
        }
        else if (expre[i] == ')' || expre[i] == ']')  // 右括号判断与栈顶元素是否匹配，再决定栈顶元素是否出栈
        {
            if (StackEmpty(S))  // 先判断栈是否为空
            {
                DestroyStack(&S);  // 栈空后销毁栈
                return ERROR;
            }
            SElemType topChar;
            GetTop(S, &topChar);
            if ((topChar == '(' && expre[i] == ')') || (topChar == '[' && expre[i] == ']'))  // 匹配成功则出栈
            {
                SElemType e;
                Pop(&S, &e);
            }
            else  // 匹配失败
            {
                DestroyStack(&S);  // 匹配失败销毁栈
                return ERROR;
            }
        }
    }
    Status result = (S.base == S.top) ? OK : ERROR;
    DestroyStack(&S);
    return result;  // 最后看是否栈空，栈空则全部匹配成功
}

int main()
{
    const char *testCases[] = {
        "([()])",     // 成功
        "()[]",       // 成功
        "([)]",       // 失败：顺序错误
        "(([])",      // 失败：左括号多余
        "()]]",       // 失败：右括号多余
        "(((",        // 失败：纯左括号
        ")"           // 失败：纯右括号
    };

    printf("--- 括号匹配测试 ---\n");
    for (int i = 0; i < 7; i++)
    {
        printf("测试案例 %d: %-10s -> ", i + 1, testCases[i]);
        if (CheckBracket(testCases[i]) == OK)
        {
            printf("【匹配成功】\n");
        }
        else
        {
            printf("【匹配失败】\n");
        }
    }

    // system("pause");
    return 0;
}