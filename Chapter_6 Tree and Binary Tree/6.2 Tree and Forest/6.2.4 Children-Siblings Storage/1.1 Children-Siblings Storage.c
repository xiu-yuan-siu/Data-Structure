#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

typedef char ElmeType;

typedef struct CSNode {         // Children Siblings Node
    ElmeType data;
    struct CSNode *firstchild, *nextsibling, *parent;       // 这里加了一个双亲域，便于进行双亲操作
} CSNode, *CSTree;

// 便于实现各种树的操作
// 1.找双亲
CSTree GetParent(CSTree T, CSTree x) {
    if (!x || x == T) return NULL;
    return x->parent;
}

// 2.找孩子 -- 要访问结点 x 的第 i 个孩子，则先从 firstchild 找到第 1 个孩子结点，然后沿着孩子结点的 nextsibling 域走 i-1 步
CSTree GetChild(CSTree x, int i) {
    if (!x || i < 1) return NULL;
    CSTree p = x->firstchild;
    for (int j = 1; p && j < i; ++j) {
        p = p->nextsibling;
    }
    return p;           // // 如果 i 超出孩子总数，p 会是 NULL
}
