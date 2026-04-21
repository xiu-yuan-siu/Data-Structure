#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define MAX_TREE_SIZE 100

typedef char TElemType;

typedef struct PTNode {             // 结点结构
    TElemType data;
    int parent;                     // 双亲位置域
} PTNode;                           // Parent Tree Node

typedef struct {                    // 树结构
    PTNode nodes[MAX_TREE_SIZE];    // 顺序结构 -- 下标代替指针(静态链表), 从 0 开始存储数据元素
    int r, n;                       // 根的位置和结点数
} PTree;                            // Parent Tree

// 按照书中简单写三个函数对比双亲表存储的优缺点
// 1.找双亲
// 优点：直接访问 nodes[e].parent 即可，时间复杂度 O(1)
int Parent(PTree T, int e) {
    if (e >= 0 && e < T.n) {    // e 合法
        return T.nodes[e].parent;
    }
    return -1;                  // 没找到双亲
}

// 2.找根节点
// 优点: 时间复杂度 O(1), 若传入int e 也可通过不断访问双亲最终找到根节点(最好直接用 根的位置下标)
int Root(PTree T) {
    if (T.n > 0) {              // 非空树
        return T.r;
    }
    return -1;
}

// 3.找左孩子
// 缺点: 必须遍历整个数组来寻找谁的双亲是 e，时间复杂度 O(n)
int LeftChild(PTree T, int e) {
    if (e < 0 || e >= T.n) {
        return -1;
    }
    for (int i = 0; i < T.n; ++i) {
        if (T.nodes[i].parent == e)
            return i;           // 找到第一个双亲为 e 的结点，即为左孩子（第一个孩子）
    }
    return -1;
}
