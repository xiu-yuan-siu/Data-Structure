#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* ---------- 十字链表结构 ---------- */
#define MAX_VERTEX_NUM 20
typedef char VertexType;

typedef struct ArcBox {                     // 弧结点
    int tailvex, headvex;                   // 该弧的尾和头顶点的下标
    struct ArcBox *hlink, *tlink;           // 分别为弧头(入弧链域)、弧尾(出弧链域)相同的弧的链域
} ArcBox;
typedef struct VexNode {                    // 顶点结点
    VertexType data;
    ArcBox *firstin, *firstout;             // 分别指向该顶点第一条入弧、出弧
} VexNode;
typedef struct {
    VexNode xlist[MAX_VERTEX_NUM];
    int vexnum, arcnum;
} OLGraph;

/* ========== 基本操作 ========== */
int LocateVex(OLGraph G, VertexType v) {
    int i;
    for (i = 0; i < G.vexnum; i++)
        if (G.xlist[i].data == v)
            return i;
    return -1;
}

Status CreateDG(OLGraph *G) {
    int i, j, k;
    VertexType v1, v2;
    ArcBox *p;
    
    printf("请输入有向图的顶点数和弧数: ");
    scanf("%d%d", &G->vexnum, &G->arcnum);
    
    printf("请输入%d个顶点: ", G->vexnum);
    for (i = 0; i < G->vexnum; i++) {
        scanf(" %c", &G->xlist[i].data);
        G->xlist[i].firstin = G->xlist[i].firstout = NULL;
    }
    
    printf("请输入%d条弧(尾 头): \n", G->arcnum);
    for (k = 0; k < G->arcnum; k++) {
        scanf(" %c %c", &v1, &v2);
        i = LocateVex(*G, v1);  /* 弧尾 */
        j = LocateVex(*G, v2);  /* 弧头 */
        
        p = (ArcBox *)malloc(sizeof(ArcBox));
        p->tailvex = i;
        p->headvex = j;
        
        /* 插入到十字链表：链入出弧链表（同tailvex） */
        p->tlink = G->xlist[i].firstout;
        G->xlist[i].firstout = p;
        
        /* 链入入弧链表（同headvex） */
        p->hlink = G->xlist[j].firstin;
        G->xlist[j].firstin = p;
    }
    return OK;
}

/* ---------- 求有向图的强连通分量 -- kosaraju 算法 ---------- */
int count;                                  // finished数组计数器
Boolean visited[MAX_VERTEX_NUM];            // 是否访问
int finished[MAX_VERTEX_NUM];               // 记录顶点退出DFS的顺序

// 第一次DFS(正向DFS)，沿着 firstout->tlink (出弧方向，顺着箭头)
void DFS_1(OLGraph G, int v) {
    visited[v] = TRUE;
    printf("%c ", G.xlist[v].data);
    for (ArcBox *p = G.xlist[v].firstout; p != NULL; p = p->tlink) {
        int w = p->headvex;                 // w是弧头，即 v 的出弧邻接点
        if (!visited[w]) {
            DFS_1(G, w);
        }
    }
    // 所有邻接点搜索完成后，记录顶点v到finished数组()
    finished[++count] = v;
}

// 第二次DFS(逆向DFS)，沿着 firstin->hlink (入弧方向，逆着箭头)
void DFS_2(OLGraph G, int v) {
    visited[v] = TRUE;
    printf("%c ", G.xlist[v].data);         // 输出该强连通分量中的顶点
    for (ArcBox *p = G.xlist[v].firstin; p != NULL; p = p->hlink) {
        int w = p->tailvex;                 // w 是弧尾，即 v 的入弧邻接点
        if (!visited[w]) {
            DFS_2(G, w);
        }
    }
}

// 求有向图的强连通分量
void StronglyConnectedComponents(OLGraph G) {
    count = 0;                              // finished数组计数器初始化为 0
    // 初始化 visited 数组
    for (int i = 0; i < G.vexnum; ++i) 
        visited[i] = FALSE;
    /* ========== 步骤1：正向DFS，记录finished数组 ========== */
    printf("\n===== 步骤1：正向DFS，记录完成顺序 =====\n");
    for (int v = 0; v < G.vexnum; ++v) {
        if (!visited[v]) {
            printf("路径起始点 %c: ", G.xlist[v].data); 
            DFS_1(G, v);
            printf("\n");
        }
    }

    printf("\nfinished数组（完成顺序）: ");
    for (int i = 1; i <= count; i++)            // 从 1 开始
        printf("%c ", G.xlist[finished[i]].data);
    printf("\n");

    // 重置 visited 数组
    for (int i = 0; i < G.vexnum; ++i) 
        visited[i] = FALSE;
    
    /* ========== 步骤2：逆向DFS，求强连通分量 ========== */
    // 从finished数组中最后完成搜索的顶点开始，逆序取顶点
    printf("\n===== 步骤2：逆向DFS，求强连通分量 =====\n");
    int scc_count = 0;
    for (int i = count; i >= 1; --i) {          // count = 4开始
        int v = finished[i];
        if (!visited[v]) {
            ++scc_count;
            printf("强连通分量 %d: { ", scc_count);
            DFS_2(G, v);
            printf("}\n");
        }
    }
    printf("\n该有向图共有 %d 个强连通分量\n", scc_count);
}

int main() {
    OLGraph G;
    CreateDG(&G);
    StronglyConnectedComponents(G);
    // system("pause");
    return 0;
}

/*
========== 输入输出案例(书中所说图7.11)：==========

请输入有向图的顶点数和弧数: 4 7
请输入4个顶点: 1 2 3 4
请输入7条弧(尾 头):
1 2
1 3
3 4
4 2
3 1
4 1
4 3

===== 步骤1：正向DFS，记录完成顺序 =====
路径起始点 1: 1 3 4 2

finished数组（完成顺序）: 2 4 3 1

===== 步骤2：逆向DFS，求强连通分量 =====
强连通分量 1: { 1 4 3 }
强连通分量 2: { 2 }

该有向图共有 2 个强连通分量
请按任意键继续. . .
*/
