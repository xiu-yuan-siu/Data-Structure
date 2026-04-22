#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* =================== 使用双亲表示实现并查集功能 =================== */

/*
* 1.并查集(Merge Find Set, 常称为 Disjoint Set 不相交集) -- 用于合并等价类
*/

#define MAX_TREE_SIZE 100

// 不使用数据域是因为 下标本身 就代表了 元素(节省空间)
typedef struct {
    int parent;             // 逻辑重构:为负数时表示根结点，其绝对值表示该集合的成员个数; 为非负数时表示其双亲的下标
} ElemTag;

typedef struct {
    ElemTag nodes[MAX_TREE_SIZE];
    int n;                  // 元素个数, 没有根结点下标是因为一个数组就是一片森林，有很多根结点，且根结点的数目是动态变化的
} MFSet;

// 1.构造(Initial)
void initial_mfset(MFSet *S, int n) {
    S->n = n;
    for (int i = 1; i <= n; ++i) {      // 下标为 0 的位置为空(下标代表元素)
        S->nodes[i].parent = -1;        // 每个成员都是一个集合(根结点)，每个集合只有一个元素
    }
}

// 2.1 查找(Find) -- 顺链向上找到树的根结点
int find_mfset(MFSet S, int i) {
    if (i < 1 || i > S.n) return -1;        // i 非法
    // 顺链向上找到树的根结点
    int root = i;
    while (S.nodes[root].parent > 0) {      // 非根结点进入循环，parent 为负数则找到根结点，退出循环并返回根结点的下标(集合)
        root = S.nodes[root].parent;
    }
    return root;
}

// 2.2 路径压缩查找(优化查找) -- 将 i 及其所有祖先直接指向根节点，从而彻底扁平化树结构
int fix_mfset(MFSet *S, int i) {
    if (i < 1 || i > S->n) return -1;
    // 找根结点
    int root = i;
    while (S->nodes[root].parent > 0) {
        root = S->nodes[root].parent;
    }
    // 将从 i 至根路径上所有结点都变成根的孩子结点
    int curr = i;                           // 相当于书中 k
    while (curr != root) {
        int p = S->nodes[curr].parent;         // curr 的parent(上一个)，相当于书中 t
        S->nodes[curr].parent = root;
        curr = p;
    }
    return root;
}

// 3.1 基础合并(Merge)
Status merge_mfset(MFSet *S, int i, int j) {    // i, j 是两个不同等价类的根
    if (i < 1 || i > S->n || j < 1 || j > S->n) return ERROR;
    S->nodes[i].parent = j;                     // 合并：直接将一棵子集树的根结点指向另一棵子集树的根结点
    return OK;
}

// 3.2 混合合并 -- 为了防止树退化成链表(最坏的情况)，总是把结点数少的树合并到结点数多的树中
Status mix_mfset(MFSet *S, int i, int j) {                // i, j 是两个不同等价类的根
    if (i < 1 || i > S->n || j < 1 || j > S->n) return ERROR;
    if (S->nodes[i].parent > S->nodes[j].parent) {      // i 的结点数更少，则更新 j 的结点数目，将 i 挂到 j
        S->nodes[j].parent += S->nodes[i].parent;
        S->nodes[i].parent = j;
    } else {                                            // j 的结点数更少，则更新 i 的结点数目，将 j 挂到 i
        S->nodes[i].parent += S->nodes[j].parent;
        S->nodes[j].parent = i;
    }
    return OK;
}

/* ================= 测试用例 ================= */
void print_set_state(MFSet *S, int limit) {
    printf("当前 parent 数组状态 (1~%d): ", limit);
    for(int i = 1; i <= limit; i++) {
        printf("[%d]=%d ", i, S->nodes[i].parent);
    }
    printf("\n");
}

int main() {
    MFSet S;
    int n = 10; // 包含 1..9 个元素

    printf("=== 1. 初始化测试 ===\n");
    initial_mfset(&S, n);
    print_set_state(&S, n);

    printf("\n=== 2. 基础合并(merge)与查找测试 ===\n");
    merge_mfset(&S, 1, 2); // 1 挂到 2 下
    merge_mfset(&S, 2, 3); // 2 挂到 3 下
    print_set_state(&S, 3);
    printf("查找 1 的根: %d (期望值: 3)\n", find_mfset(S, 1));
    printf("查找 4 的根: %d (期望值: 4)\n", find_mfset(S, 4));

    printf("\n=== 3. 混合合并(mix)测试 ===\n");
    initial_mfset(&S, n); // 重置
    mix_mfset(&S, 1, 2);  // 2 挂到 1 下，1 的大小变为 -2
    mix_mfset(&S, 3, 4);  // 4 挂到 3 下，3 的大小变为 -2
    printf("Mix(1,2) 和 Mix(3,4) 后: \n");
    print_set_state(&S, 4);
    mix_mfset(&S, 1, 3);  // 3 挂到 1 下，1 的大小变为 -4
    printf("Mix(3,1) 后 (期望 1 为根且值为 -4, 3 指向 1): \n");
    print_set_state(&S, 4);

    printf("\n=== 4. 路径压缩(fix)测试：书中例6-1场景 ===\n");
    initial_mfset(&S, n); // 重置

    // 根据书142页例6-1构造初始树结构
    // 依次执行: (1,2), (3,4), (5,6), (7,8), (1,3), (5,7), (1,5)
    mix_mfset(&S, 1, 2); // 1->2
    mix_mfset(&S, 3, 4); // 3->4
    mix_mfset(&S, 5, 6); // 5->6
    mix_mfset(&S, 7, 8); // 7->8
    mix_mfset(&S, 1, 3); // 1的根2挂到3的根4下: 2->4
    mix_mfset(&S, 5, 7); // 5的根6挂到7的根8下: 6->8
    mix_mfset(&S, 1, 5); // 1的根4挂到5的根8下: 4->8

    printf("Mix构建, Fix 前状态: \n");
    print_set_state(&S, 9);

    fix_mfset(&S, 8);
    printf("Fix(S, 8) 后的状态(期望: 7 和 8 的值均为 1): \n");
    print_set_state(&S, 9);

    printf("\n=== 5. 报错捕获测试 ===\n");
    printf("越界查找 Find(11): %d (期望值: -1)\n", find_mfset(S, 11));
    printf("越界查找 Fix(-1): %d (期望值: -1)\n", fix_mfset(&S, -1));
    printf("越界合并 Merge(0, 1): %s\n", merge_mfset(&S, 0, 1) == ERROR ? "捕获到ERROR" : "失败");
    printf("越界混合 Mix(1, 100): %s\n", mix_mfset(&S, 1, 100) == ERROR ? "捕获到ERROR" : "失败");

    system("pause");
    return 0;
}
