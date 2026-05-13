#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* ---------- 邻接矩阵 ---------- */
#define INFINITY 65535
#define MAX_VERTEX_NUM 20
typedef char VertexType;
typedef int VRType;                     // 顶点关系类型，有向网为权值类型

typedef Boolean PathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];     
typedef int ShortPathTable[MAX_VERTEX_NUM];

// 有向网 -- 故不定义图的种类
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];                // 顶点数组
    VRType arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];    // 权值邻接矩阵
    int vexnum, arcnum;
} MGraph;

/* ---------------- 辅助函数：创建图 7.34 ---------------- */
void CreateGraph_G6(MGraph *G) {
    int i, j;
    G->vexnum = 6;
    G->arcnum = 8; // 数一下图里的边
    // 顶点 0-5 代表 V0-V5
    for (i = 0; i < G->vexnum; ++i) 
        G->vexs[i] = '0' + i;               // 方便打印

    // 初始化邻接矩阵
    for (i = 0; i < G->vexnum; ++i)
        for (j = 0; j < G->vexnum; ++j)
            G->arcs[i][j] = INFINITY;
    
    for (i = 0; i < G->vexnum; ++i)
        G->arcs[i][i] = 0;

    // 根据图 7.34 输入边 <Vi, Vj, weight>
    // V0->V2 (10), V0->V4 (30), V0->V5 (100)
    G->arcs[0][2] = 10; 
    G->arcs[0][4] = 30; 
    G->arcs[0][5] = 100;
    // V1->V2 (5)
    G->arcs[1][2] = 5;
    // V2->V3 (50)
    G->arcs[2][3] = 50;
    // V3->V5 (10)
    G->arcs[3][5] = 10; 
    // V4->V5 (60)
    G->arcs[4][5] = 60;
    // V4->V3 (20)
    G->arcs[4][3] = 20;
}

/* ---------------- 辅助递归函数：回溯打印路径 ---------------- */
// 利用 D 数组和邻接矩阵反推前驱，实现有序输出，如果图中存在多条长度相同的最短路径，该函数只会输出找到的第一条
void PrintPath(MGraph G, int v0, int current, PathMatrix P, ShortPathTable D) {
    // 1. 递归终止：回溯到起点
    if (current == v0) {
        printf("v%d", v0);
        return;
    }

    // 2. 寻找 current 的直接前驱 pre
    // 条件：
    // (1) pre 在当前路径集合 P[current] 中
    // (2) pre 不是 current 自己
    // (3) pre 到 current 有边 (防止 INFINITY 干扰计算)
    // (4) 核心逻辑：D[pre] + weight(pre->current) == D[current]
    for (int pre = 0; pre < G.vexnum; ++pre) {
        if (P[current][pre] && pre != current && 
            G.arcs[pre][current] != INFINITY && 
            D[pre] + G.arcs[pre][current] == D[current]) {
            
            // 找到前驱，先递归打印前驱的路径
            PrintPath(G, v0, pre, P, D);
            // 回溯返回后，打印当前顶点
            printf(" -> v%d", current);
            return; // 找到唯一前驱即可返回（假设最短路径唯一）
        }
    }
    // 若未找到前驱但 current != v0，说明路径不连续或数据有误，保底输出
    printf("%d", current);
}

/* ---------------- 打印结果主函数 ---------------- */
void PrintResult(MGraph G, int v0, PathMatrix P, ShortPathTable D) {
    int v;
    printf("\n--- Dijkstra Result (Start V%d) ---\n", v0);
    for (v = 0; v < G.vexnum; ++v) {
        if (v == v0) continue;
        
        printf("V%d -> V%d: Distance = %d, Path: ", v0, v, D[v]);
        
        if (D[v] == INFINITY) {
            printf("No Path");
        } else {
            // 调用回溯函数打印有序路径
            PrintPath(G, v0, v, P, D);
        }
        printf("\n");
    }
}

/* ---------------- 算法 7.15：Dijkstra 算法 ---------------- */
// Dijkstra算法，求有向网G的v0顶点到其余顶点v的最短路径P[v]及其带权长度D[v] -- 时间复杂度 O(n^3)(实际实现)，空间复杂度 O(n^2)                     
void ShortestPath_DIJ(MGraph G, int v0, PathMatrix *P, ShortPathTable *D) {
    // P[v][w] 为 TRUE 表示 w 是从 v0 到 v 当前求得的最短路径上的顶点
    // D[v] 表示 v0 到 v 的最短路径长度(动态更新)
    // final[v]为 TRUE 当且仅当 v∈S，即已经求得从 v0 到 w 的最短路径
    Boolean final[MAX_VERTEX_NUM];      
    // 1. 初始化
    for (int v = 0; v < G.vexnum; ++v) {
        final[v] = FALSE;
        (*D)[v] = G.arcs[v0][v];        // D[v] 初始化为 v0 直接到 v 的权值
        for (int w = 0; w < G.vexnum; ++w) {
            (*P)[v][w] = FALSE;         // 设空路径
        }
        // 若 v0 到 v 有直接路径，则路径上包含 v0 和 v
        if ((*D)[v] < INFINITY) {
            (*P)[v][v0] = TRUE;
            (*P)[v][v] = TRUE;
        }
    }
    // 初始化v0
    (*D)[v0] = 0;                       // v0 到 v0 自身最短距离为 0
    final[v0] = TRUE;                   // 初始化，v0 顶点属于 S 集
    // 2. 主循环：每次求得 v0 到某个 v 的最短路径，并把 v 加到 S 集 -- O(n^3)
    for (int i = 1; i < G.vexnum; ++i) {    // 其余 G.vexnum-1 个顶点，故 i 从 1 开始
        int min = INFINITY;                 // 当前所知离 v0 顶点的最近距离
        int v = -1;                         // 当前离 v0 最近的点
        // 寻找 V-S 中距离 v0 最近的顶点
        for (int w = 0; w < G.vexnum; ++w) {
            if (!final[w]) {                // final[w] = FALSE, 即 w 顶点不在 S 中，在 V-S 中
                if ((*D)[w] < min) {        // 当前的 w 顶点离 v0 顶点更近
                    v = w;
                    min = (*D)[w];
                }
            }
        }
        if (v == -1) break;                 // 无可达顶点时退出，防止溢出
        final[v] = TRUE;                    // 把当前离 v0 最近的点 v 加入 S 集
        // 更新当前最短路径和距离
        for (int w = 0; w < G.vexnum; ++w) {        
            // 注意：这里应该判断 arcs[v][w] != INFINITY 防止溢出
            // 如果 w∈V-S 中，且 v0 通过 v 到达 w 比直接到达 w 更近
            if (!final[w] && G.arcs[v][w] != INFINITY && (min + G.arcs[v][w] < (*D)[w])) {      
                // 更新 D[w]
                (*D)[w] = min + G.arcs[v][w];                       
                // 更新 P[w]，即 w 的路径 = v 的路径 + w
                for (int j = 0; j < G.vexnum; ++j) {                // 该循环相当于书中 P[w] = P[v], 这里还有一个 O(n)
                    (*P)[w][j] = (*P)[v][j];                        // w 的路径 = v 的路径
                }
                (*P)[w][w] = TRUE;                                  // w 的路径 = v 的路径 + w
            }
        }
    }
}
int main() {
    MGraph G;
    PathMatrix P;
    ShortPathTable D;
    int v0 = 0; // 从 V0 出发

    CreateGraph_G6(&G);
    ShortestPath_DIJ(G, v0, &P, &D);
    PrintResult(G, v0, P, D);
    
    // system("pause");
    return 0;
}