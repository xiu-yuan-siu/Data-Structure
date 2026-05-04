#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* ---------- 邻接表结构 ---------- */
#define MAX_VERTEX_NUM 20
typedef char VertexType;

typedef struct ArcNode {                            // 弧结点
    int adjvex;                                     // 该弧弧头在数组中的下标
    struct ArcNode *nextarc;
} ArcNode;
typedef struct VNode {                              // 顶点结点
    VertexType data;
    ArcNode *firstarc;
} VNode, AdjList[MAX_VERTEX_NUM];
typedef struct {
    AdjList vertices;
    int vexnum, arcnum;
} ALGraph;

// ================== 全局变量 ==================
int count;                              // 用于给访问次序编号
int visited[MAX_VERTEX_NUM];            // 记录顶点的 DFS 访问次序
int low[MAX_VERTEX_NUM];                // 记录顶点能回退到的最小次序

// 尾插法添加单向边
void AddEdge(ALGraph *G, int u, int v) {
    ArcNode *arc = (ArcNode*)malloc(sizeof(ArcNode));
    arc->adjvex = v;
    arc->nextarc = NULL;
    if (G->vertices[u].firstarc == NULL) {
        G->vertices[u].firstarc = arc;
    } else {
        ArcNode *p = G->vertices[u].firstarc;
        while (p->nextarc != NULL) {
            p = p->nextarc;
        }
        p->nextarc = arc;
    }
}

// 构造书中图 7.19 的无向图 G5
void CreateGraphG5(ALGraph *G) {
    G->vexnum = 13;
    G->arcnum = 17; // 实际双向边数量，这里暂不作严格约束
    
    // 初始化顶点 0:A, 1:B, 2:C, 3:D, 4:E, 5:F, 6:G, 7:H, 8:I, 9:J, 10:K, 11:L, 12:M
    char nodes[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'};
    for (int i = 0; i < G->vexnum; i++) {
        G->vertices[i].data = nodes[i];
        G->vertices[i].firstarc = NULL;
    }

    // 严格按顺序构造邻接表，以完美复现教材 Page 178 的 DFS 访问次序
    AddEdge(G, 0, 11); AddEdge(G, 0, 1);  AddEdge(G, 0, 2);  AddEdge(G, 0, 5);  // A -> L, B, C, F
    AddEdge(G, 1, 7);  AddEdge(G, 1, 3);  AddEdge(G, 1, 2);  AddEdge(G, 1, 6); AddEdge(G, 1, 9); AddEdge(G, 1, 0); // B -> H, D, C, G, J, A
    AddEdge(G, 2, 0);  AddEdge(G, 2, 1);  // C -> A, B
    AddEdge(G, 3, 4);  AddEdge(G, 3, 1);  // D -> E, B
    AddEdge(G, 4, 3);                     // E -> D
    AddEdge(G, 5, 0);                     // F -> A
    AddEdge(G, 6, 8);  AddEdge(G, 6, 1);  AddEdge(G, 6, 10); // G -> I, B, K
    AddEdge(G, 7, 10); AddEdge(G, 7, 1);  // H -> K, B
    AddEdge(G, 8, 6);                     // I -> G
    AddEdge(G, 9, 1);  AddEdge(G, 9, 12); // J -> B, M
    AddEdge(G, 10, 6); AddEdge(G, 10, 7); // K -> G, H
    AddEdge(G, 11, 12); AddEdge(G, 11, 0);// L -> M, A
    AddEdge(G, 12, 9);  AddEdge(G, 12, 11);// M -> J, L
}

// 算法 7.11：从第 v0 个顶点出发深度优先遍历，查找并输出关节点
void DFSArticul(ALGraph G, int v0) {
    int min, w;
    ArcNode *p;

    visited[v0] = min = ++count;                            // v0 是第 count(从2开始) 个访问的结点
    for (p = G.vertices[v0].firstarc; p != NULL; p = p->nextarc) {                      // 对 v0 的每个邻接顶点检查
        w = p->adjvex;                                      // w 是 v0 的邻接顶点(孩子)
        if (visited[w] == 0) {                              // w 未曾访问，是 v0 的孩子，通过树边更新 low[v]
            DFSArticul(G, w);
            if (low[w] < min) {
                min = low[w];
            }
            if (low[w] >= visited[v0]) {
                printf("%c ", G.vertices[v0].data);         // 输出关节点
            }
        } else if (visited[w] < min) {                      // 通过回边更新 low[v]
            min = visited[w];                               // w 已访问，w 是 v0 在生成树上的祖先
        }
    }
    low[v0] = min;
}

// 算法 7.10：连通图找关节点的主控函数 -- O(n+e)
void FindArticul(ALGraph G) {
    count = 1;
    // 初始化 visited 数组
    visited[0] = 1;                     // 设定 0 号顶点(A)为生成树的根
    low[0] = 1;
    for (int i = 1; i < G.vexnum; ++i) {
        visited[i] = 0;                 // 其余节点尚未访问
    }
    ArcNode *p = G.vertices[0].firstarc;
    int v = p->adjvex;
    DFSArticul(G, v);                   // 从根结点(A)的邻接节点 v 开始深度优先查找关节点
    if (count < G.vexnum) {             // 第一棵子树遍历完 count < 顶点数，说明根结点有多颗子树，则根结点是关节点
        printf("%c ", G.vertices[0].data);
        while (p->nextarc != NULL) {
            p = p->nextarc;
            v = p->adjvex;
            if (visited[v] == 0)
                DFSArticul(G, v);
        }
    }
}

// ================== 测试主程序 ==================
int main() {
    ALGraph G;
    printf("========== 关节点查找算法 ==========\n");
    CreateGraphG5(&G);
    
    printf("图 G5 的关节点为: ");
    FindArticul(G);
    printf("\n");
    
    printf("\n--- 验证 visited 与 low 数组 ---\n");
    printf("顶点\tvisited\tlow\n");
    for(int i = 0; i < G.vexnum; i++) {
        printf("%c\t%d\t%d\n", G.vertices[i].data, visited[i], low[i]);
    }
    system("pause");
    return 0;
}

/*
严蔚敏书中 J       4       1 应该是 J       4       2，出现 2 的答案是因为有些教材（包括 Tarjan 原始论文）在计算 low 时有意忽略父结点，只考虑非父亲的回边
*/