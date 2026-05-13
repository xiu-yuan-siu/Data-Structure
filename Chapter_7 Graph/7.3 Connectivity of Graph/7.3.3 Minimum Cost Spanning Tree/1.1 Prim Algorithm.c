#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

// 辅助数组：用于记录从 U 到 V-U 的最小代价边
struct {
    VertexType adjvex;              // 最小代价边依附的在 U 中的顶点
    int lowcost;                    // 最小代价边的权
} closedge[MAX_VERTEX_NUM];

// 定位顶点下标
int LocateVex(MGraph G, VertexType u) {
    for (int v = 0; v < G.vexnum; ++v) 
        if (G.vexs[v] == u) return v;
    return -1;
}

// 寻找 lowcost 中最小的顶点(且不在 U 中)
int minmum(MGraph G) {
    int min = INFINITY;
    int index = -1;
    for (int i = 0; i < G.vexnum; ++i) {
        if (closedge[i].lowcost > 0 && closedge[i].lowcost < min) {     // closedge[i].lowcost=0 意味着 i 对应的顶点已并入集合 U 中
            min = closedge[i].lowcost;
            index = i;
        }
    }
    return index;
}

// prim 算法求最小生成树 -- 时间复杂度 O(n^2)
void MiniSpanTree_PRIM(MGraph G, VertexType u) {
    // 从顶点 u 出发构造网 G 的最小生成树
    int k = LocateVex(G, u);
    // 初始化 closedge 数组
    for (int j = 0; j < G.vexnum; ++j) {
        if (j != k) {
            closedge[j].adjvex = u;
            closedge[j].lowcost = G.arcs[k][j];
        }
    }
    closedge[k].lowcost = 0;                // k 对应的 lowcost=0 即 k 对应的 u 已并入 U 中，即 U = {u}
    printf(">>> Prim 算法执行步骤 <<<\n");
    printf("Step 0: U = {v%c}, V-U = {全部其他}\n", u);
    for (int i = 1; i < G.vexnum; ++i) {    // 对于剩下的 G.vexnum-1 个顶点
        k = minmum(G);                      // 找到离当前 U 最近的顶点(最小代价边)
        // 此时 closedge[k].lowcost = MIN{closedge[v_i].lowcost && closedge[v_i].lowcost > 0, v_i ∈ V-U}
        printf("Step %d: 选择边 (v%c, v%c) 权值: %d\n", i, closedge[k].adjvex, G.vexs[k], closedge[k].lowcost);
        closedge[k].lowcost = 0;            // 第 k 个顶点并入 U 
        // 第 k 个顶点并入 U 后，更新辅助数组，重新选择最小边
        for (int j = 0; j < G.vexnum; ++j) {
            if (G.arcs[k][j] < closedge[j].lowcost && closedge[j].lowcost > 0) {
                closedge[j].adjvex = G.vexs[k];
                closedge[j].lowcost = G.arcs[k][j];
            }
        }
        // 输出当前 U 集状态
        printf("   当前 U = { ");
        for (int m = 0; m < G.vexnum; ++m) {
            if (closedge[m].lowcost == 0) {
                printf("v%c ", G.vexs[m]);
            } 
        }
        printf("}\n");
    }
}

int main() {
    MGraph G;
    CreateGraph716(&G);  
    MiniSpanTree_PRIM(G, '1');
    // system("pause");
    return 0;
}