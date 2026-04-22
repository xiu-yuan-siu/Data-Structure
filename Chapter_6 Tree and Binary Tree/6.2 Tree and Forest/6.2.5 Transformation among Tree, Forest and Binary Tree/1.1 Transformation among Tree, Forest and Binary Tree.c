#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

typedef char TElemType;

// 树、森林的每一棵树都使用孩子-兄弟表示法(二叉树表示法) -- 逻辑上等同于 二叉树
typedef struct CSNode {
    TElemType data;
    struct CSNode *firstchild;      // 树的第一个孩子 -- 对应二叉树的左孩子
    struct CSNode *nextsibling;     // 树的下一个兄弟 -- 对应二叉树的右孩子
} CSNode, *CSTree, *BiTree;

/*
* 1.对于树而言，使用孩子-兄弟表示法表示就已经是二叉树了 -- 即 已经实现了树和二叉树之间的转换
* 2.由于树=二叉树，所以树的先根遍历、后根遍历 = 二叉树的先序遍历、中序遍历，故不做实现
*/

// 下面实现 森林 和 二叉树之间的转换
// 森林 -> 二叉树：森林中第一棵树的根作为二叉树的根；该树的子树森林变为左子树；森林中剩余的树变为右子树
BiTree ForestToBinaryTree(CSNode* trees[], int count) {
    if (count <= 0) return NULL;
    // 森林中的第一棵树作为二叉树的根结点
    BiTree root = trees[0];
    // 递归将后序的树连在当前根结点的右兄弟上
    if (count == 1) {                       // 森林只有一棵树
        root->nextsibling = NULL;
    } else {                                // count > 1
        root->nextsibling = ForestToBinaryTree(&trees[1], count - 1);
    }
    return root;
}

// 辅助函数: 递归打印以 T 为根结点的树(后根遍历 -- 对应二叉树的中序遍历)
void PrintTree(CSTree T, int level) {
    if (!T) return;

    CSTree p = T;
    // 1.先访问第一棵子树(相当于二叉树的左孩子)
    PrintTree(p->firstchild, level + 1);    // 这里 level + 1 因为第一棵子树在根结点下一层
    // 2.再访问根结点 
    printf("[%d] %c\n", level, p->data);      // 显示层级
    // 3.最后访问其他其他子树(相当于二叉树的右孩子)
    PrintTree(p->nextsibling, level);       // 右孩子和左孩子在同一层
}

// 二叉树 -> 森林: 将二叉树根结点的右孩子及其右链断开，每一段作为一个独立的树根，再递归还原其子树
void BinaryTreeToForest(BiTree T) {         // // 将二叉树还原为森林（通过遍历打印或重新构建逻辑结构）
    if (!T) return;
    CSNode *p = T;
    int treeCount = 1;
    while (p) {
        printf("第 %d 棵树的根结点: [%d] %c\n", treeCount++, 0, p->data);

        // 沿左链找该树的孩子
        if (p->firstchild) {
            printf("  结点 %c 的子树(后根遍历展示):\n", p->data);
            PrintTree(p->firstchild, 1);        // 传入 p->firstchild（只打印当前树的子树），并将初始层级固定为 1
        } else {
            printf("  结点 %c 无子树\n", p->data);
        }

        // 沿右链遍历其他树
        p = p->nextsibling;
    }
}

// 创建结点辅助函数
CSNode* CreateNode(char data) {
    CSNode* node = (CSNode*)malloc(sizeof(CSNode));
    node->data = data;
    node->firstchild = NULL;
    node->nextsibling = NULL;
    return node;
}

int main() {
    // --- 第一棵树 (A) ---
    // A -> B -> C -> D (A是根，B,C,D是兄弟)
    // B -> E (E是B的孩子)
    CSNode* A = CreateNode('A');
    CSNode* B = CreateNode('B');
    CSNode* C = CreateNode('C');
    CSNode* D = CreateNode('D');
    CSNode* E = CreateNode('E');
    A->firstchild = B;
    B->nextsibling = C;
    C->nextsibling = D;
    B->firstchild = E;

    // --- 第二棵树 (F) ---
    // F -> G (G是F的孩子)
    // G -> H -> I (H,I是G的孩子)
    CSNode* F = CreateNode('F');
    CSNode* G = CreateNode('G');
    CSNode* H = CreateNode('H');
    CSNode* I = CreateNode('I');
    F->firstchild = G;
    G->firstchild = H;
    H->nextsibling = I;

    // --- 第三棵树 (J) ---
    CSNode* J = CreateNode('J');

    // 1. 将三棵树组成森林并转换为二叉树
    CSNode* forest[] = {A, F, J};
    BiTree root = ForestToBinaryTree(forest, 3);

    printf("===== 森林转换为二叉树测试 =====\n");
    printf("二叉树根结点 (原树1根): %c\n", root->data);
    printf("二叉树右孩子 (原树2根): %c\n", root->nextsibling->data);
    printf("二叉树右孩子的右孩子 (原树3根): %c\n", root->nextsibling->nextsibling->data);

    // 2. 二叉树还原展示
    printf("\n===== 还原森林并进行后根遍历 (对应二叉树中序) =====\n");
    BinaryTreeToForest(root);

    printf("\n测试完成。\n");
    system("pause");
    return 0;
}