#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"


typedef char TElemType;
typedef enum { Link, Thread } PointerTag;       // Link==0:指针, Thread==1:线索

typedef struct BiThrNode {
    TElemType data;
    struct BiThrNode *lchild, *rchild;          // 左右孩子指针
    PointerTag LTag, RTag;                      // 左右标志
} BiThrNode, *BiThrTree;

BiThrTree pre;          // 全局变量，始终指向刚才访问过的元素(p 的前驱)

// 辅助函数：遍历函数示例 Visit -- 打印
Status Visit(TElemType e) {
    printf("%c ", e);
    return OK;
}

// 算法6.5:双向线索链表的中序遍历非递归算法(不需要栈)
Status InOrderTraverse_Thr(BiThrTree T, Status(*Visit)(TElemType e)) {
    BiThrTree p = T->lchild;                                // p 指向头结点
    while (p != T) {                                        // 空树或 p == T 时退出循环，最后一个结点也被访问
        while (p->LTag == Link) {                           // 有左孩子则让 p 指向其左孩子
            p = p->lchild;
        }
        if (!Visit(p->data)) {                              // 直到没有左孩子则访问输出
            return ERROR;
        }
        while (p->RTag == Thread && p->rchild != T) {       // 访问没有左孩子的结点的后继结点(如果有且不为头结点)
            p = p->rchild;
            Visit(p->data);
        }
        p = p->rchild;
    }
    return OK;
}

// 算法6.7:线索化核心递归函数 -- 在中序遍历的过程中修改空指针建立线索（线索化的核心法则: “当前结点 p 填补前驱 pre 的后继”）
void InThreading(BiThrTree p) {             // 传入的 p 为树的根结点前驱
    if (p) {
        InThreading(p->lchild);             // 左子树线索化
        // 根结点才进行if中的代码(实际上，遍历只对根结点进行操作，叶子结点可以看作只有根结点的树)
        if (!p->lchild) {                   // p 的左孩子为 NULL，则建立前驱线索到 pre(p 线索化它的前驱)
            p->LTag = Thread;
            p->lchild = pre;
        }
        if (!pre->rchild) {                 // pre 的右孩子为 NULL，则建立后继线索到 p (pre 线索化它的后继)
            pre->RTag = Thread;
            pre->rchild = p;
        }
        pre = p;                            // 更新 pre，保持 pre 指向 p 的前驱
        InThreading(p->rchild);             // 右子树线索化
    }
}

// 算法6.6:带头结点的线索化包装函数
Status InOrderThreading(BiThrTree *Thrt, BiThrTree T) {
    // 中序遍历二叉树，并将其中序线索化，Thrt指向头结点
    if (!(*Thrt = (BiThrTree)malloc(sizeof(BiThrNode)))) exit(OVERFLOW);
    // 建头结点
    (*Thrt)->LTag = Link;                   // 头结点 lchild 指向二叉树的根结点
    (*Thrt)->RTag = Thread;                 // 头结点 rchild 最终指向中序遍历访问的最后一个结点
    (*Thrt)->rchild = *Thrt;                // 右指针回指，保证空树 rchild 指向自己，树非空时在 else 中重新赋值, 指向中序遍历访问的最后一个结点
    if (!T) {
        (*Thrt)->lchild = *Thrt;            // 二叉树为空，左指针回指
    } else {                                // 二叉树非空，将 T 全部线索化，并更新头结点 (*Thrt)
        (*Thrt)->lchild = T;
        pre = *Thrt;                        // pre 永远指向刚刚访问过的结点
        InThreading(T);                     // T 线索化(最后一个结点 pre 未线索化)
        // T 最后一个结点线索化
        pre->RTag = Thread;
        pre->rchild = *Thrt;
        // 更新右结点的 rchild 域，使其指向中序遍历访问的最后一个结点
        (*Thrt)->rchild = pre;
    }
    return OK;
}

// ============== 测试区域 ==============

// 辅助函数：快速创建一个普通的二叉树结点
BiThrTree CreateNode(TElemType data) {
    BiThrTree node = (BiThrTree)malloc(sizeof(BiThrNode));
    node->data = data;
    node->lchild = NULL;
    node->rchild = NULL;
    node->LTag = Link;
    node->RTag = Link;
    return node;
}

int main() {
    printf("--- 开始测试线索二叉树 ---\n");

    /* 手动构建一棵普通二叉树用来测试：
             A
           /   \
          B     C
         / \
        D   E
       预期中序遍历序列应为：D B E A C
    */
    BiThrTree T = CreateNode('A');
    T->lchild = CreateNode('B');
    T->rchild = CreateNode('C');
    T->lchild->lchild = CreateNode('D');
    T->lchild->rchild = CreateNode('E');

    // 声明头结点
    BiThrTree Thrt;

    // 1. 进行中序线索化
    printf("1. 正在进行中序线索化...\n");
    InOrderThreading(&Thrt, T);
    
    // 2. 利用线索非递归遍历输出
    printf("2. 非递归中序遍历输出：\n");
    printf(">>> 预期输出结果: D B E A C (Visit函数打印空格)\n");
    printf(">>> 实际输出结果: ");
    InOrderTraverse_Thr(Thrt, Visit);

    printf("\n--- 测试结束 ---\n");
    system("pause");
    return 0;
}