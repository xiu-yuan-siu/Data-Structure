#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/*
* 十字链表是有向图的一种链式存储结构，故而这里不设置图类型枚举
* 与邻接表相比，十字链表容易求出有向图顶点的入度和出度，故而易求得顶点的度 TD = ID + OD
* 建立十字链表的时间复杂度和建立邻接表的时间复杂度相同
*/

#define MAX_VERTEX_SIZE 20
typedef int InfoType;               // 弧信息类型
typedef char VertexType;            // 顶点数据类型

typedef struct ArcBox {             // 弧结点
    int tailvex, headvex;           // 该弧的弧尾和弧头在数组中的位置
    struct ArcBox *hlink, *tlink;   // 指向弧头相同和弧尾相同的弧
    InfoType *info;                 // 弧相关信息的指针
} ArcBox;

typedef struct VexNode {            // 顶点结点
    VertexType data;
    ArcBox *firstin, *firstout;     // 指向该顶点的第一条入弧(弧头入弧)和出弧(弧尾出弧)
} VexNode;

typedef struct {
    VexNode xlist[MAX_VERTEX_SIZE]; // 表头向量（顶点结点数组）
    int vexnum, arcnum;             // 有向图的当前结点数和弧数
} OLGraph;

// 辅助函数：确定顶点 e 在顶点数组的位置（下标）
int LocateVex(OLGraph G, VertexType e) {
    for (int i = 0; i < G.vexnum; ++i) {
        if (G.xlist[i].data == e) {
            return i;
        }
    }
    return -1;
}

// 创建有向图 -- O(n + e)
Status CreateDG(OLGraph *G) {
    int IncInfo;
    printf("========== 构造有向图 ==========");
    printf("请输入有向图的顶点数、弧数、是否包含弧信息(0/1):\n");
    scanf("%d %d %d", &G->vexnum, &G->arcnum, &IncInfo);
    // 构造表头向量 -- O(n)
    printf("请输入 %d 个顶点信息(空格分隔):\n", G->vexnum);
    for (int i = 0; i < G->vexnum; ++i) {
        scanf(" %c", &G->xlist[i].data);                       // 初始化顶点信息
        G->xlist[i].firstin = G->xlist[i].firstout = NULL;      // 初始化弧指针
    }
    // 输入弧构造十字链表 -- O(e)
    printf("请输入 %d 个弧（格式：弧尾 弧头）(换行分隔):\n", G->arcnum);
    int i, j;
    VertexType v1, v2;                      // 弧尾和弧头
    for (int k = 0; k < G->arcnum; ++k) {
        scanf(" %c %c", &v1, &v2);
        i = LocateVex(*G, v1);          // 弧尾在数组中的位置
        j = LocateVex(*G, v2);          // 
        if (i == -1 || j == -1) return ERROR;
        ArcBox *p = (ArcBox*)malloc(sizeof(ArcBox));
        if (!p) exit(OVERFLOW);
        *p = (ArcBox){i, j, G->xlist[j].firstin, G->xlist[i].firstout, NULL};   // 弧结点结构体赋值
        G->xlist[j].firstin = G->xlist[i].firstout = p;             // 弧头j入弧，弧尾i出弧
        if (IncInfo) {                                              // IncInfo 若弧有相关信息，则输入
            p->info = (InfoType*)malloc(sizeof(InfoType));
            if (!p) exit(OVERFLOW);
            printf("请输入弧的信息:\n");
            scanf("%d", p->info);
        }
    }
    return OK;
}