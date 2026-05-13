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

// 边结构(Kruskal算法核心)
typedef struct {
    int begin;
    int end;
    int weight;
} Edge;

// 并查集 -- 高效判断"加入这条边是否会形成环"
int Find(int parent[], int f) {
    int root = f;
    while (parent[root] > 0) 
        root = parent[root];      // 沿父指针一直向上找到根（parent[root]==0）        
    // 路径压缩：将查找路径上所有结点直接挂到根下
    while (f != root) {
        int next = parent[f];
        parent[f] = root;
        f = next;
    }
    return root;
}

// 选择排序(O(e^2)) -- 求最小生成树升序排列，反之，求最大生成树则需要降序排列
/*
* 选择排序是不稳定排序——当某个位置遇到更小的元素时会发生交换，这可能会打乱相等元素的相对先后
* 排序后，这三条权值 5 的边实际出现的次序变成了 v3-v4 → v2-v3 → v1-v4（或其他排列），而你的输出恰好碰上了先处理 v3-v4 的情况
*/
void SortEdge(Edge edges[], int arcnum) {
    for (int i = 0; i < arcnum - 1; ++i) {
        for (int j = i + 1; j < arcnum; ++j) {
            if (edges[i].weight > edges[j].weight) {
                Edge temp = edges[i];
                edges[i] = edges[j];
                edges[j] = temp;
            }
        }
    }
}

// 冒泡排序(O(e^2)) -- 稳定排序，带有提前终止优化
void SortEdge_Bubble(Edge edges[], int arcnum) {
    for (int i = 0; i < arcnum - 1; ++i) {
        int swapped = 0;                // 是否交换的标志，用于已经好后提前退出外层循环
        for (int j = 0; j < arcnum - 1 - i; ++j) {          // 冒泡排序核心：j 的取值范围
            if (edges[j].weight > edges[j + 1].weight) {        // 相邻比较
                Edge temp = edges[j];
                edges[j] = edges[j + 1]; 
                edges[j + 1] = temp;
                swapped = 1;            // 标记发生交换
            }
        }
        if (swapped == 0) break;        // 没有交换 → 已有序，提前退出
    }
}

// Kruskal 算法实现
void MiniSpanTree_KRUSKAL(MGraph G) {
    Edge edges[MAX_VERTEX_NUM * (MAX_VERTEX_NUM - 1) / 2];    // 无向图边数最多为 V*(V-1)/2，定义足够大的数组
    int k = 0;
    // 初始化 edges 数组
    for (int i = 0; i < G.vexnum; ++i) {
        for (int j = i + 1; j < G.vexnum; ++j) {
            if (G.arcs[i][j] != INT_MAX) {
                edges[k].begin = i;
                edges[k].end = j;
                edges[k].weight = G.arcs[i][j];
                ++k;
            }
        }
    }
    // 升序排列edges数组
    SortEdge_Bubble(edges, k);                         // 按实际边的数目 k 排序
    printf(">>> Kruskal 算法执行步骤 <<<\n");
    int count = 0;                              // 用于计数，当加入 G.vexnum - 1 条边时退出循环
    int parent[MAX_VERTEX_NUM] = {0};           // 列表初始化所有元素为 0(表示每一个元素是一个集合，即连通分量)，不为 0 时parent[]的值表示其父节点
    for (int i = 0; i < k; ++i) {               // 遍历 edges 中的 k 条边，直至选出 G.vexnum - 1 条边
        int n = Find(parent, edges[i].begin);
        int m = Find(parent, edges[i].end);
        if (n != m) {                           // 不构成回路，则加入此条边
            printf("选择边: (v%c, v%c) 权值: %d [OK]\n", G.vexs[edges[i].begin], G.vexs[edges[i].end], edges[i].weight);
            ++count;
            parent[n] = m;                      // 合并两个集合
        } else {                                // 构成回路，则舍弃此条边
            printf("舍弃边: (v%c, v%c) 权值: %d [发现回路!]\n", G.vexs[edges[i].begin], G.vexs[edges[i].end], edges[i].weight);
        }
        if (count == G.vexnum - 1) break;
    }
}

int main() {
    MGraph G;
    CreateGraph716(&G);
    MiniSpanTree_KRUSKAL(G);
    // system("pause");
    return 0;
}