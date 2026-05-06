#include <stdio.h>
#include <stdlib.h>
#include "../../../Chapter_3 Stack and Queue/3.1. Stack/1. Sequential Stack/1.1 Sequential Stack.h"
#include "../../../Status.h"

/* ---------- 邻接表结构 ---------- */
#define MAX_VERTEX_NUM 20
typedef char VertexType;

typedef struct ArcNode {
    int adjvex;
    struct ArcNode *nextarc;
} ArcNode;
typedef struct VNode {
    VertexType data;
    ArcNode *firstarc;
    int indegree;                       // 增加的 顶点的入度域
} VNode, AdjList[MAX_VERTEX_NUM];
typedef struct {
    AdjList vertices;
    int vexnum, arcnum;
} ALGraph;

// 求各顶点入度
void FindInDegree(ALGraph G) {
    // 初始化入度数组
    for (int i = 0; i < G.vexnum; ++i) G.vertices[i].indegree = 0;
    for (int i = 0; i < G.vexnum; ++i) {
        ArcNode *p = G.vertices[i].firstarc;
        while (p) {
            ++(G.vertices[p->adjvex].indegree);
            p = p->nextarc;
        }
    }
}

// 拓扑排序(时间复杂度O(n+e)) -- 若 G 无回路，则输出 G 的顶点的一个拓扑序列并返回 OK，否则返回 ERROR
Status TopologicalSort(ALGraph G) {
    int i, k, count;
    SqStack S;
    ArcNode *p;

    FindInDegree(G);                  // 对个顶点求入度 base=0
    InitStack(&S);
    for (i = 0; i < G.vexnum; ++i)                          // 建零入度顶点栈 -- O(n)
        if (G.vertices[i].indegree == 0) Push(&S, i);       // 入度为 0 的顶点进栈(注意这里 i 和 SElemType 刚好为 int)
    count = 0;                                              // 计数已输出的顶点数
    while (!StackEmpty(S)) {                                // O(e)
        Pop(&S, i);                             // 栈顶元素出栈
        printf("%c ", G.vertices[i].data);      // 输出第 i 个顶点
        ++count;                                // 输出顶点计数
        for (p = G.vertices[i].firstarc; p; p = p->nextarc) {
            k = p->adjvex;
            --(G.vertices[k].indegree);                      // 第 i 个顶点的每个邻接点入度减 1
            if (!(G.vertices[k].indegree)) Push(&S, k);      // 减 1 后入度为 0 则进栈
        }
    }
    if (count < G.vexnum) return ERROR;         // 有环返回错误
    else return OK;
}

// 手动实现栈的拓扑排序，不借助"1.1 Sequential Stack.h"，更加灵活方便
Status TopologicalSort_2(ALGraph G) {
    int i, k, count;
    ArcNode *p;
    int S[MAX_VERTEX_NUM];                  // 使用栈暂存入度为 0 的顶点，这里的 栈类型可以改
    int top = -1;

    FindInDegree(G);
    for (i = 0; i < G.vexnum; ++i) {
        if (G.vertices[i].indegree == 0) 
            S[++top] = i;                   // 入度为 0 进栈(先自增，再进栈)
    }
    count = 0;
    while (top != -1) {
        i = S[top--];                       // 栈顶元素出栈(先出栈，再自减)
        printf("%c ", G.vertices[i].data);  // 输出第 i 个顶点对应的数据
        ++count;
        for (p = G.vertices[i].firstarc; p; p = p->nextarc) {
            k = p->adjvex;
            --(G.vertices[k].indegree);                  // i 的所有邻接顶点入度减 1
            if (G.vertices[k].indegree == 0) S[++top] = k;
        }
    }
    if (count < G.vexnum) return ERROR;
    else return OK;
}