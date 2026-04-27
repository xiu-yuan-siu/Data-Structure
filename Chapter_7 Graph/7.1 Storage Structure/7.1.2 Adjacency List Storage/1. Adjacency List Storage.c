#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/*
* 无向图 n 个顶点，e 条边 -- 邻接表需 n 个头结点，2e 个边结点（无向图边没有方向），度即结点数
* 有向图                --        n           e                          出度即结点数，入度必须遍历邻接表求
* 建立邻接表或逆邻接表时，若输入的顶点信息即为顶点的编号（下标），则时间复杂度为 O(n + e)
*                                    不为               则要先查找顶点在图中的位置，时间复杂度为 O(n * e)
*/

#define MAX_VERTEX_NUM 20

typedef enum { DG, DN, UDG, UDN };  // 图种类
typedef int InfoType;               // 弧相关信息（权值等）
typedef char VextexType;            // 顶点数据类型

// 弧节点（链表）
typedef struct ArcNode {        
    int adjvex;                 // 该弧所指向的顶点的位置
    struct ArcNode *nextarc;    // 指向下一条弧的指针
    InfoType *info;             // 该弧相关信息的指针
} ArcNode;

// 头节点 -- 顶点结点（数组）
typedef struct VNode {
    VextexType data;            // 顶点信息
    ArcNode *firstarc;          // 指向第一条依附该顶点的弧的指针
} VNode, AdjList[MAX_VERTEX_NUM];

// 图结构
typedef struct {
    AdjList vertices;           // 顶点数组
    int vexnum, arcnum;         // 图的当前顶点数和弧数
    int kind;                   // 图的种类标志
} ALGraph;
