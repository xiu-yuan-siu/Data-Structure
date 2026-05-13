#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef struct 
{
    int x;
    int y;
}PosType;           // 坐标类型

typedef struct
{
    int ord;        // 通道块在路径上的“序号”
    PosType seat;   // 通道块在迷宫的“坐标位置”
    int di;         // 从此通道块走向下一通道块的“方向”(1-4,  东、南、西、北)
}SElemType;         // 栈的元素类型

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

// 迷宫定义 (0:通路, 1:墙, 2:已访问, 3:死胡同)，还原书 P60 图 3.4，起点(1, 1), 终点(8, 8), (x, y), 向下为 x 增加
int maze[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1}, // 1
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1}, 
    {1, 0, 0, 0, 0, 1, 1, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 0, 1, 1, 0, 1}, 
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 1}, // 8
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  // 9
};

// 当前位置是否可通
Status Pass(PosType p)
{
    return maze[p.x][p.y] == 0;
}

// 留下足迹 2(已访问)
void FootPrint(PosType p)
{
    maze[p.x][p.y] = 2;
}

// 标记死胡同 3(死胡同)
void MarkPrint(PosType p)
{
    maze[p.x][p.y] = 3;
}

// 移动位置(1-东, 2-南, 3-西, 4-北)
PosType NextPos(PosType p, int di)
{
    switch(di) 
    {
        case 1:  // 东
            p.y++;
            break;
        case 2:  // 南
            p.x++;
            break;
        case 3:  // 西
            p.y--;
            break;
        case 4:  // 北
            p.x--;
            break;
    }
    return p;
}

void PrintStackPath(SqStack S) {
    printf("\n确定的路径坐标序列：\n");
    SElemType *p = S.base;
    while (p < S.top) {
        printf("(%d,%d) -> ", p->seat.x, p->seat.y);
        p++;
        if ((p - S.base) % 5 == 0) printf("\n"); // 每行打印5个坐标
    }
    printf("END\n");
}

Status MazePath(PosType start, PosType end)
{
    SqStack S;
    InitStack(&S);
    PosType curpos = start;  // 当前位置 current position
    int curstep = 1;         // 当前步数 current step
    // 用 do while 因为第一次栈肯定为空，但又要进入循环进行与之后的curstep相同的找路步骤
    do {
        if (Pass(curpos))  // 当前位置可通(0)
        {
            FootPrint(curpos);  // 留下足迹
            SElemType e = {curstep, curpos, 1};  // 记录走过的点(序号, 坐标, 下一步的方向)
            Push(&S, e);  // 将点入栈
            if (curpos.x == end.x && curpos.y == end.y)  // 到达终点
            {
                PrintStackPath(S);  // 打印坐标序列
                return TRUE;
            }
            // 未到达终点
            curpos = NextPos(curpos, 1);  // 向东走，更新当前位置
            curstep++;  // 更新步数
        }
        else  // 往前位置不可通(1, 2, 3), 要弹栈/出栈
        {
            if (!StackEmpty(S))
            {
                SElemType e;
                Pop(&S, &e);
                while (e.di == 4 && !StackEmpty(S))
                {
                    MarkPrint(e.seat);  // 标记上一个点为死胡同，不能通过
                    Pop(&S, &e);  // 如果当前四个方向都试过了，为死胡同，继续回退
                }
                if (e.di < 4)
                {
                    e.di++;  // 顺时针更换方向(1-东, 2-南, 3-西, 4-北)
                    Push(&S, e);  // 入栈
                    curpos = NextPos(e.seat, e.di);  // 更改当前位置为上一位置另一方向的近邻
                }
            }
        }
    } while (!StackEmpty(S));
    DestroyStack(&S);
    return FALSE;
}

// 辅助打印函数：观察路径搜索后的迷宫状态
void PrintMaze() {
    printf("\n迷宫状态 (2:路径, 3:死胡同, 1:墙):\n");
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (maze[i][j] == 1) printf(" # ");
            else if (maze[i][j] == 2) printf(" . ");
            else if (maze[i][j] == 3) printf(" X ");
            else printf("   ");
        }
        printf("\n");
    }
}

int main() {
    PosType start = {1, 1};
    PosType end = {8, 8};

    printf("--- 测试 1: 正常寻径 ---");
    if (MazePath(start, end) == OK) {
        printf("\n成功找到路径！");
        PrintMaze();
    } else {
        printf("\n未找到路径。");
    }

    // 重置迷宫进行边界测试
    printf("\n--- 测试 2: 起点即终点 ---");
    PosType same = {1, 1};
    if (MazePath(same, same) == OK) printf("\n边界测试通过：起点即终点处理正确。");

    printf("\n--- 测试 3: 无解情况 (封死 8,8) ---");
    maze[8][7] = 1; maze[7][8] = 1; // 人为堵死
    if (MazePath(start, end) == FALSE) {
        printf("\n测试通过：算法正确识别无解迷宫。\n");
    }
    // system("pause");
    return 0;
}