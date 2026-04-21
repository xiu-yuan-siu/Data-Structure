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
    ChilePtr firstchild;            // 孩子链表的表头指针 -- 指向第一个孩子(最左边的孩子)
} CTBox;

// 树结构
typedef struct {
    CTBox nodes[MAX_TREE_SIZE];     // 结点数组
    int n, r;                       // 结点数，根结点的位置
} CTree;

// 优点: 查找孩子快 GetChildren(T, x) O(d) (d 是该结点的度, 每个结点不同)
Status PrintChildren(CTree T, int i) {
    // 打印下标为 i 的结点的所有孩子
    if (i < 0 || i >= T.n) {
        return ERROR;
    }
    ChilePtr p = T.nodes[i].firstchild;
    while (p) {
        printf("孩子数据: %d", T.nodes[p->child].data);
        p = p->next;
    }
    return OK;
}

// 缺点：查找双亲困难 -- 从根结点开始扫描每一个结点的孩子链表
int GetParent(CTree T, int target_idx) {
    for (int i = 0; i < T.n; ++i) {
        ChilePtr p = T.nodes[i].firstchild;
        while (p) {
            if (p->child == target_idx) 
                return i;
            p = p->next;
        }
    }
    return -1;
}
