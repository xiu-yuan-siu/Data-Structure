#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* ---------- 邻接表结构(无向图) ---------- */
#define MAX_VERTEX_NUM 20
typedef char VertexType;

typedef struct ArcNode {        // 弧结点
    int adjvex;
    struct ArcNode *nextarc;
} ArcNode;
typedef struct VNode {          // 顶点结点
    VertexType data;
    ArcNode *firstarc;
} VNode, AdjList[MAX_VERTEX_NUM];
typedef struct {                // 邻接表 -- Adjacency List
    AdjList vertices;
    int vexnum, arcnum;
} ALGraph;

/* ---------- 孩子兄弟链表(二叉链表)结构 ---------- */
typedef char ElemType;
typedef struct CSNode {
    ElemType data;
    struct CSNode *lchild, *nextsibling;
} CSNode, *CSTree;

/* ---------- 深度优先生成树和非连通图的生成森林实现 ---------- */
// 全局变量：用于算法7.7和7.8
Boolean visited[MAX_VERTEX_NUM];    // 是否访问

// 构建深度优先生成树 -- 算法 7.8
void DFSTree(ALGraph G, int v, CSTree *T) {
    // 从第 v 个顶点出发深度优先遍历图 G，建立以 T 为根的生成树
    visited[v] = TRUE;
    Boolean first = TRUE;
    CSTree q = NULL;                            // 向最新创建的孩子结点（后续兄弟结点会挂到它的 nextsibling 上），而不是“兄弟结点”本身
    for (ArcNode *w = G.vertices[v].firstarc; w != NULL; w = w->nextarc) {
        if (!visited[w->adjvex]) {              // v 的邻接点为未被访问
            CSTree p = (CSTree)malloc(sizeof(CSNode));
            *p = (CSNode){G.vertices[w->adjvex].data, NULL, NULL};
            if (first) {                        // w 是 v 第一个未被访问的邻接顶点，则是根的左孩子
                (*T)->lchild = p;
                first = FALSE;
            } else {                            // w 是 v 的其他未被访问的邻接顶点，则是上一邻接顶点的右兄弟结点
                q->nextsibling = p;
            }
            q = p;                              // 永远指向新创建的孩子结点
            DFSTree(G, w->adjvex, &q);          // 递归访问子树
        }
    }
}

// 非连通图的生成森林 -- 算法 7.7 -- 时间复杂度 O(n+e)
void DFSForest(ALGraph G, CSTree *T) {
    // 遍历图中的所有顶点，如果发现顶点未访问，则说明发现了一个新的连通分量，并从该点开始生长出一棵生成树
    *T = NULL;
    CSTree q = NULL;                            // 指向当前生成树的根
    for (int v = 0; v < G.vexnum; ++v) {
        visited[v] = FALSE;
    }
    for (int v = 0; v < G.vexnum; ++v) {
        if (!visited[v]) {                      // 顶点 v 未被访问，则是新的生成树的根结点
            CSTree p = (CSTree)malloc(sizeof(CSNode));
            *p = (CSNode){G.vertices[v].data, NULL, NULL};
            if (!(*T)) *T = p;                  // *T 若为空，则顶点 v 对应的CSNode* p 是 *T 的第一棵生成树的根
            else q->nextsibling = p;            // *T 不为空，则 p 为上一棵生成树的右兄弟
            q = p;                              // q 永远指向当前生成树的根
            DFSTree(G, v, &p);                  // 根据根结点 p 建生成树
        }
    }
}