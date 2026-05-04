#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define MAX_VERTEX_NUM 20
#define INFINITY INT_MAX            // 用整型最大值表示弧不存在
typedef char VertexType;

/* ---------- 邻接矩阵 ---------- */
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];                 // 顶点向量
    int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];        // 邻接矩阵表示边
    int vexnum, arcnum;
} MGraph;

// 构造书中的图 7.16
void CreateGraph716(MGraph *G) {
    G->vexnum = 6;
    G->arcnum = 10;
    char names[] = {'1', '2', '3', '4', '5', '6'}; // 对应v1-v6
    for (int i = 0; i < 6; i++) G->vexs[i] = names[i];

    // 初始化矩阵
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            G->arcs[i][j] = (i == j) ? 0 : INT_MAX;

    // 添加边 (书中图7.16的数据)
    int edges[10][3] = {
        {0,1,6}, {0,2,1}, {0,3,5}, // v1-v2, v1-v3, v1-v4
        {1,2,5}, {1,4,3},          // v2-v3, v2-v5
        {2,3,5}, {2,4,6}, {2,5,4}, // v3-v4, v3-v5, v3-v6
        {3,5,2},                   // v4-v6
        {4,5,6}                    // v5-v6
    };
    for (int i = 0; i < 10; i++) {
        int u = edges[i][0], v = edges[i][1], w = edges[i][2];
        G->arcs[u][v] = G->arcs[v][u] = w;
    }
}

