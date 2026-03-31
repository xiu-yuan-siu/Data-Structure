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

}

// 按事件发生时间递增插入
Status OrderInsert(LinkList *L, Event e)
{

}

// 删除第一个事件(最早发生的事件)
Status DelFirst(LinkList *L, Event *e)
{

}

Status ListEmpty(LinkList L)
{

}

/*==================== 链队列操作 ====================*/
Status InitQueue(LinkQueue *Q)
{

}

Status QueueEmpty(LinkQueue *Q)
{

}

int QueueLength(LinkQueue Q)
{

}

Status EnQueue(LinkQueue *Q, QElemType e)
{

}

Status DeQueue(LinkQueue *Q, QElemType *e)
{

}

Status GetHead(LinkQueue Q, QElemType *e)
{

}

/*==================== 全局变量 ====================*/


/*==================== 辅助函数 ====================*/
// 生成随机 逗留时间 和 到达时间间隔
Status Random(int *duT, int *intT)
{

}

// 找到最短队列的窗口编号
int Minimum(LinkQueue q[])
{

}

/*==================== 业务逻辑 ====================*/
// 银行开门初始化
void OpenForDay()
{

}

// 处理顾客到达事件
void CustomerArrived()
{

}

// 处理顾客离开事件
void CustomedDeparture()
{
    
}

// 银行模拟主过程
void Bank_Simulation()
{

}

/*==================== 主函数 ====================*/
int main()
{
    srand((unsigned)time(NULL));

    Bank_Simulation();

    system("pause");
    return 0;
}