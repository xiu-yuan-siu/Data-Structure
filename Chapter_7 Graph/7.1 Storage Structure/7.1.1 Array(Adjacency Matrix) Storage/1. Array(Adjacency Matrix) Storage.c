#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../Status.h"

#define INFINITY INT_MAX                        // 最大值♾️(TNT_MAX代表无穷大(int所能存储的最大值))
#define MAX_VERTEX_NUM 20                       // 最大顶点个数
typedef int VRType;                             // 邻接矩阵元素类型(对图为 1 或 0，代表是否邻接，对网为权值)
typedef char InfoType;                          // 弧相关信息类型
typedef char VertexType;                        // 顶点类型

typedef enum { DG, DN, UDG, UDN } GraphKind;    // {Digraph, Dinetwork, Undigraph, Undinetwork}
typedef struct ArcCell {
    VRType adj;                                 // VRType 是顶点关系类型。对无权图，用 1(邻接) 或 0(不邻接)，对网(有权图)，用 weight(权)或无穷(不可到达)
    InfoType *info;                             // 该弧相关信息的指针
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];            // 顶点向量, 用于定位，下标值即为在邻接矩阵中的行/列(行 = 列)
    AdjMatrix arcs;                             // 邻接矩阵(存储顶点间的关系(是否邻接/权值) -- 即VR)
    int vexnum, arcnum;                         // 图当前的顶点数和弧数
    GraphKind kind;                             // 图的种类标志
} MGraph;

// 辅助函数: 查找顶点 u 在图中的位置，若存在返回下标，否则返回 -1
int LocateVex(MGraph G, VertexType u) {
    for (int i = 0; i < G.vexnum; ++i) {
        if (G.vexs[i] == u) {
            return i;
        }
    }
    return -1;
}

// 辅助函数: 输入弧的相关信息(此处简化为无信息输入, 仅分配空间示意)
Status Input(InfoType *info) {
    // 若有额外信息，可扩展
    // scanf(" %c", info); // 从缓冲区读取字符信息
    return OK;
}

// 构造有向图
Status CreateDG(MGraph *G) {
    int i, j, k;
    int IncInfo;
    VertexType v1, v2;

    printf("\n========= 构造有向图 ==========\n");
    printf("请输入有向图DG的顶点数、弧数、是否有弧信息(0/1): \n");
    scanf("%d %d %d", &G->vexnum, &G->arcnum, &IncInfo);

    printf("请输入 %d 个顶点(空格分隔): \n", G->vexnum);
    for (i = 0; i < G->vexnum; ++i) {
        scanf(" %c", &G->vexs[i]);     // 前面用 scanf("%d") 读取数字后，输入缓冲区残留 \n，紧接着的 scanf("%c") 会读入这个换行符，导致顶点/弧读取错位 -- 在 %c 前加空格，写成 scanf(" %c", ...)，让 scanf 自动跳过空白字符
    }
    // 初始化邻接矩阵
    for (i = 0; i < G->vexnum; ++i) {
        for (j = 0; j < G->vexnum; ++j) {
            G->arcs[i][j].adj = 0;      // 网这里为无穷
            G->arcs[i][j].info = NULL;
        }
    }
    // 构造邻接矩阵
    printf("请输入 %d 个弧, 每行一条弧(格式: 顶点1 顶点2): \n", G->arcnum);
    for (k = 0; k < G->arcnum; ++k) {
        scanf(" %c %c", &v1, &v2);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i == -1 || j == -1) return ERROR;
        // 给 adj 赋值
        G->arcs[i][j].adj = 1;          // 图这里为 1
        // 给 info 赋值
        if (IncInfo) {                  // 若弧含有相关信息，则输入
            G->arcs[i][j].info = (InfoType*)malloc(sizeof(InfoType));
            Input(G->arcs[i][j].info);
        }
    }
    return OK;
}

// 构造有向网
Status CreateDN(MGraph *G) {
    int i, j, k, w;
    int IncInfo;
    VertexType v1, v2;

    printf("\n========= 构造有向网 ==========\n");
    printf("请输入有向网DN的顶点数、弧数、是否有弧信息(0/1): \n");
    scanf("%d %d %d", &G->vexnum, &G->arcnum, &IncInfo);

    printf("请输入 %d 个顶点(空格分隔): \n", G->vexnum);
    for (i = 0; i < G->vexnum; ++i) {
        scanf(" %c", &G->vexs[i]);
    }
    // 初始化邻接矩阵
    for (i = 0; i < G->vexnum; ++i) {
        for (j = 0; j < G->vexnum; ++j) {
            G->arcs[i][j].adj = INFINITY;   // 网这里为无穷
            G->arcs[i][j].info = NULL;
        }
    }
    // 构造邻接矩阵
    printf("请输入 %d 个弧, 每行一条弧(格式: 顶点1 顶点2 权值): \n", G->arcnum);
    for (k = 0; k < G->arcnum; ++k) {
        scanf(" %c %c %d", &v1, &v2, &w);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i == -1 || j == -1) return ERROR;
        // 给 adj 赋值
        G->arcs[i][j].adj = w;          // 网这里为权值
        // 给 info 赋值
        if (IncInfo) {                  // 若弧含有相关信息，则输入
            G->arcs[i][j].info = (InfoType*)malloc(sizeof(InfoType));
            Input(G->arcs[i][j].info);
        }
    }
    return OK;
}

// 构造无向图
Status CreateUDG(MGraph *G) {
    int i, j, k;
    int IncInfo;
    VertexType v1, v2;

    printf("\n========= 构造无向图 ==========\n");
    printf("请输入无向图UDG的顶点数、弧数、是否有弧信息(0/1): \n");
    scanf("%d %d %d", &G->vexnum, &G->arcnum, &IncInfo);

    printf("请输入 %d 个顶点(空格分隔): \n", G->vexnum);
    for (i = 0; i < G->vexnum; ++i) {
        scanf(" %c", &G->vexs[i]);
    }
    // 初始化邻接矩阵
    for (i = 0; i < G->vexnum; ++i) {
        for (j = 0; j < G->vexnum; ++j) {
            G->arcs[i][j].adj = 0;
            G->arcs[i][j].info = NULL;
        }
    }
    // 构造邻接矩阵
    printf("请输入 %d 个弧, 每行一条弧(格式: 顶点1 顶点2): \n", G->arcnum);
    for (k = 0; k < G->arcnum; ++k) {
        scanf(" %c %c", &v1, &v2);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i == -1 || j == -1) return ERROR;
        // 给 adj 赋值
        G->arcs[i][j].adj = 1;          // 图这里为 1
        G->arcs[j][i].adj = 1;          // 无向图/网这里要对称赋值
        // 给 info 赋值
        if (IncInfo) {                  // 若弧含有相关信息，则输入
            G->arcs[i][j].info = (InfoType*)malloc(sizeof(InfoType));
            Input(G->arcs[i][j].info);
        } else {
            G->arcs[i][j].info = NULL;
        }
        G->arcs[j][i].info = NULL;      // 避免 DestroyGraph 时销毁 double free
    }
    return OK;
}

// 构造无向网 -- 算法 7.2
Status CreateUDN(MGraph *G) {
    int i, j, k, w;
    int IncInfo;
    VertexType v1, v2;

    printf("\n========= 构造无向网 ==========\n");
    printf("请输入无向网UDN的顶点数、弧数、是否有弧信息(0/1): \n");
    scanf("%d %d %d", &G->vexnum, &G->arcnum, &IncInfo);

    printf("请输入 %d 个顶点(空格分隔): \n", G->vexnum);
    for (i = 0; i < G->vexnum; ++i) {
        scanf(" %c", &G->vexs[i]);
    }
    // 初始化邻接矩阵
    for (i = 0; i < G->vexnum; ++i) {
        for (j = 0; j < G->vexnum; ++j) {
            G->arcs[i][j].adj = INFINITY;   // 网这里为无穷
            G->arcs[i][j].info = NULL;
        }
    }
    // 构造邻接矩阵
    printf("请输入 %d 个弧, 每行一条弧(格式: 顶点1 顶点2 权值): \n", G->arcnum);
    for (k = 0; k < G->arcnum; ++k) {
        scanf(" %c %c %d", &v1, &v2, &w);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i == -1 || j == -1) return ERROR;
        // 给 adj 赋值
        G->arcs[i][j].adj = w;          // 网这里为权值
        G->arcs[j][i].adj = w;          // 无向图/网这里要对称赋值
        // 给 info 赋值
        if (IncInfo) {                  // 若弧含有相关信息，则输入
            G->arcs[i][j].info = (InfoType*)malloc(sizeof(InfoType));
            Input(G->arcs[i][j].info);
        } else {
            G->arcs[i][j].info = NULL;
        }
        G->arcs[j][i].info = NULL;      // 对称位置不分配 info，避免重复释放
    }
    return OK;
}

// 构造图 -- 对用户的接口 -- 算法 7.1
Status CreateGraph(MGraph *G) {
    int kind;
    printf("请输入图的种类(0=DG有向图, 1=DN有向网, 2=UDG无向图, 3=UDN无向网): ");
    scanf("%d", &kind);
    G->kind = (GraphKind)kind;          // 通过枚举把数字转换成对应的string
    switch (G->kind) {
        case DG: return CreateDG(G);
        case DN: return CreateDN(G);
        case UDG: return CreateUDG(G);
        case UDN: return CreateUDN(G);
        default: return ERROR;
    }
}

// 释放动态分配的弧信息内存(数组逻辑销毁 -- 即不进行处理)
void DestroyGraph(MGraph *G) {
    if (G == NULL) {
        printf("警告: 试图销毁空图\n");
        return;
    }
    for (int i = 0; i < G->vexnum; ++i) {
        for (int j = 0; j < G->vexnum; ++j) {
            if (G->arcs[i][j].info != NULL) {
                free(G->arcs[i][j].info);
                G->arcs[i][j].info = NULL;
            }
        }
    }
    G->vexnum = 0;
    G->arcnum = 0;
}

// 打印邻接矩阵
void PrintGraph(MGraph G) {
    printf("\n图的邻接矩阵:\n   ");
    for (int i = 0; i < G.vexnum; ++i) {
        printf("%4c ", G.vexs[i]);
    }
    printf("\n");

    for (int i = 0; i < G.vexnum; ++i) {
        printf("%c  ", G.vexs[i]);        // 顶点名称
        for (int j = 0; j < G.vexnum; ++j) {
            if (G.arcs[i][j].adj == INFINITY)
                printf(" INF ");
            else if (G.kind == DG || G.kind == UDG) 
                printf("%4d ", G.arcs[i][j].adj);   // 0/1
            else 
                printf("%4d ", G.arcs[i][j].adj);   // 权值
        }
        printf("\n");
    }
}

int main(void)
{
    MGraph G;
    Status result;

    result = CreateGraph(&G);
    if (result == OK) {
        PrintGraph(G);
    } else {
        printf("图创建失败！\n");
    }

    DestroyGraph(&G);
    // system("pause");
    return 0;
}