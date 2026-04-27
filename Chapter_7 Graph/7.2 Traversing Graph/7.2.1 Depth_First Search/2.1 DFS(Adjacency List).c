#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../../Status.h"

/* ---------------- 邻接表存储表示的DFS(以书中无向图 G4 为测试案例，故而只写无向图创建的代码) ---------------- */
#define MAX_VERTEX_NUM 20
// typedef int InfoType;
typedef char VertexType;

typedef struct ArcNode {            // 弧结点
    int adjvex;                     // 该弧所指向的顶点在数组中的下标
    struct ArcNode *nextarc;        // 指向下一条弧的指针
    // InfoType *info;                 // 弧相关信息指针，此处略
} ArcNode;

typedef struct VNode {              // 顶点结点
    VertexType data;
    ArcNode *firstarc;              // 指向第一条依附该顶点的指针
} VNode, AdjList[MAX_VERTEX_NUM];

typedef struct {
    AdjList vertices;               // 顶点结点数组
    int vexnum, arcnum;
    int kind;
} ALGraph;                          // Adjacency List

// 查找顶点 u 在图中的位置，若存在返回下标，否则返回 -1
int LocateVex(ALGraph G, VertexType u) {
    for (int i = 0; i < G.vexnum; ++i) {
        if (G.vertices[i].data == u)
            return i;
    }
    return -1;
}

// 构造无向图(无弧信息)
Status CreateUDG(ALGraph *G) {
    VertexType v1, v2;
    int i, j, k;

    printf("请输入无向图的顶点数、边数:\n");
    scanf("%d %d", &G->vexnum, &G->arcnum);

    printf("请输入 %d 个顶点(空格分隔):\n", G->vexnum);
    for (i = 0; i < G->vexnum; ++i) {
        scanf(" %c", &G->vertices[i].data);
        G->vertices[i].firstarc = NULL;         // 初始化头结点(顶点结点)指针域
    }

    printf("请输入 %d 条边(格式: v1 v2)(回车分割):\n", G->arcnum);
    for (k = 0; k < G->arcnum; ++k) {
        scanf(" %c %c", &v1, &v2);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i == -1 || j == -1) return ERROR;
        // 头插法插入：无向图需双向插入(i为头 和 j为头)，有向图只用插入单向插入
        // 插入 i 的表头(即 边 i-j)
        ArcNode *p1 = (ArcNode*)malloc(sizeof(ArcNode));
        p1->adjvex = j;                         // 边从 i 指向 j
        p1->nextarc = G->vertices[i].firstarc;
        G->vertices[i].firstarc = p1;
        // 插入 j 的表头(即 边 j-i)
        ArcNode *p2 = (ArcNode*)malloc(sizeof(ArcNode));
        p2->adjvex = i;                         // 边从 j 指向 i
        p2->nextarc = G->vertices[j].firstarc;
        G->vertices[j].firstarc = p2;
    }
    return OK;
}

// 销毁图，释放内存 -- 弧指针置空，顶点数组逻辑清空
void DestroyGraph(ALGraph *G) {
    for (int i = 0; i < G->vexnum; ++i) {
        ArcNode *p = G->vertices[i].firstarc;
        while (p) {
            ArcNode *temp = p;
            p = p->nextarc;
            free(temp);
        }
        G->vertices[i].firstarc = NULL;     // 每个顶点的头指针置空
    }
    G->vexnum = 0;
    G->arcnum = 0;
}

/* ---------------- DFS 遍历逻辑 ---------------- */
Boolean visited[MAX_VERTEX_NUM];
Status (*VisitFunc)(VertexType v);

Status Visit(VertexType v) {
    printf("%c ", v);
    return OK;
}

void DFS(ALGraph G, int v) {
    visited[v] = TRUE;
    VisitFunc(G.vertices[v].data);
    for (ArcNode *p = G.vertices[v].firstarc; p != NULL; p = p->nextarc) {  // 通过边找邻接点，不需要FirstAdjVex和NextAdjVex
        if (!visited[p->adjvex])
            DFS(G, p->adjvex);
    }
}

// 图的深度优先遍历 -- O(n + e)(邻接表)
void DFSTraverse(ALGraph G, Status(*Visit)(VertexType v)) {
    VisitFunc = Visit;
    for (int v = 0; v < G.vexnum; ++v) visited[v] = FALSE;
    for (int v = 0; v < G.vexnum; ++v)
        if (!visited[v])
            DFS(G, v);
}

/* ==================== 测试代码 ==================== */
void redirect_input(const char *data) {
    FILE *fp = fopen("Input_DFS_Adjacency_List.txt", "w");
    fprintf(fp, "%s", data);
    fclose(fp);
    freopen("Input_DFS_Adjacency_List.txt", "r", stdin);
}

void test() {
    ALGraph G;
    printf("\n========== 测试：无向图 G4 (邻接表实现) ==========\n");

    // 模拟输入：图 7.13(a)
    // 注意：邻接表采用头插法，遍历顺序可能与邻接矩阵（按列序找）略有不同
    // 但都是正确的深度优先路径(1 3 7 6 2 5 8 4) -- 原因：头插法建表
    redirect_input(
        "8 9\n"
        "1 2 3 4 5 6 7 8\n"
        "1 2\n" 
        "1 3\n"
        "2 4\n" 
        "2 5\n"
        "3 6\n" 
        "3 7\n"
        "4 8\n" 
        "5 8\n"
        "6 7\n"
    );

    if (CreateUDG(&G) == OK) {
        printf("DFS遍历序列(从右到左) (期望为 1 3 7 6 2 5 8 4): \n");
        DFSTraverse(G, Visit);
        printf("\n");
        DestroyGraph(&G);
    }
}

int main() {
    test();
    remove("Input_DFS_Adjacency_List.txt");
    printf("\n========== 测试执行完毕 ==========\n");
    
    freopen("CON", "r", stdin); 
    system("pause");
    return 0;
}