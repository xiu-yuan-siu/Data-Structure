#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../../../Chapter_3 Stack and Queue/3.1. Stack/1. Sequential Stack/1.1 Sequential Stack.h"
#include "../../../Status.h"



/* ---------- 邻接表结构 ---------- */
#define MAX_VERTEX_NUM 20
typedef char VertexType;
typedef int InfoType;

typedef struct ArcNode {
    int adjvex;
    struct ArcNode *nextarc;
    InfoType *info;             // 存放弧的权值, 即书中 dut
} ArcNode;
typedef struct VNode {
    VertexType data;
    ArcNode *firstarc;
} VNode, AdjList[MAX_VERTEX_NUM];
typedef struct {
    AdjList vertices;
    int vexnum, arcnum;
} ALGraph;

// 辅助函数：添加边（头插法）
void AddEdge(ALGraph *G, int tail, int head, int weight) {
    ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex = head;
    p->info = (InfoType *)malloc(sizeof(InfoType));
    *(p->info) = weight;
    // 头插法
    p->nextarc = G->vertices[tail].firstarc;
    G->vertices[tail].firstarc = p;
}

// 构建图 7.29
void CreateGraph7_29(ALGraph *G) {
    int i;
    // 初始化顶点
    G->vexnum = 9; // V1 ~ V9
    G->arcnum = 11;
    for (i = 0; i < G->vexnum; ++i) {
        G->vertices[i].data = '1' + i; // V1='1', V2='2'...
        G->vertices[i].firstarc = NULL;
    }
    // 根据图 7.29 添加边 (注意：下标从0开始，V1对应0, V9对应8)
    // a1: V1->V2 (6)
    AddEdge(G, 0, 1, 6);
    // a2: V1->V3 (4)
    AddEdge(G, 0, 2, 4);
    // a3: V1->V4 (5)
    AddEdge(G, 0, 3, 5);
    // a4: V2->V5 (1)
    AddEdge(G, 1, 4, 1);
    // a5: V3->V5 (1)
    AddEdge(G, 2, 4, 1);
    // a6: V4->V6 (2)
    AddEdge(G, 3, 5, 2);
    // a7: V5->V7 (9)
    AddEdge(G, 4, 6, 9);
    // a8: V5->V8 (7)
    AddEdge(G, 4, 7, 7);
    // a9: V6->V8 (4)
    AddEdge(G, 5, 7, 4);
    // a10: V7->V9 (2)
    AddEdge(G, 6, 8, 2);
    // a11: V8->V9 (4)
    AddEdge(G, 7, 8, 4);
}

// 创建图 7.30
void CreateGraph7_30(ALGraph *G) {
    int i;
    
    // 初始化顶点
    G->vexnum = 6;  // V1 ~ V6
    G->arcnum = 8;  // 8条边
    
    for (i = 0; i < G->vexnum; ++i) {
        G->vertices[i].data = '1' + i;  // V1='1', V2='2'...
        G->vertices[i].firstarc = NULL;
    }
    // 根据图 7.30(a) 添加边
    // 注意：下标从0开始，V1对应0, V6对应5
    // a1: V1->V2 (3)
    AddEdge(G, 0, 1, 3);
    // a2: V1->V3 (2)
    AddEdge(G, 0, 2, 2);
    // a3: V2->V4 (2)
    AddEdge(G, 1, 3, 2);
    // a4: V2->V5 (3)
    AddEdge(G, 1, 4, 3);
    // a5: V3->V4 (4)
    AddEdge(G, 2, 3, 4);
    // a6: V3->V6 (3)
    AddEdge(G, 2, 5, 3);
    // a7: V4->V6 (2)
    AddEdge(G, 3, 5, 2);
    // a8: V5->V6 (1)
    AddEdge(G, 4, 5, 1);
}

// 求各顶点入度
void FindInDegree(ALGraph G, int indegree[]) {
    for (int i = 0; i < G.vexnum; ++i) indegree[i] = 0;
    for (int i = 0; i < G.vexnum; ++i) {
        ArcNode *p = G.vertices[i].firstarc;
        while (p) {
            ++indegree[p->adjvex];
            p = p->nextarc;
        }
    }
}

// 全局变量: 各顶点的最早发生时间
int ve[MAX_VERTEX_NUM];

// 正拓扑排序求各顶点的最早发生时间 ve (全局变量)，逆拓扑排序求各顶点的最迟发生时间 vl (在CriticalPath函数中)
Status TopologicalOrder(ALGraph G, SqStack *T) {
    // 初始化栈，T 为返回的拓扑序列顶点栈，S 为零入度顶点栈
    SqStack S;
    InitStack(T);
    InitStack(&S);
    // 求各顶点入度
    int indegree[MAX_VERTEX_NUM];
    FindInDegree(G, indegree);                      
    // 初始化 ve
    for (int i = 0; i < G.vexnum; ++i) 
        ve[i] = 0;
    // 建零入度栈
    for (int i = 0; i < G.vexnum; ++i) {
        if (!indegree[i])
            Push(&S, i);
    }

    int count = 0;                          // 记录拓扑序列顶点数，用于判断是否成环
    while (!StackEmpty(S)) {
        int j;
        Pop(&S, &j);
        Push(T, j);                         // 拓扑序列元素入栈 T, T 为返回的拓扑序列
        ++count;
        for (ArcNode *p = G.vertices[j].firstarc; p; p = p->nextarc) {
            int k = p->adjvex;
            if (--indegree[k] == 0)         // 第 j 个顶点的邻接点入度减 1 后，入度变为 0 则进栈 S
                Push(&S, k);
            if (ve[j] + *(p->info) > ve[k]) // 求 ve 核心：事件 k 想要发生，必须要指向它的所有活动均完成 -- 即路径长度最大的为 ve[k]
                ve[k] = ve[j] + *(p->info); // 核心：更新 ve[k] = max(ve[k], ve[j] + dut)
        }
    }
    if (count < G.vexnum) return ERROR;
    else return OK;
}

// 输出有向网 G 的各项关键活动
Status CriticalPath(ALGraph G) {
    SqStack T;
    if (!TopologicalOrder(G, &T)) return ERROR;         // 有环无法逆拓扑排序和求关键路径

    int vl[MAX_VERTEX_NUM];                             // 所有顶点最迟发生时间
    for (int i = 0; i < G.vexnum; ++i) 
        vl[i] = ve[G.vexnum-1];                         // 初始化顶点事件的最迟发生时间 -- 所有顶点初始化为汇点的最早发生时间 (即工程总工期)
    while (!StackEmpty(T)) {                            // 按拓扑逆序求各顶点的 vl 值
        int j;
        Pop(&T, &j);
        for (ArcNode *p = G.vertices[j].firstarc; p; p = p->nextarc) {
            int k = p->adjvex;
            int dut = *(p->info);                       // 书中 dut<j, k>
            if (vl[k] - dut < vl[j])                    // 核心：vl[j] = min(vl[j], vl[k] - dut)
                vl[j] = vl[k] - dut;
        }
    }
    printf("\n关键活动判定 (ee == el):\n");
    printf("Start\tEnd\tDut\tee\tel\tTag\n");
    printf("----\t---\t---\t--\t--\t---\n");
    // 求 ee, el, 关键活动
    for (int j = 0; j < G.vexnum; ++j) {
        for (ArcNode *p = G.vertices[j].firstarc; p; p = p->nextarc) {
            int k = p->adjvex;
            int dut = *(p->info);
            int ee = ve[j];
            int el = vl[k] - dut;
            char tag = (ee == el) ? '*' : ' ';
            printf("v%d\tv%d\t%d\t%d\t%d\t%c\n", j+1, k+1, dut, ee, el, tag);
        }
    }
    return OK;
}

// --- 5. 主函数 ---
int main() {
    ALGraph G1, G2;
    CreateGraph7_29(&G1);
    CreateGraph7_30(&G2);
    if (CriticalPath(G1) == OK) {
        printf("\n计算完成。带 '*' 的行表示关键活动。\n");
    } else {
        printf("图中存在回路，无法计算关键路径。\n");
    }
    if (CriticalPath(G2) == OK) {
        printf("\n计算完成。带 '*' 的行表示关键活动。\n");
    } else {
        printf("图中存在回路，无法计算关键路径。\n");
    }
    system("pause");
    return 0;
}

/*
* 切换到当前文件所在目录: cd ".\Chapter_7 Graph\7.4 Directed Acycline Graph\7.4.2 Critical Path\" 
* 编译指令: gcc -finput-charset=UTF-8 -fexec-charset=GBK "1. Critical Path.c" "../../../Chapter_3 Stack and Queue/3.1. Stack/1. Sequential Stack/1.1 Sequential Stack.c" -o "1. Critical Path.exe"
* tasks.json 的配置仅在 VSCode 内按 Ctrl+Shift+B 或 F5 时生效。终端手动编译需自行携带 -fexec-charset
*/