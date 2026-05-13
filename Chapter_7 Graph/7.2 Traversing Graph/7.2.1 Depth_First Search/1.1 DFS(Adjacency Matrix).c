#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../Status.h"

/* ---------------- 邻接矩阵存储表示的DFS(以书中无向图 G4 为测试案例，故而只写无向图创建的代码) ---------------- */
#define INFINITY INT_MAX            // 最大值为int的最大值
#define MAX_VERTEX_NUM 20
typedef enum {DG, DN, UDG, UDN} GraphKind;
typedef char VertexType;            // 顶点数据类型
typedef int VRType;                 // 顶点关系类型
typedef char InfoType;              // 弧相关信息类型

typedef struct ArcCell {            // 邻接矩阵元素结构
    VRType adj;                     // 无向图为 0/1，有向图为 权值/无穷
    InfoType *info;
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];   // 二阶数组表示矩阵

typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];    // 顶点数据数组
    AdjMatrix arcs;                     // 顶点关系邻接矩阵
    int vexnum, arcnum;
    GraphKind kind;
} MGraph;                               // Matrix Graph

// 辅助函数: 查找顶点 u 在图中的位置，若存在返回下标，否则返回 -1
int LocateVex(MGraph G, VertexType u) {
    for (int i = 0; i < G.vexnum; ++i) {
        if (G.vexs[i] == u) {
            return i;
        }
    }
    return -1;
}

// 辅助函数: 输入弧的相关信息(此处简化为无信息输入)
Status Input(InfoType *info) {
    // 可拓展输入
    return OK;
}

// 构造无向图 (仅保留此函数，满足 G4 测试需求)
Status CreateUDG(MGraph *G) {
    int i, j, k;
    int IncInfo;
    VertexType v1, v2;

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
        
        G->arcs[i][j].adj = 1;          // 图这里为 1
        G->arcs[j][i].adj = 1;          // 无向图对称赋值
        
        if (IncInfo) {                  // 若弧含有相关信息，则输入
            G->arcs[i][j].info = (InfoType*)malloc(sizeof(InfoType));
            Input(G->arcs[i][j].info);
        } else {
            G->arcs[i][j].info = NULL;
        }
        G->arcs[j][i].info = NULL;      // 避免 DestroyGraph 时重复释放
    }
    return OK;
}

// 构造图 -- 对用户的接口 -- 简化版
Status CreateGraph(MGraph *G) {
    int kind;
    printf("请输入图的种类(0=DG, 1=DN, 2=UDG, 3=UDN): \n");
    scanf("%d", &kind);
    G->kind = (GraphKind)kind;
    
    if (G->kind == UDG) {
        return CreateUDG(G);
    } else {
        printf("当前精简版代码仅保留了无向图(UDG)的构造逻辑！\n");
        return ERROR;
    }
}

// 释放动态分配的弧信息内存
void DestroyGraph(MGraph *G) {
    if (G == NULL) return;
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
        printf("%c  ", G.vexs[i]);        
        for (int j = 0; j < G.vexnum; ++j) {
            printf("%4d ", G.arcs[i][j].adj);   // 0/1
        }
        printf("\n");
    }
}

/* ---------------- DFS 遍历逻辑 ---------------- */
Boolean visited[MAX_VERTEX_NUM];            // 访问标志数组，大小与顶点数相同，从 0 开始   
Status (*VisitFunc)(VertexType v);          // 函数变量

// VisitFunc 案例 -- Visit
Status Visit(VertexType v) {
    printf("%c ", v);
    return OK;
}

// 返回 v 的第一个邻接顶点
int FirstAdjVex(MGraph G, int v) {
    for (int j = 0; j < G.vexnum; ++j) {
        if (G.arcs[v][j].adj != 0) return j;
    }
    return -1;
}

// 返回 v 的(相对于 w 的)下一个邻接顶点
int NextAdjVex(MGraph G, int v, int w) {
    for (int j = w + 1; j < G.vexnum; ++j) {        // 从 w 的下一列开始找                        
        if (G.arcs[v][j].adj != 0) return j;
    }          
    return -1;
}

void DFS(MGraph G, int v) {
    visited[v] = TRUE;                      // 更新 v 对应的访问标志数组
    VisitFunc(G.vexs[v]);                   // 访问第 v 个顶点的数据                
    for (int w = FirstAdjVex(G, v); w >= 0; w = NextAdjVex(G, v, w))    // 每个结点有多个邻接顶点，要进行遍历 -- O(n^2)
        if (!visited[w])
            DFS(G, w);                      // 对 v 尚未访问的邻接顶点 w 递归调用 DFS                     
}

// 图的深度优先遍历 -- O(n^2)(邻接矩阵，体现在调用DFS查找邻接点的两个函数中，相当于对 nxn 矩阵每个元素都遍历了一遍)
void DFSTraverse(MGraph G, Status(*Visit)(VertexType v)) {
    VisitFunc = Visit;                      // 使用全局变量 VisitFunc, 使 DFS 不必设函数指针参数                 
    for (int v = 0; v < G.vexnum; ++v)      // 初始化标志数组(0-G.vexnum均为 FALSE, 表示未访问)
        visited[v] = FALSE;
    for (int v = 0; v < G.vexnum; ++v)      
        if (!visited[v])                    
            DFS(G, v);                      // 对尚未访问的顶点调用 DFS       
}

/* ==================== 测试代码 ==================== */
void redirect_input(const char *data) {
    FILE *fp = fopen("Input_DFS_Adjacency_Matrix.txt", "w");
    fprintf(fp, "%s", data);
    fclose(fp);
    freopen("Input_DFS_Adjacency_Matrix.txt", "r", stdin);
}

// 专门测试图 7.13 的无向图 G4
void test() {
    MGraph G;
    printf("\n========== 测试用例：图 7.13(a) 无向图 G4 ==========\n");
    
    // 构造输入数据：
    // 2          (选择UDG无向图)
    // 8 8 0      (8个顶点 v1~v8, 8条边, 无弧信息)
    // 1 2 3 4 5 6 7 8 (用数字字符1-8代表顶点v1-v8)
    // 接下来 8 行是教材图 7.13(a) 中的所有无向边
    redirect_input(
        "2\n"
        "8 9 0\n"
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
    
    Status s = CreateGraph(&G);
    if (s == OK) {
        PrintGraph(G);
        printf("\n=> DFS遍历序列(从左到右) (期望为 1 2 4 8 5 3 6 7): \n");
        DFSTraverse(G, Visit);
        printf("\n");
        DestroyGraph(&G);
    } else {
        printf("图构造失败！\n");
    }
}

int main() {
    test();
    
    remove("Input_DFS_Adjacency_Matrix.txt");
    printf("\n========== 测试执行完毕 ==========\n");

    freopen("CON", "r", stdin); // 恢复键盘控制
    // system("pause");
    return 0;
}