#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/*
* 邻接多重表时无向图的一种链式存储结构，故不列出图中的的枚举
* 邻接多重表与邻接表的区别：1.同一条边邻接表用两个结点，而邻接多重表只有一个结点；2.邻接多重表多了 mark 标志域标记边是否搜索
*/

#define MAX_VERETX_SIZE 20
typedef int InfoType;
typedef char VertexType;
typedef enum { unvisited, visited } VisitIf;            // 是否访问（0=未访问，1=已访问）

// 边结点
typedef struct EBox {
    VisitIf mark;                           // 访问标记
    int ivex, jvex;                         // 该边依附的两个顶点在顶点数组中的位置
    struct EBox *ilink, *jlink;             // 分别指向依附这两个顶点的下一条边
    InfoType *info;                         // 该边的信息指针
} EBox;

// 顶点结点
typedef struct VexBox {
    VertexType data;
    EBox *firstedge;                        // 指向第一条依附该顶点的边
} VexBox;

typedef struct {
    VexBox adjmulist[MAX_VERETX_SIZE];      // 顶点数组
    int vexnum, arcnum;                     // 无向图的当前顶点数和边数
} AMLGraph;
