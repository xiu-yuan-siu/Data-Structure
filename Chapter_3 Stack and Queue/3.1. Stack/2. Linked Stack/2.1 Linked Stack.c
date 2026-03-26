#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

typedef int SElemType;

typedef struct StackNode
{
    SElemType data;
    struct StackNode *next;
} StackNode, *LinkStack;

Status InitStack(LinkStack *S)
{
    *S = NULL;
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

Status Push(LinkStack *S, SElemType e)
{
    StackNode *p = (StackNode*)malloc(sizeof(StackNode));
    if (!p)
    {
        exit(OVERFLOW);
    }
    p->data = e;
    p->next = *S;
    *S = p;  // 更新头指针
    return OK;
}

Status Pop(LinkStack *S, SElemType *e)
{
    if (*S == NULL)  // 空栈
    {
        return ERROR;
    }
    StackNode *p = *S;
    *e = p->data;
    *S = p->next;
    free(p);
    return OK;
}

Status DestroyStack(LinkStack *S)
{
    StackNode *p = *S;
    while (p)
    {
        StackNode *q = p;
        p = p->next;  // 先移动 p 防止 p 和 q 指向同一片空间 free(q) 时 p 的数据也没有了
        free(q);
    }
    // 循环结束后 p 已经为 NULL，不需要单独释放
    *S = NULL;  // 释放头指针
    return OK;
}

Status ClearStack(LinkStack *S)
{
    StackNode *p = *S;
    while (p)
    {
        StackNode *q = p;
        p = p->next;
        free(q);
    }
    *S = NULL;
    return OK;
}

Status StackEmpty(LinkStack S)
{
    if (S == NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// ===== 辅助打印函数 =====
void PrintStack(LinkStack S)
{
    printf("栈顶 -> ");
    while (S)
    {
        printf("%d ", S->data);
        S = S->next;
    }
    printf("-> 栈底\n");
}

// ===== 测试主函数 =====
int main()
{
    LinkStack S;
    SElemType x;
    Status ret;

    printf("========== InitStack 测试 ==========\n");
    ret = InitStack(&S);
    printf("InitStack 返回值: %d\n", ret);
    printf("是否为空栈: %s\n", StackEmpty(S) ? "TRUE" : "FALSE");
    PrintStack(S);

    printf("\n========== 空栈 GetTop 报错测试 ==========\n");
    ret = GetTop(S, &x);
    printf("GetTop(空栈) 返回值: %d (期望 ERROR=0)\n", ret);

    printf("\n========== 空栈 Pop 报错测试 ==========\n");
    ret = Pop(&S, &x);
    printf("Pop(空栈) 返回值: %d (期望 ERROR=0)\n", ret);

    printf("\n========== Push 测试 ==========\n");
    Push(&S, 10);
    Push(&S, 20);
    Push(&S, 30);
    PrintStack(S);
    printf("是否为空栈: %s\n", StackEmpty(S) ? "TRUE" : "FALSE");

    printf("\n========== GetTop 正常测试 ==========\n");
    ret = GetTop(S, &x);
    printf("GetTop 返回值: %d, 栈顶元素: %d (期望 30)\n", ret, x);

    printf("\n========== Pop 正常测试 ==========\n");
    ret = Pop(&S, &x);
    printf("Pop 返回值: %d, 出栈元素: %d (期望 30)\n", ret, x);
    PrintStack(S);

    ret = Pop(&S, &x);
    printf("Pop 返回值: %d, 出栈元素: %d (期望 20)\n", ret, x);
    PrintStack(S);

    printf("\n========== ClearStack 测试 ==========\n");
    ret = ClearStack(&S);
    printf("ClearStack 返回值: %d\n", ret);
    printf("是否为空栈: %s\n", StackEmpty(S) ? "TRUE" : "FALSE");
    PrintStack(S);

    printf("\n========== 清空后再次 Pop 报错测试 ==========\n");
    ret = Pop(&S, &x);
    printf("Pop(清空后) 返回值: %d (期望 ERROR=0)\n", ret);

    printf("\n========== 再次 Push 测试 ==========\n");
    Push(&S, 100);
    Push(&S, 200);
    PrintStack(S);

    printf("\n========== DestroyStack 测试 ==========\n");
    ret = DestroyStack(&S);
    printf("DestroyStack 返回值: %d\n", ret);
    printf("是否为空栈: %s\n", StackEmpty(S) ? "TRUE" : "FALSE");
    PrintStack(S);

    system("pause");
    return 0;
}