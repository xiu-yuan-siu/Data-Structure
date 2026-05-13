#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

typedef int ElemType;

typedef struct OLNode
{
    int i, j;                       // 该非零元的行下标、列下标
    ElemType e;
    struct OLNode *right, *down;    // 该非零元所在行表、列表的后继链域
} OLNode, *OLink;

typedef struct
{
    // rhead[i] 和 chead[j] 分别指向 第 i 行/第 j 列 的第一个非零元 ，支持 O(1) 定位行列起点
    OLink *rhead, *chead;       // 行、列链表头指针，指向首元素(row head, column head) 本质为 指针数组
    int mu, nu, tu;             // 稀疏矩阵的行数、列数、非零元个数
} CrossList;  // 十字链表

Status CreateSMatrix_OL(CrossList *M)
{
    if (M->rhead)  // 如果 M 已经存在，先释放内部节点 -- ClearSMatrix_OL
    {
        for (int i = 1; i <= M->mu; ++i)  // 逐行逐行销毁
        {
            OLNode *p = M->rhead[i];
            while (p)
            {
                OLNode *q = p;
                p = p->right;
                free(q);
            }
        }
        free(M->rhead);
        free(M->chead);
    }
    // 输入并存储 行数、列数、非零元个数
    int m, n, t;
    printf("请按格式(行数 列数 非零元个数)输入矩阵的行数、列数、非零元个数: ");
    scanf("%d %d %d", &m, &n, &t);
    M->mu = m;
    M->nu = n;
    M->tu = t;
    // 给头指针分配存储空间
    M->rhead = (OLink*)malloc((m + 1) * sizeof(OLink));
    M->chead = (OLink*)malloc((n + 1) * sizeof(OLink));
    if (!M->rhead || !M->chead)
    {
        exit(OVERFLOW);
    }
    // 头指针初始化
    for (int i = 1; i <= m; ++i)
        M->rhead[i] = NULL;
    for (int j = 1; j <= n; ++j)
        M->chead[j] = NULL;
    // 逐个插入非零元
    for (int k = 0; k < t; ++k)  // 0 ~ t-1，t不能等
    {
        int i, j;
        ElemType e;
        scanf("%d %d %d", &i, &j, &e);
        // 为输入值创造结点
        OLNode *p = (OLNode*)malloc(sizeof(OLNode));
        p->i = i;
        p->j = j;
        p->e = e;
        // 接下来对 p 的剩下两个属性初始化，也就是插入十字链表
        // 1.插入行链表(按列号升序) -- 行链表对应 rhead、i、right, p->right
        if (M->rhead[i] == NULL || M->rhead[i]->j > j)  // 当前行为空 或 当前行首元素列号 > 输入的列号，则 p 插入到首元素前面
        {
            p->right = M->rhead[i];
            M->rhead[i] = p;            // 当前行头指针更新，指向 p
        }
        else
        {
            // 先找列号比 j 小的最大列号的结点 -- 即 p 插入位置的前驱
            OLNode *q = M->rhead[i];
            while (q->right && q->right->j < j)  // 找到的 q 满足 q->j < j && q->right->j > j -- 即 q 为 p 的前驱，q->right 为 p 的后继 
                q = q->right;
            // 插入
            p->right = q->right;
            q->right = p;
        }
        // 2.插入列链表(按行号升序) -- 列链表对应 chead、j、down
        if (M->chead[j] == NULL || M->chead[j]->i > i)
        {
            p->down = M->chead[j];
            M->chead[j] = p;            // 更新第 j 列头指针
        }
        else
        {
            // 找前驱
            OLNode *q = M->chead[j];
            while (q->down && q->down->i < i)
            {
                q = q->down;
            }
            // 插入
            p->down = q->down;
            q->down = p;
        }
    }
    return OK;
}

void AddSMatrix(CrossList *A, CrossList *B)  // 原地操作 A = A + B
{
    OLink *hl = (OLink*)malloc((A->nu + 1) * sizeof(OLink));  // 指针数组, 列前驱，hl[j] 指向第 j 列当前位置的列前驱结点，只在插入或者删除结点时才更新
    if (!*hl)
    {
        exit(OVERFLOW);
    }
    // 初始化 hl，初值和列链表的头指针相同
    for (int j = 1; j <= A->nu; ++j)
        hl[j] = A->chead[j];
    // 逐行处理 -- 从 A 的第一行到第 mu 行
    for (int i = 1; i <= A->mu; ++i)
    {
        OLNode *pa = A->rhead[i];  // pa 指向 A 第 i 行的第一个非零元
        OLNode *pb = B->rhead[i];  // pb 指向 B 第 i 行的第一个非零元
        OLNode *pre = NULL;        // 永远指向 pa 的 行 前驱结点
        while (pb != NULL)  // 依次处理 B 本行结点，直至 B 的本行中无非零元结点
        {
            if (pa == NULL || pa->j > pb->j)  // 情况3：A中该行已无元素，或A中元素列号 > B中元素列号, 则在 pa 前插入一个值为pb->e的新结点，此时 pa 不动，pa 的前驱 pre 更新到新节点, 插入节点后 pb 右移
            {
                // 创建新节点 p
                OLNode *p = (OLNode*)malloc(sizeof(OLNode));
                p->i = i;
                p->j = pb->j;
                p->e = pb->e;
                // 插入行链表
                if (pre == NULL)  // 无前驱(pa 指向第一个结点，不是该行只有一个结点)，则插入在首元素的位置 p->right = pa
                {
                    A->rhead[p->i] = p;
                }
                else
                {
                    // p->right = pre->right;  // 两种情况都要移动 p 直接在分支结构外对 p->right 进行赋值
                    pre->right = p;
                }
                p->right = pa;  //  == p->right = pre->right
                pre = p;        // 更新 pre，pa 不用变，因为 p 插入在 pa 的前面
                // 插入列链表
                if (A->chead[p->j] == NULL || A->chead[p->j]->i > p->i)  // 该列只有无结点 或 首节点节点行号比插入结点大(不是只有一个结点)，则将 p 插入首位置
                {
                    p->down = A->chead[p->j];
                    A->chead[p->j] = p;
                }
                else
                {
                    // hl[pb->j]为行号比 p->i(即 i) 小的第 p->j 列的最后一个结点
                    p->down = hl[p->j]->down;
                    hl[p->j]->down = p;
                }
                // 更新 hl[p->j] -- 新插入的结点 p 成为了第 p->j 列行号 <= i 的最后一个节点
                hl[p->j] = p;
                ++A->tu;            // 更新 A 中非零元个数
                pb = pb->right;     // pb 右移
            }
            else if (pa->j < pb->j)  // 情况2: pa != NULL 且A中元素列号 < B中元素列号，则A在该行继续向右找是否有行号相等的情况，即 pa 右移，pb 不动
            {
                pre = pa;
                pa = pa->right;
            }
            else  // 情况3+4: pa != NULL 且A中元素列号 = B中元素列号，判断所对应的元素之和是否为 0
            {
                ElemType sum = pa->e + pb->e;
                if (sum != 0)  // 情况3: pa != NULL && A中元素列号 = B中元素列号 && pa->e + pb->e != 0, 则将 sum 值送到 pa 所指结点的 e 域，其他所有域均不变，但 pre、pa、pb 均要更新
                {
                    pa->e = sum;
                    // 更新 pre、pa、pb
                    pre = pa;
                    pa = pa->right;
                    pb = pb->right;
                }
                else  // 情况3: pa != NULL && A中元素列号 = B中元素列号 && pa->e + pb->e == 0，则删除 pa 所指结点
                {
                    OLNode *p = pa;  // p 指向要删除的结点
                    // 行链表删除
                    if (pre == NULL)  // 等价于 A->rhead[p->i] == pa/p 是 行链表的第一个结点
                    {
                        A->rhead[p->i] = pa->right;
                        // pa = pa->right;  // 在外面统一右移
                    }
                    else
                    {
                        pre->right = pa->right;
                    }
                    pa = pa->right;  // 两个分支统一 pa 右移
                    // 列链表删除
                    if (A->chead[p->j] == p)  // p 是列链表的第一个结点(这里只能用 p 不能用 pa，因为上面 pa 已经右移了)
                    {
                        A->chead[p->j] = p->down;
                        // 更新 hl[p->j] 为新的头结点
                        hl[p->j] = A->chead[p->j];
                    }
                    else
                    {
                        // 用hl[p->j] 作为列前驱进行删除，同时更新了 hl[p->j] 
                        hl[p->j]->down = p->down;
                    }
                    free(p);            // 释放 p 的空间
                    --A->tu;            // 更新A中非零元的个数
                    pb = pb->right;     // pb 右移
                }
            }
        }
    }
    free(hl);  // 释放辅助数组
}

// 打印矩阵（用于测试）
void PrintSMatrix(CrossList M)
{
    printf("\n稀疏矩阵（%d行 × %d列，%d个非零元）：\n", M.mu, M.nu, M.tu);
    printf("行号  列号  值\n");
    for (int i = 1; i <= M.mu; ++i)
    {
        OLNode *p = M.rhead[i];
        while (p)
        {
            printf("%3d   %3d   %d\n", p->i, p->j, p->e);
            p = p->right;
        }
    }
}

// 以稠密形式打印（便于验证）
void PrintDenseMatrix(CrossList M)
{
    printf("\n稠密矩阵形式：\n");
    for (int i = 1; i <= M.mu; ++i)
    {
        OLNode *p = M.rhead[i];
        for (int j = 1; j <= M.nu; ++j)
        {
            if (p && p->j == j)
            {
                printf("%4d ", p->e);
                p = p->right;
            }
            else
            {
                printf("%4d ", 0);
            }
        }
        printf("\n");
    }
}

int main()
{
    CrossList A = {NULL, NULL, 0, 0, 0};
    CrossList B = {NULL, NULL, 0, 0, 0};
    
    printf("===== 测试1：基本加法（修改值 + 删除零元素） =====\n");
    printf("矩阵A：3行3列，3个非零元\n");
    printf("输入：3 3 3\n");
    printf("      1 1 1\n");
    printf("      1 2 2\n");
    printf("      2 2 3\n");
    printf("矩阵B：3行3列，2个非零元\n");
    printf("输入：3 3 2\n");
    printf("      1 1 4\n");
    printf("      2 2 -3\n");
    printf("预期：A[1,1]=5, A[1,2]=2, A[2,2]被删除\n");
    
    printf("\n--- 输入矩阵A ---\n");
    CreateSMatrix_OL(&A);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n--- 输入矩阵B ---\n");
    CreateSMatrix_OL(&B);
    PrintSMatrix(B);
    PrintDenseMatrix(B);
    
    printf("\n--- A = A + B ---\n");
    AddSMatrix(&A, &B);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n===== 测试2：插入新元素（B中有A没有的元素） =====\n");
    printf("矩阵A：2行2列，1个非零元\n");
    printf("输入：2 2 1\n");
    printf("      1 1 5\n");
    printf("矩阵B：2行2列，1个非零元\n");
    printf("输入：2 2 1\n");
    printf("      2 2 8\n");
    printf("预期：A[1,1]=5, 新增A[2,2]=8\n");
    
    printf("\n--- 输入矩阵A ---\n");
    CreateSMatrix_OL(&A);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n--- 输入矩阵B ---\n");
    CreateSMatrix_OL(&B);
    PrintSMatrix(B);
    PrintDenseMatrix(B);
    
    printf("\n--- A = A + B ---\n");
    AddSMatrix(&A, &B);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n===== 测试3：全部相消为零 =====\n");
    printf("矩阵A：2行2列，2个非零元\n");
    printf("输入：2 2 2\n");
    printf("      1 1 10\n");
    printf("      2 2 20\n");
    printf("矩阵B：2行2列，2个非零元\n");
    printf("输入：2 2 2\n");
    printf("      1 1 -10\n");
    printf("      2 2 -20\n");
    printf("预期：A变为空矩阵（无非零元）\n");
    
    printf("\n--- 输入矩阵A ---\n");
    CreateSMatrix_OL(&A);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n--- 输入矩阵B ---\n");
    CreateSMatrix_OL(&B);
    PrintSMatrix(B);
    PrintDenseMatrix(B);
    
    printf("\n--- A = A + B ---\n");
    AddSMatrix(&A, &B);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n===== 测试4：插入到空行（第1行原本无元素） =====\n");
    printf("矩阵A：3行3列，1个非零元（只在第2行）\n");
    printf("输入：3 3 1\n");
    printf("      2 2 5\n");
    printf("矩阵B：3行3列，1个非零元（在第1行）\n");
    printf("输入：3 3 1\n");
    printf("      1 1 3\n");
    printf("预期：A[1,1]=3, A[2,2]=5\n");
    
    printf("\n--- 输入矩阵A ---\n");
    CreateSMatrix_OL(&A);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n--- 输入矩阵B ---\n");
    CreateSMatrix_OL(&B);
    PrintSMatrix(B);
    PrintDenseMatrix(B);
    
    printf("\n--- A = A + B ---\n");
    AddSMatrix(&A, &B);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n===== 测试5：多元素插入与混合操作 =====\n");
    printf("矩阵A：3行3列，2个非零元\n");
    printf("输入：3 3 2\n");
    printf("      1 1 1\n");
    printf("      3 3 1\n");
    printf("矩阵B：3行3列，3个非零元\n");
    printf("输入：3 3 3\n");
    printf("      1 2 2\n");
    printf("      2 2 3\n");
    printf("      3 3 -1\n");
    printf("预期：A[1,1]=1, 新增A[1,2]=2, 新增A[2,2]=3, A[3,3]被删除\n");
    
    printf("\n--- 输入矩阵A ---\n");
    CreateSMatrix_OL(&A);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n--- 输入矩阵B ---\n");
    CreateSMatrix_OL(&B);
    PrintSMatrix(B);
    PrintDenseMatrix(B);
    
    printf("\n--- A = A + B ---\n");
    AddSMatrix(&A, &B);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n===== 测试6：同行多元素，测试指针移动 =====\n");
    printf("矩阵A：2行3列，2个非零元\n");
    printf("输入：2 3 2\n");
    printf("      1 1 1\n");
    printf("      1 3 3\n");
    printf("矩阵B：2行3列，2个非零元（插入到A的中间）\n");
    printf("输入：2 3 2\n");
    printf("      1 2 2\n");
    printf("      2 1 4\n");
    printf("预期：A[1,1]=1, A[1,2]=2, A[1,3]=3, 新增A[2,1]=4\n");
    
    printf("\n--- 输入矩阵A ---\n");
    CreateSMatrix_OL(&A);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n--- 输入矩阵B ---\n");
    CreateSMatrix_OL(&B);
    PrintSMatrix(B);
    PrintDenseMatrix(B);
    
    printf("\n--- A = A + B ---\n");
    AddSMatrix(&A, &B);
    PrintSMatrix(A);
    PrintDenseMatrix(A);
    
    printf("\n所有测试完成！\n");
    
    // system("pause");
    return 0;
}