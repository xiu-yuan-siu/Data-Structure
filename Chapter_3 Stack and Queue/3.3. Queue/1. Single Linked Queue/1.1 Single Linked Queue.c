#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

typedef int QElemType;

typedef struct QNode
{
    QElemType data;
    struct QNode *next;
} QNode, *Queueptr;

typedef struct
{
    Queueptr front;  // 队头指针
    Queueptr rear;   // 队尾指针
} LinkQueue;

Status InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = (Queueptr)malloc(sizeof(QNode));
    if (!Q->front)
    {
        exit(OVERFLOW);
    }
    Q->front->next = NULL;
    return OK;
}

Status DestroyQueue(LinkQueue *Q)
{
    while (Q->front)  // 循环结束 front = rear = NULL;
    {
        Q->rear = Q->front->next;  // 这里的 Q->rear 也可以用 QNode *p = Q->front->next; 
        free(Q->front);  // 先进先出，删除头指针
        Q->front = Q->rear;  // 头指针删除后，用Q->front->next给其赋值
    }
    return OK;
}

// 尾插头删 -- 实现先进先出

Status EnQueue(LinkQueue *Q, QElemType e)  // 类似链表的插入, 尾插法
{
    QNode *p = (Queueptr)malloc(sizeof(QNode));
    if (!p)
    {
        exit(OVERFLOW);
    }
    p->data = e;
    p->next = NULL;
    Q->rear->next = p;
    Q->rear = p;
    return OK;
}

Status DeQueue(LinkQueue *Q, QElemType *e)  // 头删法
{
    if (Q->front == Q->rear)  // 队列为空
    {
        return ERROR;
    }
    QNode *p = Q->front->next;
    *e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p)  // 队列只有一个元素，删除后尾指针被释放成为野指针，所以应该先给尾指针赋值
    {
        Q->rear = Q->front;
    }
    free(p);
    return OK;
}

Status GetHead(LinkQueue Q, QElemType *e)
{
    if (Q.front == Q.rear)
    {
        return ERROR;
    }
    *e = Q.front->next->data;
    return OK;
}

void PrintQueueStatus(Status s, const char* msg) {
    if (s == OK) printf("[成功] %s\n", msg);
    else if (s == ERROR) printf("[失败] %s (逻辑错误/队列为空)\n", msg);
    else printf("[异常] %s (内存溢出)\n", msg);
}

int main() {
    LinkQueue Q;
    QElemType e;

    // 1. 测试初始化
    printf("--- 1. 初始化测试 ---\n");
    PrintQueueStatus(InitQueue(&Q), "InitQueue");

    // 2. 测试报错：空队列出队
    printf("\n--- 2. 报错测试：空队列出队 ---\n");
    PrintQueueStatus(DeQueue(&Q, &e), "DeQueue (Empty)");

    // 3. 测试报错：空队列取队头
    printf("\n--- 3. 报错测试：空队列取队头 ---\n");
    PrintQueueStatus(GetHead(Q, &e), "GetHead (Empty)");

    // 4. 入队测试
    printf("\n--- 4. 入队测试 (10, 20, 30) ---\n");
    EnQueue(&Q, 10);
    EnQueue(&Q, 20);
    EnQueue(&Q, 30);
    printf("入队完成。\n");

    // 5. 取队头测试
    if (GetHead(Q, &e) == OK) printf("当前队头元素: %d\n", e);

    // 6. 出队测试
    printf("\n--- 6. 出队测试 ---\n");
    while (DeQueue(&Q, &e) == OK) {
        printf("出队元素: %d\n", e);
    }
    PrintQueueStatus(DeQueue(&Q, &e), "再次出队 (应该失败)");

    // 7. 销毁测试
    printf("\n--- 7. 销毁测试 ---\n");
    PrintQueueStatus(DestroyQueue(&Q), "DestroyQueue");

    system("pause");
    return 0;
}