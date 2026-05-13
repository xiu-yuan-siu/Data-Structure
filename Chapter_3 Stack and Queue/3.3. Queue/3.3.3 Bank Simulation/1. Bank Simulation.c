#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../../Status.h"

/*==================== 常量定义 ====================*/
#define CloseTime 480   // 银行关门时间(营业时间) 480 min = 8 hour
#define MaxQ 4          // 服务窗口数量

/*==================== 数据类型定义 ====================*/
// 事件表(有序链表) -- 记录到达和离开事件
typedef struct
{
    int OccurTime;  // 事件发生时刻(分到达和离开)
    int NType;      // 事件类型(0 表示到达事件; 1~4 表示第 1~4 窗口离开事件)
} Event, ElemType;

// 链表结点 -- 事件表结点(单个事件)
typedef struct LNode
{
    Event data;
    struct LNode *next;
} LNode, *LinkList, *EventList;

// 单链队列 -- 处理事务
// 顾客信息
typedef struct
{
    int ArrivalTime;  // 到达时刻
    int Dutation;     // 办理事务所需时间(单个人逗留时间)
} QElemType;

// 单链队列节点
typedef struct QNode
{
    QElemType data;
    struct QNode *next;
} QNode, *QueuePtr;

// 单链队列
typedef struct
{
    QueuePtr front;
    QueuePtr rear;
} LinkQueue;

/*==================== 事件表（有序链表）操作 ====================*/
Status InitList(LinkList *L)
{
    *L = (LinkList)malloc(sizeof(LNode));
    if (!(*L))
    {
        exit(OVERFLOW);
    }
    (*L)->next = NULL;
    return OK;
}

// 按事件发生时间递增插入
Status OrderInsert(LinkList *L, Event e)
{
    LNode *pre = *L;  // p 的前驱，便于插入
    LNode *p = pre->next;
    while (p && p->data.OccurTime <= e.OccurTime)
    {
        pre = p;
        p = p->next;
    }
    LNode *s = (LNode*)malloc(sizeof(LNode));
    s->data = e;
    s->next = p;
    pre->next = s;
    return OK;
}

// 删除第一个事件(最早发生的事件) -- 首元结点
Status DelFirst(LinkList *L, Event *e)
{
    if (!(*L)->next)
    {
        return ERROR;
    }
    LNode *p = (*L)->next;
    *e = p->data;
    (*L)->next = p->next;
    free(p);
    return OK;
}

Status ListEmpty(LinkList L)
{
    return L->next == NULL ? TRUE : FALSE;
}

/*==================== 链队列操作 ====================*/
Status InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
    if (!Q->front)
    {
        exit(OVERFLOW);
    }
    Q->front->next = NULL;
    return OK;
}

Status QueueEmpty(LinkQueue Q)
{
    return Q.front == Q.rear ? TRUE : FALSE;
}

int QueueLength(LinkQueue Q)
{
    QNode *p = Q.front->next;
    int len = 0;
    while (p)
    {
        ++len;
        p = p->next;
    }
    return len;
}

Status EnQueue(LinkQueue *Q, QElemType e)  // 尾插
{
    QNode *s = (QNode*)malloc(sizeof(QNode));
    if (!s)
    {
        exit(OVERFLOW);
    }
    s->data = e;
    s->next = NULL;
    Q->rear->next = s;
    Q->rear = s;
    return OK;
}

Status DeQueue(LinkQueue *Q, QElemType *e)  // 头删
{
    if (Q->front == Q->rear)
    {
        return ERROR;
    }
    QNode *p = Q->front->next;
    *e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p)  // 队列中只有一个元素的情况
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

/*==================== 全局变量 ====================*/
EventList ev;       // 事件表
Event en;           // 事件
LinkQueue q[MaxQ];  // 4 个客户队列
QElemType customer; // 顾客信息
int TotalTime;      // 总逗留时间
int CustomerNum;    // 顾客总数

/*==================== 辅助函数 ====================*/
// 生成随机 逗留时间 和 到达时间间隔
Status Random(int *durTime, int *interTime)
{
    *durTime = 1 + rand() % 30;  // 每个客户办理业务时间 1~30 min
    *interTime = 1 + rand() % 5;  // 两个相邻到达银行的客户的时间间隔 1~5 min
    return OK;
}

// 找到最短队列的窗口编号(0~3)
int Minimum()
{
    int min = 0;  // 假设 1 号窗口最短, 对应下标为 0
    int minlen = QueueLength(q[0]);  // 假设 1 号窗口最短
    for (int i = 1; i < MaxQ; i++)
    {
        int len = QueueLength(q[i]);
        if (len < minlen)
        {
            minlen = len;
            min = i;
        }
    }
    return min;
}

/*==================== 业务逻辑 ====================*/
// 银行开门初始化
void OpenForDay()
{
    // 初始化总逗留时间和顾客总人数为 0
    TotalTime = 0;
    CustomerNum = 0;
    // 初始化时间链表为空表
    InitList(&ev);
    // 初始化窗口
    for (int i = 0; i < MaxQ; i++)
    {
        InitQueue(&q[i]);  // 取地址符不能掉
    }
    // 初始化第一个用户到达事件并插入到事件表
    en.OccurTime = 0;
    en.NType = 0;
    OrderInsert(&ev, en);
}

// 处理顾客到达事件
void CustomerArrived()
{
    int durtime, intertime;

    Random(&durtime, &intertime);  // 生成随机逗留时间和间隔时间
    int t = en.OccurTime + intertime;  // 下一客户到达时间

    // 若下一位顾客到达时银行未关门, 把到达事件插入事件表
    if (t < CloseTime)
    {
        Event newEvent;  // 到达事件
        newEvent.OccurTime = t;
        newEvent.NType = 0;
        OrderInsert(&ev, newEvent);
    }

    // 找最短队列进入
    int i = Minimum();  // 最短队列下标
    customer.ArrivalTime = en.OccurTime;
    customer.Dutation = durtime;
    ++CustomerNum;  // 生成客户信息后再增加客户数量
    EnQueue(&q[i], customer);

    // 若该窗口原来为空，则安排该顾客的离开时间，只将离开时间插入事件表
    if (QueueLength(q[i]) == 1)  // 前面没人，这名客户来到第 i+1 窗口办理业务并准备离开
    {
        Event newEvent;  // 离开事件
        newEvent.OccurTime = en.OccurTime + durtime;
        newEvent.NType = i + 1;  // 第 i+1 个窗口的离开事件(i为数组下标，实际窗口为 i+1)
        OrderInsert(&ev, newEvent);
    }
}

// 处理顾客离开事件
void CustomerDeparture()
{
    int i = en.NType - 1;  // 窗口编号 1~4 对应数组下标 0~3
    DeQueue(&q[i], &customer);  // 删除第 i+1 个窗口的队头客户
    TotalTime += en.OccurTime - customer.ArrivalTime;  // 离开时间 - 到达事件 = 逗留时间
    // 若队列中还有人，安排下一位顾客离开事件
    if (!QueueEmpty(q[i]))  // 若队列非空
    {
        GetHead(q[i], &customer);
        Event newEvent;  // 离开事件
        newEvent.OccurTime = en.OccurTime + customer.Dutation;
        newEvent.NType = i + 1;
        OrderInsert(&ev, newEvent);
    }
}

// 银行模拟主过程
void Bank_Simulation()
{
    OpenForDay();

    while (!ListEmpty(ev))  // 事件表非空
    {
        DelFirst(&ev, &en);
        if (en.NType == 0)
        {
            CustomerArrived();
        }
        else
        {
            CustomerDeparture();
        }
    }
    printf("====================================\n");
    printf("银行排队离散事件模拟结束\n");
    printf("顾客总人数: %d\n", CustomerNum);
    if (CustomerNum != 0)
        printf("顾客平均逗留时间: %.2f 分钟\n", (double)TotalTime / CustomerNum);
    else
        printf("顾客平均逗留时间: 0.00 分钟\n");
    printf("顾客总逗留时间: %d 分钟\n", TotalTime);
    printf("====================================\n");
}

/*==================== 主函数 ====================*/
int main()
{
    srand((unsigned)time(NULL));

    Bank_Simulation();

    // system("pause");
    return 0;
}