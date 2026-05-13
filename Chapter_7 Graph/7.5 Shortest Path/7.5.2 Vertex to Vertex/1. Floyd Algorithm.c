#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* ---------- 邻接矩阵 ---------- */
#define INFINITY INT_MAX
#define MAX_VERTEX_NUM 20
typedef char VertexType;
typedef int VRType;                     // 顶点关系类型，有向网为权值类型

typedef int DistanceMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];                 
typedef Boolean PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM][MAX_VERTEX_NUM];    

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

// 最短路径唯一时只打印一条
/* --- 按距离矩阵贪心重建路径顺序 --- */
void PrintPathInOrder(MGraph G, DistanceMatrix D, PathMatrix P, int v, int w) {
    printf("%c ", G.vexs[v]);
    if (v == w) return;
    
    int cur = v;
    Boolean visited[MAX_VERTEX_NUM] = {FALSE};
    visited[v] = TRUE;
    
    while (cur != w) {
        int next = -1;
        int minDist = INFINITY;  // 选距离 cur 最近的中间点
        
        for (int u = 0; u < G.vexnum; ++u) {
            // 关键修改1: 用子问题三角等式 + 排除终点
            if (P[v][w][u] && !visited[u] && u != w &&
                D[cur][u] != INFINITY && D[u][w] != INFINITY &&
                D[cur][w] == D[cur][u] + D[u][w]) {  // ✅ 子问题：cur->u->w 是最短的一部分
                // 关键修改2: 有多个候选时，选离 cur 最近的（贪心策略）
                if (D[cur][u] < minDist) {
                    minDist = D[cur][u];
                    next = u;
                }
            }
        }
        // 没找到中间点，说明 cur 直达 w
        if (next == -1) next = w;
        
        printf("%c ", G.vexs[next]);
        visited[next] = TRUE;
        cur = next;
    }
}

void PrintResult(MGraph G, PathMatrix P, DistanceMatrix D) {
    printf("\n--- Floyd Result ---\n");
    for (int v = 0; v < G.vexnum; ++v) {
        for (int w = 0; w < G.vexnum; ++w) {
            if (v == w || D[v][w] == INFINITY) continue;
            
            printf("%c -> %c: Distance = %d, Path: ", G.vexs[v], G.vexs[w], D[v][w]);
            PrintPathInOrder(G, D, P, v, w);
            printf("\n");
        }
    }
}

/* ---------------- 算法 7.16：Floyd 算法 ---------------- */
// 用 Floyd 算法求有向网 G 中各对顶点 v 和 w 之间的最短路径 P[v][w] 及其带权长度 D[v][w] -- 时间复杂度 O(n^3)
void ShortestPath_FLOYD(MGraph G, PathMatrix *P, DistanceMatrix *D) {
    // P[v][w][u] 为 TRUE 表示 u 是从 v 到 w 当前求得最短路径上的顶点
    // D[v][w] 表示 v 到 w 的最短路径长度
    // 1. 初始化 -- 各对结点之间初始已知路径及距离
    for (int v = 0; v < G.vexnum; ++v) {
        for (int w = 0; w < G.vexnum; ++w) {
            (*D)[v][w] = G.arcs[v][w];                  // 初始化距离
            for (int u = 0; u < G.vexnum; ++u) {
                (*P)[v][w][u] = FALSE;
            }
            if ((*D)[v][w] < INFINITY) {                // 从 v 到 w 有直达路径，初始化已知路径(v和w在从v到w的路径上)
                (*P)[v][w][v] = TRUE;
                (*P)[v][w][w] = TRUE;
            }
        }
    }
    // 2. 主循环
    // u 代表允许作为中间顶点的序号，从 0 到 n-1
    for (int u = 0; u < G.vexnum; ++u) {
        for (int v = 0; v < G.vexnum; ++v) {
            for (int w = 0; w < G.vexnum; ++w) {
                if ((*D)[v][u] + (*D)[u][w] < (*D)[v][w]) {         // 经过 u 的路径比 v 到 w 直达的短
                    // 更新距离
                    (*D)[v][w] = (*D)[v][u] + (*D)[u][w];
                    // 更新路径集合 P[v][w]
                    // P[v][w] = P[v][u] ∪ P[u][w]
                    for (int i = 0; i < G.vexnum; ++i) {
                        (*P)[v][w][i] = (*P)[v][u][i] || (*P)[u][w][i];
                    }
                }
            }
        }
    }
}

int main() {
    MGraph G;
    PathMatrix P;
    DistanceMatrix D;

    CreateGraph_G7(&G);
    ShortestPath_FLOYD(G, &P, &D);
    PrintResult(G, P, D);

    // system("pause");
    return 0;
}