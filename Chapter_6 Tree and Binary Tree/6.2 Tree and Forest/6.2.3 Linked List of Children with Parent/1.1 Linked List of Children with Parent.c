#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define MAX_TREE_SIZE 100

typedef char TElemType;

// 孩子链表中的结点(代表一个孩子)
typedef struct CTNode {
    int child;                      // 孩子结点在主数组中的下标
    struct CTNode *next;            // 指向下一个孩子结点的指针
} *ChilePtr;

// 主数组中的结点
typedef struct {
    TElemType data;                 // 结点的数据
    int parent;                     // 增加的双亲域，存放双亲在数组中的下标
    ChilePtr firstchild;            // 孩子链表的表头指针 -- 指向第一个孩子(最左边的孩子)
} PCTBox;

// 树结构
typedef struct {
    PCTBox nodes[MAX_TREE_SIZE];     // 结点数组
    int n, r;                       // 结点数，根结点的位置
} PCTree;

// 优点: 找双亲和孩子都很快，但是较为复杂
// 1.找双亲
int GetParent(PCTree T, int i) {
    if (i < 0 || i > T.n) return -1;
    return T.nodes[i].parent;
}

// 2.找孩子
void PrintChildren(PCTree T, int i) {
    ChilePtr p = T.nodes[i].firstchild;
    while (p) {
        printf("孩子数据: ", T.nodes[p->child].data);
        p = p->next;
    }
}
