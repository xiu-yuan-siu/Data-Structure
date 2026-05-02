#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define MAX_VERTEX_NUM 20
typedef char VertexType;

Boolean visited[MAX_VERTEX_NUM];            // 访问标志数组，大小与顶点数相同，从 0 开始   

// 遍历函数示例
Status Visit(VertexType v) {
    printf("%c ", v);
    return OK;
}

/* ==================== 1. 辅助数据结构：链队列实现 ==================== */
// 类似树的层序遍历借助队列实现，广度优先搜索也借助队列实现，队列用于存储图的顶点下标(int 类型)
typedef struct QNode {
    int data;
    struct QNode *next;
} QNode, *QueuePtr;

typedef struct {
    QueuePtr front;
    QueuePtr rear;
} LinkQueue;

Status InitQueue(LinkQueue *Q) {
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
    if (!Q->front || !Q->rear) exit(OVERFLOW);
    Q->front->next = NULL;
    return OK;
}

Status DestroyQueue(LinkQueue *Q) {
    while (Q->front) {
        Q->rear = Q->front->next;
        free(Q->front);
        Q->front = Q->rear;
    }
    return OK;
}

Boolean QueueEmpty(LinkQueue Q) {
    return Q.front == Q.rear;
}

// 尾插法入队
Status EnQueue(LinkQueue *Q, int e) {
    QNode *p = (QueuePtr)malloc(sizeof(QNode));
    if (!p) exit(OVERFLOW);
    p->data = e;
    p->next = NULL;
    Q->rear->next = p;
    Q->rear = p;
    return OK;
}

// 头删法出队
Status DeQueue(LinkQueue *Q, int *e) {
    if (Q->front == Q->rear) return ERROR;      // 队空
    QNode *p = Q->front->next;
    Q->front->next = p->next;
    *e = p->data;
    if (Q->rear == p) Q->rear = Q->front;       // 队列只有一个元素，则 rear = p，为避免将rear所指删除，应该先令 rear = front
    free(p);
    p = NULL;
    return OK;
}

/* ==================== 2. 邻接矩阵及 BFS ==================== */
// 无 InfoType 和 GraphKind 简化成一个结构体
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];
    int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum, arcnum;
} MGraph;

// 辅助函数: 查找顶点 u 在图中的位置，若存在返回下标，否则返回 -1
int LocateVex_M(MGraph G, VertexType u) {
    for (int i = 0; i < G.vexnum; ++i)
        if (G.vexs[i] == u)
            return i;
    return -1;
}

// 构造无向图
Status CreateUDG_M(MGraph *G) {
    int i, j, k;
    VertexType v1, v2;

    printf("请输入无向图UDG的顶点数、弧数:\n");
    scanf("%d %d", &G->vexnum, &G->arcnum);

    printf("请输入 %d 个顶点(空格分隔): \n", G->vexnum);
    for (i = 0; i < G->vexnum; ++i) {
        scanf(" %c", &G->vexs[i]);
    }
    // 初始化邻接矩阵
    for (i = 0; i < G->vexnum; ++i) {
        for (j = 0; j < G->vexnum; ++j) {
            G->arcs[i][j] = 0;
        }
    }
    // 构造邻接矩阵
    printf("请输入 %d 个弧, 每行一条弧(格式: 顶点1 顶点2): \n", G->arcnum);
    for (k = 0; k < G->arcnum; ++k) {
        scanf(" %c %c", &v1, &v2);
        i = LocateVex_M(*G, v1);
        j = LocateVex_M(*G, v2);
        if (i == -1 || j == -1) return ERROR;
        G->arcs[i][j] = 1;          // 图这里为 1
        G->arcs[j][i] = 1;          // 无向图对称赋值
    }
    return OK;
}

void DestroyGraph_M(MGraph *G) {
    if (G == NULL) return;
    G->vexnum = 0;
    G->arcnum = 0;
}

/* ---------------- BFS 遍历逻辑 ---------------- */
// 返回 v 的第一个邻接顶点
int FirstAdjVex(MGraph G, int v) {
    for (int j = 0; j < G.vexnum; ++j) {
        if (G.arcs[v][j] != 0) return j;
    }
    return -1;
}

// 返回 v 的(相对于 w 的)下一个邻接顶点
int NextAdjVex(MGraph G, int v, int w) {
    for (int j = w + 1; j < G.vexnum; ++j) {        // 从 w 的下一列开始找                        
        if (G.arcs[v][j] != 0) return j;
    }          
    return -1;
}

// 图的广度优先非递归遍历 -- 使用辅助队列 Q，时间复杂度与深度优先遍历相同
void BFSTraverse_M(MGraph G, Status(*Visit)(VertexType v)) {
    for (int v = 0; v < G.vexnum; ++v) visited[v] = FALSE;
    LinkQueue Q;
    InitQueue(&Q);                          // 置空的辅助队列 Q
    for (int v = 0; v < G.vexnum; ++v) {    // 嵌套循环，外层遍历所有顶点 -- O(n)
        if (!visited[v]) {                  // v 尚未访问
            visited[v] = TRUE;
            Visit(G.vexs[v]);
            EnQueue(&Q, v);                 // 入队 -- 先入队的先出队，即顶点 v 的邻接节点在下一层中先遍历
            while (!QueueEmpty(Q)) {        // 先被访问的顶点，其邻接点也先被访问 -- 队列 先进先出 的特性
                int u;
                DeQueue(&Q, &u);            // 队头元素出队并置为 u
                for (int w = FirstAdjVex(G, u); w >= 0; w = NextAdjVex(G, u, w)) {      // 遍历 u 的所有邻接点 -- O(n)
                    if (!visited[w]) {      // w 为 u 未访问的邻接节点
                        visited[w] = TRUE;
                        Visit(G.vexs[w]);
                        EnQueue(&Q, w);     // 邻接点入队
                    }
                }
            }
        }
    }
    DestroyQueue(&Q);
}

/* ==================== 3. 邻接表及 BFS ==================== */
typedef struct ArcNode {            // 弧结点
    int adjvex;                     // 该弧所指向的顶点在数组中的下标
    struct ArcNode *nextarc;        // 指向下一条弧的指针
} ArcNode;

typedef struct VNode {              // 顶点结点
    VertexType data;
    ArcNode *firstarc;              // 指向第一条依附该顶点的指针
} VNode, AdjList[MAX_VERTEX_NUM];

typedef struct {
    AdjList vertices;               // 顶点结点数组
    int vexnum, arcnum;
} ALGraph;                          // Adjacency List

// 查找顶点 u 在图中的位置，若存在返回下标，否则返回 -1
int LocateVex_AL(ALGraph G, VertexType u) {
    for (int i = 0; i < G.vexnum; ++i) {
        if (G.vertices[i].data == u)
            return i;
    }
    return -1;
}

// 构造无向图(无弧信息)
Status CreateUDG_AL(ALGraph *G) {
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
        i = LocateVex_AL(*G, v1);
        j = LocateVex_AL(*G, v2);
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
void DestroyGraph_AL(ALGraph *G) {
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

void BFSTraverse_AL(ALGraph G, Status(*Visit)(VertexType v)) {
    for (int v = 0; v < G.vexnum; ++v) visited[v] = FALSE;
    LinkQueue Q;
    InitQueue(&Q);
    for (int v = 0; v < G.vexnum; ++v) {
        if (!visited[v]) {
            visited[v] = TRUE;
            Visit(G.vertices[v].data);
            EnQueue(&Q, v);
            while (!QueueEmpty(Q)) {
                int u;
                DeQueue(&Q, &u);            // 先入队的先出队
                for (ArcNode *p = G.vertices[u].firstarc; p != NULL; p = p->nextarc) {
                    if (!visited[p->adjvex]) {
                        visited[p->adjvex] = TRUE;
                        Visit(G.vertices[p->adjvex].data);
                        EnQueue(&Q, p->adjvex);
                    }
                }
            }
        }
    }
    DestroyQueue(&Q);
}

/* ==================== 测试框架 ==================== */
void redirect_input(const char *data) {
    FILE *fp = fopen("test_bfs.txt", "w");
    fprintf(fp, "%s", data);
    fclose(fp);
    freopen("test_bfs.txt", "r", stdin);
}

int main() {
    // 数据依然是教材 图7.13 的 G4 
    const char* input_data = 
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
        "6 7\n";

    printf("\n============= 邻接矩阵的 BFS =============\n");
    redirect_input(input_data);
    MGraph MG;
    if (CreateUDG_M(&MG)) {
        printf("邻接矩阵 BFS 遍历序列: ");
        BFSTraverse_M(MG, Visit); // 期望输出类似: 1 2 3 4 5 6 7 8
        printf("\n");
        DestroyGraph_M(&MG);
    }

    printf("\n============= 邻接表的 BFS =============\n");
    redirect_input(input_data);
    ALGraph ALG;
    if (CreateUDG_AL(&ALG)) {
        printf("邻接表 BFS 遍历序列: ");
        BFSTraverse_AL(ALG, Visit); // 输出顺序由 头插法 决定(从右到左)，同一层的节点顺序可能与矩阵不同
        printf("\n");
        DestroyGraph_AL(&ALG);
    }

    remove("test_bfs.txt");
    printf("\n========== 所有测试执行完毕 ==========\n");

    freopen("CON", "r", stdin);
    system("pause");
    return 0;
}