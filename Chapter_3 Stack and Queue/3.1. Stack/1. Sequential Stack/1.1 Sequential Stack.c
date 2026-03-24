#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

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
    S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S->base)
    {
        exit(OVERFLOW);  // 存储分配失败
    }
    S->top = S->base;  // 栈空, 清空操作是让 top 回到 base 的位置，不能交换
    S->stacksize = STACK_INIT_SIZE;
    return OK;
}

Status DestroyStack(SqStack *S)
{
    if (S->base)
    {
        free(S->base);
        S->base = NULL;
        S->top = NULL;
        S->stacksize = 0;
    }
    return OK;
}

Status ClearStack(SqStack *S)
{
    S->top = S->base;  // 清空操作是让 top 回到 base 的位置，不能交换，栈空
    return OK;
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
    if (S->top - S->base >= S->stacksize)
    {
        // 计算容量应该是 当前容量 + 增量（而不是刚开始分配的容量STACK_INIT_SIZE + 增量）
        S->base = (SElemType*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S->base)
        {
            exit(OVERFLOW);  // 存储分配失败
        }
        S->top = S->base + S->stacksize;
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

void TestStack() {
    SqStack S;
    SElemType e;

    printf("1. 初始化栈...\n");
    InitStack(&S);

    printf("2. 测试连续入栈并触发扩容 (初始大小5)...\n");
    for (int i = 1; i <= 7; i++) {
        Push(&S, i * 10);
        printf("入栈: %d, 当前栈大小: %d\n", i * 10, S.stacksize);
    }

    printf("3. 测试 GetTop...\n");
    if (GetTop(S, &e)) printf("栈顶元素: %d\n", e);

    printf("4. 测试 Pop (出栈 3 个)...\n");
    for (int i = 0; i < 3; i++) {
        Pop(&S, &e);
        printf("出栈: %d\n", e);
    }

    printf("5. 测试 ClearStack...\n");
    ClearStack(&S);
    if (GetTop(S, &e) == ERROR) printf("栈已清空，获取栈顶失败 (预期行为)\n");

    printf("6. 测试空栈 Pop 报错...\n");
    if (Pop(&S, &e) == ERROR) printf("空栈 Pop 失败 (预期报错成功)\n");

    printf("7. 销毁栈...\n");
    DestroyStack(&S);
    printf("测试完成。\n");
}

int main() {
    TestStack();
    system("pause");
    return 0;
}