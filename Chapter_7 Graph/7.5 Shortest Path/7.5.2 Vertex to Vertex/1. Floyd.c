#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* ---------- 邻接矩阵 ---------- */
#define INFINITY INT_MAX
#define MAX_VERTEX_NUM 20
typedef char VertexType;
typedef int VRType;                     // 顶点关系类型，有向网为权值类型

// 有向网 -- 故不定义图的种类
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];                // 顶点数组
    VRType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];    // 权值邻接矩阵
    int vexnum, arcnum;
} MGraph;

/* ---------------- 辅助函数：创建图 7.36 ---------------- */
void CreateGraph_G7(MGraph *G) {
    int i, j;
    G->vexnum = 3;
    G->arcnum = 5; 

    // 顶点 a, b, c 对应 0, 1, 2
    G->vexs[0] = 'a'; 
    G->vexs[1] = 'b'; 
    G->vexs[2] = 'c';

    // 初始化邻接矩阵
    for (i = 0; i < G->vexnum; ++i)
        for (j = 0; j < G->vexnum; ++j)
            G->arcs[i][j] = INFINITY;
    
    for (i = 0; i < G->vexnum; ++i)
        G->arcs[i][i] = 0;

    // 根据图 7.36(b) 邻接矩阵反推输入边
    // 0  4  11  -> a->b=4, a->c=11
    G->arcs[0][1] = 4;
    G->arcs[0][2] = 11;
    // 6  0  2   -> b->a=6, b->c=2
    G->arcs[1][0] = 6;
    G->arcs[1][2] = 2;
    // 3  ∞  0   -> c->a=3, c->b=∞
    G->arcs[2][0] = 3;
}

/* ---------------- 打印结果 ---------------- */
void PrintResult(MGraph G, PathMatrix P, DistanceMatrix D) {
    int v, w, u;
    printf("\n--- Floyd Result ---\n");
    for (v = 0; v < G.vexnum; ++v) {
        for (w = 0; w < G.vexnum; ++w) {
            if (v == w) continue;
            printf("%c -> %c: Distance = %d, Path vertices: ", G.vexs[v], G.vexs[w], D[v][w]);
            for (u = 0; u < G.vexnum; ++u) {
                if (P[v][w][u]) {
                    printf("%c ", G.vexs[u]);
                }
            }
            printf("\n");
        }
    }
}

/* ---------------- 算法 7.16：Floyd 算法 ---------------- */
// 用 Floyd 算法求有向网 G 中各对顶点 v 和 w 之间的最短路径 P[v][w] 及其带权长度 D[v][w]
typedef int DistanceMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];                 // D[v][w] 表示 v 到 w 的最短路径长度
typedef bool PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM][MAX_VERTEX_NUM];    // P[v][w][u] 为 TRUE 表示 u 是从 v 到 w 当前求得最短路径上的顶点

void ShortestPath_FLOID(MGraph G, PathMatrix *P[], DistanceMatrix *D) {

}

int main() {
    MGraph G;
    PathMatrix P;
    DistanceMatrix D;

    CreateGraph_G7(&G);
    ShortestPath_FLOYD(G, &P, &D);
    PrintResult(G, P, D);

    system("pause");
    return 0;
}