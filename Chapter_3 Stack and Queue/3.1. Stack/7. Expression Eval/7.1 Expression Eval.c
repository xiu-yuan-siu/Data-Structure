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

int GetTop(SqStack S)
{
    if (S.base != S.top)
    {
        return *(S.top - 1);
    }
    return ERROR;
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

Status StackEmpty(SqStack S)
{
    return S.base == S.top;
}

// 算符优先级处理(P53 表 3.1)
char Precede(char t1, char t2)
{
    static const char table[7][7] = {
        {'>','>','<','<','<','>','>'}, 
        {'>','>','<','<','<','>','>'},
        {'>','>','>','>','<','>','>'}, 
        {'>','>','>','>','<','>','>'},
        {'<','<','<','<','<','=',' '}, 
        {'>','>','>','>',' ','>','>'},
        {'<','<','<','<','<',' ','='}
    };
    char OP[] = "+-*/()#";  // 算符 = 运算符 + 界限符
    int theta1 = -1, theta2 = -1;  // 必须初始化
    for (int i = 0; i < 7; ++i)
    {
        if (OP[i] == t1)
        {
            theta1 = i;
        }
        if (OP[i] == t2)
        {
            theta2 = i;
        }
    }
    return table[theta1][theta2];
}

// 判断 char c 是否在算符内
Status In(char c, char OP[])
{
    for (int i = 0; i < 7; ++i) 
    {
        if (c == OP[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 算术运算
int Operate(int a, char theta, int b)
{
    switch(theta)
    {
        case '+':
            return a + b;
            break;
        case '-':
            return a - b;
            break;
        case '*':
            return a * b;
            break;
        default:
            return a / b;
            break;
    }
}

// 算符优先算法
int EvaluateExpression()
{
    SqStack OPTR, OPND;
    InitStack(&OPTR);  // 运算符栈
    Push(&OPTR, '#');  // 表达式左边虚设一个 # 号
    InitStack(&OPND);  // 操作数栈
    char c = getchar();
    // 定义算符数组
    char OP[] = "+-*/()#";
    // 
    int a, b, theta, x;
    while (GetTop(OPTR) != '#' || c != '#')
    {
        if (!In(c, OP))  // 不是操作符是操作数 -- 进栈
        {
            Push(&OPND, c - '0');  // c 是字符, '3'(字符三, ASCII = 51) - '0'(ASCII = 48) = 3
            c = getchar();
        }
        else  // 是 算符 = 运算符 + 界限符, 比较 运算符栈栈顶元素 和 用户输入的运算符的优先级 再做相应的操作
        {
            switch (Precede(GetTop(OPTR), c))
            {
                case '<':  // 栈顶元素优先权低，将其入栈
                    Push(&OPTR, c);  
                    c = getchar();
                    break;
                case '=':  // 优先级相等('('和')'优先级相等)，脱括号(出栈)并接受下一字符
                    Pop(&OPTR, &x);
                    c = getchar();
                    break;
                case '>':  // 栈顶元素运算符优先级更高，退栈并计算结果，并把运算结果保留进 OPND 栈
                    Pop(&OPTR, &theta);  // 取运算符
                    Pop(&OPND, &b);      // 取操作数 -- 必须 先 b 后 a
                    Pop(&OPND, &a);
                    Push(&OPND, Operate(a, theta, b));  // 运算结果 入栈 OPND
            }
        }
    }
    return GetTop(OPND);
}

int main() {
    printf("请输入表达式 (案例: 3*(7-2)#): ");
    printf("结果: %d\n", EvaluateExpression());
    // system("pause");
    return 0;
}