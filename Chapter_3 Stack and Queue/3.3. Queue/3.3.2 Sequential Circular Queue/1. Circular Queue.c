#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define MAXSIZE 100

typedef int QElemType;

typedef struct
{
    QElemType *base;  // 初始化的动态分配存储空间
    int front;  // 指向队头元素(并非头结点)
    int rear;   // 指向队尾元素的下一位置, 留一空的位置 用去区分 队空(front = rear) 和 队满( (rear+1) % MAXSIZE = front )
} SqQueue;

Status InitQueue(SqQueue *Q)
{
    Q->base = (QElemType*)malloc(MAXSIZE * sizeof(QElemType));
    if (!Q->base)
    {
        exit(OVERFLOW);
    }
    Q->front = Q->rear = 0;
    return OK;
}

Status DestroyQueue(SqQueue *Q)
{
    if (Q->base)  // 先判断指针是否为空
    {
        free(Q->base);
        Q->base = NULL;
    }
    Q->front = Q->rear = 0;
    return OK;
}

int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAXSIZE) % MAXSIZE;
}

Status EnQueue(SqQueue *Q, QElemType e)  // 尾插
{
    if ((Q->rear + 1) % MAXSIZE == Q->front)
    {
        return ERROR;
    }
    Q->base[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXSIZE;  // 更新 rear
    return OK;
}

Status DeQueue(SqQueue *Q, QElemType *e)  // 头删
{
    if (Q->front == Q->rear)  // 队空
    {
        return ERROR;
    }
    *e = Q->base[Q->front];
    Q->front = (Q->front + 1) % MAXSIZE;
    return OK;
}

Status GetHead(SqQueue Q, QElemType *e)
{
    if (Q.front == Q.rear)
    {
        return ERROR;
    }
    *e = Q.base[Q.front];
    return OK;
}

/* ================= 辅助打印函数 ================= */

void PrintQueue(SqQueue Q)
{
    printf("队列内容: [ ");
    int i = Q.front;
    while (i != Q.rear)
    {
        printf("%d ", Q.base[i]);
        i = (i + 1) % MAXSIZE;
    }
    printf("]\n");
    printf("front = %d, rear = %d, length = %d\n", Q.front, Q.rear, QueueLength(Q));
}

/* ================= 测试函数 ================= */

void TestInitAndEmpty()
{
    printf("\n================ 测试1：初始化 + 空队列 =================\n");
    SqQueue Q;
    InitQueue(&Q);

    printf("初始化后：\n");
    PrintQueue(Q);

    QElemType e;
    if (GetHead(Q, &e) == ERROR)
        printf("GetHead 测试通过：空队列取头失败\n");
    else
        printf("GetHead 测试失败\n");

    if (DeQueue(&Q, &e) == ERROR)
        printf("DeQueue 测试通过：空队列出队失败\n");
    else
        printf("DeQueue 测试失败\n");

    DestroyQueue(&Q);
}

void TestNormalEnDeQueue()
{
    printf("\n================ 测试2：正常入队/出队 =================\n");
    SqQueue Q;
    InitQueue(&Q);

    EnQueue(&Q, 10);
    EnQueue(&Q, 20);
    EnQueue(&Q, 30);

    printf("入队 10,20,30 后：\n");
    PrintQueue(Q);

    QElemType e;
    if (GetHead(Q, &e) == OK)
        printf("队头元素 = %d\n", e);

    if (DeQueue(&Q, &e) == OK)
        printf("出队元素 = %d\n", e);

    printf("出队一次后：\n");
    PrintQueue(Q);

    if (GetHead(Q, &e) == OK)
        printf("新的队头元素 = %d\n", e);

    DestroyQueue(&Q);
}

void TestQueueFull()
{
    printf("\n================ 测试3：队满报错 =================\n");
    SqQueue Q;
    InitQueue(&Q);

    int i;
    for (i = 1; i <= MAXSIZE - 1; i++)   // 最多只能放 MAXSIZE-1 个
    {
        if (EnQueue(&Q, i) == ERROR)
        {
            printf("第 %d 次入队提前失败（不应该）\n", i);
            break;
        }
    }

    printf("填满队列后：\n");
    PrintQueue(Q);

    if (EnQueue(&Q, 999) == ERROR)
        printf("EnQueue 测试通过：队满时入队失败\n");
    else
        printf("EnQueue 测试失败：队满时竟然还能入队\n");

    DestroyQueue(&Q);
}

void TestCircularBehavior()
{
    printf("\n================ 测试4：循环队列回绕 =================\n");
    SqQueue Q;
    InitQueue(&Q);

    // 先入队 1~8
    for (int i = 1; i <= 8; i++)
        EnQueue(&Q, i);

    printf("初始入队 1~8：\n");
    PrintQueue(Q);

    // 出队 5 个
    QElemType e;
    for (int i = 0; i < 5; i++)
    {
        DeQueue(&Q, &e);
        printf("出队元素 = %d\n", e);
    }

    printf("出队 5 个后：\n");
    PrintQueue(Q);

    // 再入队，测试 rear 回绕
    for (int i = 100; i <= 105; i++)
        EnQueue(&Q, i);

    printf("再入队 100~105 后（测试循环回绕）：\n");
    PrintQueue(Q);

    DestroyQueue(&Q);
}

void TestDestroy()
{
    printf("\n================ 测试5：销毁队列 =================\n");
    SqQueue Q;
    InitQueue(&Q);

    EnQueue(&Q, 1);
    EnQueue(&Q, 2);

    printf("销毁前：\n");
    PrintQueue(Q);

    DestroyQueue(&Q);

    printf("销毁后：\n");
    printf("base = %p, front = %d, rear = %d\n", (void*)Q.base, Q.front, Q.rear);

    if (Q.base == NULL && Q.front == 0 && Q.rear == 0)
        printf("DestroyQueue 测试通过\n");
    else
        printf("DestroyQueue 测试失败\n");
}

/* ================= 主函数 ================= */

int main()
{
    TestInitAndEmpty();
    TestNormalEnDeQueue();
    TestQueueFull();
    TestCircularBehavior();
    TestDestroy();

    // system("pause");
    return 0;
}