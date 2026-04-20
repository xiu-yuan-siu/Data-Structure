#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

typedef char TElemType;                 // 数据元素类型，根据需求修改

typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild;    // 左右孩子指针
} BiTNode, *BiTree;

// 1. 构造空二叉树
Status InitBiTree(BiTree *T)
{
    *T = NULL;
    return OK;
}

// 2.销毁
Status DestroyBiTree(BiTree *T)
{
    if (*T)  // 头指针不为空
    {
        if ((*T)->lchild)  // 左子树不为空，则递归销毁左子树
            DestroyBiTree(&((*T)->lchild));
        if ((*T)->rchild)
            DestroyBiTree(&((*T)->rchild));
        free(*T);   // 释放头指针(存根结点的地址，但是不存数据)
        *T = NULL;
    }
    return OK;
}

// 辅助函数：创建新结点
BiTree CreateNode(TElemType e)
{
    BiTNode *p = (BiTNode*)malloc(sizeof(BiTNode));
    if (!p) exit(OVERFLOW);
    p->data = e;
    p->lchild = p->rchild = NULL;
    return p;
}

// '#' 表示空结点，definition是数组，如"ABD##E##CF###"，传入 index 参数在内部管理，不是对外接口，是为了便于递归构造二叉树的
Status CreateBiTreeInternal(BiTree *T, const TElemType definition[], int *index)
{
    TElemType ch = definition[*index];
    ++(*index);     // 按先序创建下一个结点
    if (ch == '#') 
    {
        *T = NULL;
    } 
    else 
    {
        *T = (BiTNode*)malloc(sizeof(BiTNode));
        if (!(*T)) exit(OVERFLOW);
        (*T)->data = ch;    // 头指针存根结点的地址
        // 递归调用创建左子树和右子树
        CreateBiTreeInternal(&((*T)->lchild), definition, index);
        CreateBiTreeInternal(&((*T)->rchild), definition, index);
    }
    return OK;
}

// // 3.按先序序列(最常用)构造二叉树 -- 与书中的格式相同，无对外接口 index
Status CreateBiTree(BiTree *T, const TElemType definition[])
{
    int index = 0;
    return CreateBiTreeInternal(T, definition, &index);
}

// 4.清空
Status ClearBiTree(BiTree *T)
{
    if (*T)  // 头指针不为空
    {
        if ((*T)->lchild)  // 左子树不为空，则递归销毁左子树
            DestroyBiTree(&((*T)->lchild));
        if ((*T)->rchild)
            DestroyBiTree(&((*T)->rchild));
        free(*T);   // 释放头指针(存根结点的地址，但是不存数据)
        *T = NULL;
    }
    return OK;
}

// 5.判空
Status BiTreeEmpty(BiTree T)
{
    if (T == NULL)
        return TRUE;
    else
        return FALSE;
}

// 6.深度
int BiTreeDepth(BiTree T)
{
    int depth, depthLeft, depthRight;
    if (T == NULL)
        return 0;       // 空二叉树深度为 0
    else                // 二叉树非空，则比较左子树和右子树的深度返回二者中的较大值 + 1(根结点)
    {
        depthLeft = BiTreeDepth(T->lchild);
        depthRight = BiTreeDepth(T->rchild);
        depth = (depthLeft > depthRight ? depthLeft : depthRight) + 1;
        return depth;
    }
}

// 7.返回根
TElemType Root(BiTree T)
{
    if (T == NULL)
        return '#';     // '#' 表示空结点
    return T->data;
}

/*
* 下面的一些函数要判断 e 是否是 T 中的结点，我只做两个重载示例(返回错误码, 函数8和函数10)，其他不做判断，默认传入正确的参数
*/

// 辅助函数：查找 e 是否是 T 的结点(函数 8、9、10、11、12、13、14)
// 用于重载的返回错误码的函数(重载函数8, 函数9不需要重载)
Status FindNode(BiTree T, BiTree e)
{
    if (T == NULL || e == NULL) return FALSE;
    if (T == e) return TRUE;
    return FindNode(T->lchild, e) || FindNode(T->rchild, e);
}

// 8.返回结点 e 的值(重载) -- 这里比书中多一个参数，不直接返回 TElemType 因为还要求二叉树T存在和e是T的结点，要返回错误码，下同
Status ValueOverload(BiTree T, BiTree e, TElemType *result)
{
    if (T == NULL || e == NULL)
        return ERROR;  
    if (!FindNode(T, e))  // e 不是 T 的结点，也返回错误码
        return ERROR;
    *result = e->data;
    return OK;
}

// 8.返回结点 e 的值
TElemType Value(BiTree T, BiTree e)
{
    if (T == NULL) return '#';
    return e->data;
} 

// 9.给结点 e 赋值为 value
Status Assign(BiTree T, BiTree e, TElemType value)
{
    if (T == NULL || e == NULL) return ERROR;
    if (!FindNode(T, e)) return ERROR;
    e->data = value;
    return OK;
}

// 10.返回 e 的双亲
BiTree Parent(BiTree T, BiTree e)
{
    if (T == NULL || e == NULL || T == e) return NULL;
    BiTree parent = NULL;
    // e 的双亲为根结点
    if (T->lchild == e || T->rchild == e)
        return T;
    // 递归左子树查找双亲
    BiTree p = Parent(T->lchild, e);
    if (p != NULL) return p;
    // 递归右子树查找双亲
    return Parent(T->rchild, e);  // 这里没找到直接返回 NULL
}

// 10.返回 e 的双亲(重载)
Status ParentOverload(BiTree T, BiTree e, BiTree *parent) {
    if (T == NULL || e == NULL || T == e) return ERROR;
    *parent = Parent(T, e);
    return (*parent != NULL) ? OK : ERROR;
}

// 11.返回 e 的左孩子
BiTree LeftChild(BiTree T, BiTree e)
{
    // 以下三种情况返回空：空树、e为空结点、e无左孩子
    if (T == NULL || e == NULL || e->lchild == NULL) return NULL;
    return e->lchild;
}

// 12.返回 e 的左孩子
BiTree RightChild(BiTree T, BiTree e)
{
    if (T == NULL || e == NULL || e->rchild == NULL) return NULL;
    return e->rchild;
}

// 13.返回 e 的左兄弟
BiTree LeftSibling(BiTree T, BiTree e)
{
    if (T == NULL || e == NULL || T == e) return NULL;
    // 找 e 的双亲
    BiTree p = Parent(T, e);
    if (!p) return NULL;  // 无双亲则无左兄弟
    // 只有 e 是右孩子，且有左兄弟时，才可以返回 e 的左兄弟
    if (p->rchild == e && p->lchild != NULL)
        return p->lchild;
    // p->rchild == e && p->lchild != NULL 的补集均空返回
    return NULL;
}

// 14.返回 e 的右兄弟
BiTree RightSibling(BiTree T, BiTree e)
{
    if (T == NULL || e == NULL || T == e) return NULL;
    BiTree p = Parent(T, e);
    if (!p) return NULL;
    if (p->lchild == e && p->rchild != NULL)
        return p->rchild;
    return NULL;
}

// 15.插入子树
// LR = 0：插入左子树；LR = 1：插入右子树
// c 与 T 不相交且右子树为空，p 所指结点的原有左或右子树成为 c 的右子树
Status InsertChild(BiTree T, BiTree p, int LR, BiTree c)
{
    if (!T || !p || !c) return ERROR;
    if (LR == 0)
    {
        c->rchild = p->lchild;
        p->lchild = c;
    }
    else if (LR == 1)
    {
        c->rchild = p->rchild;
        p->rchild = c;
    }
    else 
    {
        return ERROR;  // LR 不是 0 或 1
    }
    return OK;
}

// 16.删除子树
// LR = 0：删除左子树；LR = 1：删除右子树
Status DeleteChild(BiTree T, BiTree p, int LR)
{
    if (!T || !p) return ERROR;
    if (LR == 0)
        DestroyBiTree(&(p->lchild));  // 销毁中有头指针置空
    else if (LR == 1)
        DestroyBiTree(&(p->rchild));
    else 
        return ERROR;
    return OK;
}

// 辅助函数：visit函数示例 -- PrintElement
Status Visit(TElemType e)
{
    printf("%c ", e);
    return OK;
}

/*
* 先序、中序、后序遍历均适用递归工作栈(压入弹出工作记录)实现，层序遍历通过队列实现
*/

// 17.先序遍历
Status PreOrderTraverse(BiTree T, Status(*Visit)(TElemType))
{
    if (T)  // T 不为空树
    {
        if (Visit(T->data))                                 // 先访问根节点
            if (PreOrderTraverse(T->lchild, Visit))         // 再递归访问左子树
                if (PreOrderTraverse(T->rchild, Visit))     // 最后递归访问右子树
                    return OK;  // 访问完成
        return ERROR;  // 三个if访问任一结点访问失败则沿递归栈向上回传，返回 ERROR
    }
    else
        return OK;  // T 为空树
}

// 18.中序遍历
Status InOrderTraverse(BiTree T, Status(*Visit)(TElemType))
{
    if (T)
    {
        if (InOrderTraverse(T->lchild, Visit))          // 先访问左子树
            if (Visit(T->data))                         // 再访问根节点
                if (InOrderTraverse(T->rchild, Visit))  // 最后访问右子树
                    return OK;
        return ERROR;
    }
    else 
        return OK;
}

// 19.后序遍历
Status PostOrderTraverse(BiTree T, Status(*Visit)(TElemType))
{
    if (T)
    {
        if (PostOrderTraverse(T->lchild, Visit))        // 先左子树
            if (PostOrderTraverse(T->rchild, Visit))    // 再右子树
                if (Visit(T->data))                     // 最后根节点
                    return OK;
        return ERROR;
    }
    else
        return OK;
}

// 20.层序遍历 -- 队列实现
Status LevelOrderTraverse(BiTree T, Status(*Visit)(TElemType))
{
    if (T == NULL) return ERROR;

    // 使用队列实现层序遍历
    #define MAX_QUEUE_SIZE 100
    BiTree queue[MAX_QUEUE_SIZE];   // 二叉树队列
    int front = 0, rear = 0;        // 头尾指针
    queue[rear++] = T;              // queue[0] = T，并更新尾指针 rear = 1
    while (front != rear)
    {
        BiTree p = queue[front++];
        if (!Visit(p->data)) return ERROR;
        if (p->lchild) queue[rear++] = p->lchild;
        if (p->rchild) queue[rear++] = p->rchild;
    }
    return OK;
}

// 打印二叉树图形（侧向）
// depth 初始传 0
void PrintTree(BiTree T, int depth) {
    if (T == NULL) {
        return;
    }

    // 先递归处理右子树（在图形上方）
    PrintTree(T->rchild, depth + 1);

    // 打印当前节点，根据深度缩进
    for (int i = 0; i < depth; i++) {
        printf("    "); // 每一层缩进 4 个空格
    }
    printf("%c\n", T->data);

    // 再递归处理左子树（在图形下方）
    PrintTree(T->lchild, depth + 1);
}

// ============== 运行测试 ==============
int main() {
    BiTree T;
    InitBiTree(&T);
    
    // 构造一棵测试用的树: 
    //        A
    //       / \
    //      B   C
    //     / \  /
    //    D   E F
    const char* definition = "ABD##E##CF###";
    printf("1. 开始按先序字符串 [%s] 构造二叉树...\n", definition);
    CreateBiTree(&T, definition);
    
    printf("\n2. 测试基本属性：\n");
    printf("树是否为空? %s\n", BiTreeEmpty(T) ? "是" : "否");
    printf("树的深度: %d\n", BiTreeDepth(T));
    printf("树的根节点: %c\n", Root(T));

    printf("\n3. 测试遍历功能：\n");
    printf("\n7. 树的逻辑结构可视化（侧向看）：\n");
    PrintTree(T, 0);
    printf("先序遍历 (期望: A B D E C F ): ");
    PreOrderTraverse(T, Visit); printf("\n");
    
    printf("中序遍历 (期望: D B E A F C ): ");
    InOrderTraverse(T, Visit); printf("\n");
    
    printf("后序遍历 (期望: D E B F C A ): ");
    PostOrderTraverse(T, Visit); printf("\n");
    
    printf("层序遍历 (期望: A B C D E F ): ");
    LevelOrderTraverse(T, Visit); printf("\n");

    printf("\n4. 测试节点关系查询：\n");
    BiTree nodeB = T->lchild;
    BiTree nodeC = T->rchild;
    BiTree nodeE = nodeB->rchild;
    printf("E的父节点是: %c (期望: B)\n", Value(T, Parent(T, nodeE)));
    printf("E的左兄弟是: %c (期望: D)\n", Value(T, LeftSibling(T, nodeE)));
    printf("B的右孩子是: %c (期望: E)\n", Value(T, RightChild(T, nodeB)));

    printf("\n5. 测试树的操作 (InsertChild & DeleteChild)：\n");
    // 构建一棵只有单节点 G 的树 c
    BiTree c = CreateNode('G');
    // 把 G 插入为 C 的左子树（原 C 的左子树 F 将变成 G 的右子树）
    InsertChild(T, nodeC, 0, c);
    printf("在 C 的左孩子位置插入子树 G 后，先序遍历 (期望: A B D E C G F ): ");
    PreOrderTraverse(T, Visit); printf("\n");

    // 删除 B 的左子树 (D)
    DeleteChild(T, nodeB, 0);
    printf("删除 B 的左子树后，层序遍历 (期望: A B C E G F ): ");
    LevelOrderTraverse(T, Visit); printf("\n");

    printf("\n6. 销毁树...\n");
    DestroyBiTree(&T);
    printf("树是否已被销毁? %s (期望: 是)\n", BiTreeEmpty(T) ? "是" : "否");

    system("pause");
    return 0;
}