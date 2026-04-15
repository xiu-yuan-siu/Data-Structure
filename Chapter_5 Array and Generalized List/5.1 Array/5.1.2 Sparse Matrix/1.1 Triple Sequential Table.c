#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define MAXSIZE 12500  // 最大非零元素个数

typedef int ElemType;

typedef struct
{
    int i, j;       // 非零元素的 行下标、列下标
    ElemType e;     
} Triple;           // 三元组 (i, j, e)

typedef struct
{
    Triple data[MAXSIZE + 1];   // 非零元三元组表，data[0] 未使用
    int mu, nu, tu;             // 矩阵的 行数、列数、非零元个数
} TSMatrix;                     // Triple Sparse Matrix

Status CreateSMatrix(TSMatrix *M)
{
    if (!M)  // M为空
    {
        return ERROR;  
    }

    printf("请输入矩阵的行数、列数、非零元个数: ");
    scanf("%d %d %d", &M->mu, &M->nu, &M->tu);

    if (M->tu > MAXSIZE)  // 上溢
    {
        return ERROR;
    }

    printf("请按照行序优先顺序按格式 (行 列 值) 输入 %d 个非零元: \n", M->tu);
    for (int k = 1; k <= M->tu; ++k)
    {
        scanf("%d %d %d", &M->data[k].i, &M->data[k].j, &M->data[k].e);
        // 检验 行标是否合法 + 列标是否合法(是否按照行优先顺序 未检验)
        if (M->data[k].i < 1 || M->data[k].i > M->mu || M->data[k].j < 1 || M->data[k].j > M->nu)
        {
            return ERROR;
        }
    }
    return OK;
}

Status DestroySMatrix(TSMatrix *M)  // 逻辑销毁
{
    if (!M)
    {
        return ERROR;
    }
    M->mu = M->nu = M->tu = 0;
    return OK;
}

Status PrintSMatrix(TSMatrix M)
{
    if (!M.tu)  // M 非零元个数为 0
    {
        return ERROR;
    }
    int p = 1;  // 三元组表 一维数组指针
    for (int i = 1; i <= M.mu; ++i)
    {
        for (int j = 1; j <= M.nu; ++j)
        {
            if (p <= M.tu && M.data[p].i == i && M.data[p].j == j)
            {
                printf("%4d", M.data[p].e);
                ++p;
            }
            else
            {
                printf("%4d", 0);
            }
        }
        printf("\n");  // 打印完一行 换行
    }
    return OK;
}

Status CopySMatrix(TSMatrix M, TSMatrix *T)
{
    if (!T)
    {
        return ERROR;
    }
    *T = M;  // 结构体整体赋值
    return OK;
}

Status AddSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
{
    if (!Q || M.mu != N.mu || M.nu != N.nu)  // 两矩阵维度不同
    {
        return ERROR;
    }
    Q->mu = M.mu;
    Q->nu = M.nu;
    int p = 1, q = 1, k = 1;  // M, N, Q 的三元组指针

    // 考虑三种情况
    // 1.(i, j)处 M 有值，N 无值
    // 2.(i, j)处 N 有值，M 无值
    // 3.(i, j)处 M、N 都有值
    while (p <= M.tu && q <= N.tu)
    {
        // 比较时先比较行，再比较列
        // 1.(i, j)处 M 有值，N 无值(M的元素(i, j)更小)
        if (M.data[p].i < N.data[q].i || (M.data[p].i == N.data[q].i && M.data[p].j < N.data[q].j))
        {
            Q->data[k++] = M.data[p++];
        }
        // 2.(i, j)处 N 有值，M 无值(N的元素(i, j)更小)
        else if (M.data[p].i > N.data[q].i || (M.data[p].i == N.data[q].i && M.data[p].j > N.data[q].j))
        {
            Q->data[k++] = N.data[q++];
        }
        // 3.(i, j)处 M、N 都有值
        else
        {
            ElemType sum = M.data[p].e + N.data[q].e;
            if (sum != 0)
            {
                Q->data[k] = M.data[p];     // 赋值 i, j
                Q->data[k].e = sum;         // 赋值 e
                ++k;                        // 只有非零才存入并移动 k
            }
            ++p;
            ++q;
        }
    }
    // 赋值剩余元素
    while (p <= M.tu)
    {
        Q->data[k++] = M.data[p++];
    }
    while (q <= N.tu)
    {
        Q->data[k++] = N.data[q++];
    }
    
    Q->tu = k - 1;  // 最后 k 还进行了一次自增
    return OK;

}

// M + (-N)
Status SubtSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
{
    TSMatrix negN = N;  // negative N
    for (int k = 1; k <= negN.tu; ++k)
    {
        negN.data[k].e = -N.data[k].e;
    }
    return AddSMatrix(M, negN, Q);
}

Status MultiSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
{
    if (!Q || M.nu != N.mu)  // 指针不合法 或 M 的行数 ！= N 的列数
        return ERROR;
    Q->mu = M.mu;
    Q->nu = N.nu;
    Q->tu = 0;  // 初始化为 0，在向 Q 添加非零元素时统计非零元总数

    // 对 M 每一行
    for (int mrow = 1; mrow <= M.mu; ++mrow)
    {
        ElemType ctemp[MAXSIZE + 1] = {0};

        // 遍历 M 第 i 行所有非零元(非零元个数)
        for (int p = 1; p <= M.tu; ++p)
        {
            // 这里没有 带行链接信息的数组 rpos 故而从 1 开始找，并和 mrow 比较 行下标 M.data[p].i
            if (M.data[p].i != mrow)
                continue;  // 跳过本行元素
            int col = M.data[p].j;  // M.data[p].i = mrow 的列下标

            // 遍历 N 中第 col 行的所有非零元(非零元个数)，并相乘累加到 ctemp
            for (int q = 1; q <= N.tu; ++q)
            {
                // 同样的，没有 rpos 这里也要做比较
                if (N.data[q].i == col)
                {
                    int ccol = N.data[q].j;  // 乘积元素在 Q 中的列号
                    ctemp[ccol] += M.data[p].e * N.data[q].e;
                }
            }
        }
        // 判断累加结果是否为非零元，是则 先更新 Q.tu再加入 Q (保持行优先)
        for (int ccol = 1; ccol <= Q->nu; ++ccol)
        {
            if (ctemp[ccol] != 0)   
            {
                Q->data[++Q->tu] = (Triple){mrow, ccol, ctemp[ccol]};
            }
        }
    }
    return OK;
}

// 重点:稀疏矩阵转置(按原矩阵的列序扫描) 时间复杂度 O(nu x tu) 空间复杂度 O(1)，适用于稀疏矩阵 (tu << mu x nu)
// 一般的矩阵(二维数组)转置算法(经典算法): T[col][row] = M[row][col];  时间复杂度 O(mu x nu), 空间复杂度 O(1)
Status TransposeSMatrix(TSMatrix M, TSMatrix *T)
{
    if (!T)
    {
        return ERROR;
    }
    
    T->mu = M.nu;
    T->nu = M.mu;
    T->tu = M.tu;
    if (!T->tu)
    {
        return OK;  // 空矩阵直接返回
    }

    int q = 1;  // T 中当前存放的位置
    // 按原矩阵的列序（即转置后的行序）扫描
    for (int col = 1; col <= M.nu; ++col)  // j
    {
        for (int p = 1; p <= M.tu; ++p)
        {
            if (col == M.data[p].j)  // 找到原矩阵第 col 列的元素
            {
                T->data[q].i = M.data[p].j;
                T->data[q].j = M.data[p].i;
                T->data[q].e = M.data[p].e;
                ++q;
            }
        }
    }
    return OK;
}

// 快速转置算法 时间复杂度 O(nu + tu) 空间复杂度 O(nu)(实际为O(2nu), 只用 cpot 为 O(nu))
Status FastTransposeSMatrix(TSMatrix M, TSMatrix *T)
{
    if (!T)
    {
        return ERROR;
    }

    T->mu = M.nu;
    T->nu = M.mu;
    T->tu = M.tu;
    if (!T->tu)
    {
        return OK;  // 空矩阵直接返回
    }

    // num[col]: 原矩阵第col列的非零元个数
    // cpot[col]: 原矩阵第col列第一个非零元在T.data中的位置

    // 1.统计每列非零元个数 num[col]
    int num[MAXSIZE + 1];
    int cpot[MAXSIZE + 1];
    for (int col = 1; col <= M.nu; ++col)
    {
        num[col] = 0;
    }
    for (int t = 1; t <= M.tu; ++t)
    {
        ++num[M.data[t].j];
    }
    // 2. 计算每列在T中的起始位置
    cpot[1] = 1;  // 第一列的第一个元素的位置一定是 T 中的第一个位置
    for (int col = 2; col <= M.nu; ++col)
    {
        cpot[col] = cpot[col - 1] + num[col - 1]; 
    }
    /* ===================================================
    // 实际上 num 数组没有必要，上述 1、2 部分代码可以简化为:
    for (int t = 1; t <= M.tu; ++t)
    {
        ++cpot[M.data[t].j];
    }
    int next = 1;
    for (int col = 1; col <= M.nu; ++col)
    {
        // 下面三行代码顺序不能交换
        int count = cpot[col];     // 暂存当前列个数，用于 next += count; 求下一个 cpot[col]
        cpot[col] = next;          // cpot[col] 赋值
        next += count;             // 下一个 cpot[col]
    }
    ====================================================== */
    // 3. 一次扫描完成转置
    for (int p = 1; p <= M.tu; ++p)
    {
        int col = M.data[p].j;
        int q = cpot[col];
        T->data[q].i = M.data[p].j;
        T->data[q].j = M.data[p].i;
        T->data[q].e = M.data[p].e;
        ++cpot[col];  // 每一列当前元素赋值后下一次遇到该列，给下一个元素赋值
    }
    return OK;
}

void test_CreateSMatrix() {
    printf("=== 测试 CreateSMatrix ===\n");
    TSMatrix M;
    
    // 用例1: 正常输入 3×4 矩阵，4个非零元
    // 输入: 3 4 4
    //       1 2 3
    //       2 1 5
    //       2 3 1
    //       3 4 2
    CreateSMatrix(&M);
    PrintSMatrix(M);  // 应正确打印稀疏矩阵
    
    // 用例2: 上溢测试 (tu > MAXSIZE)
    // 输入: 1000 1000 12501 → 应返回 ERROR
    
    // 用例3: 下标非法 (行/列越界)
    // 输入: 2 2 1
    //       3 1 10  → 行下标3 > mu=2，应返回 ERROR
}

void test_AddSubSMatrix() {
    printf("=== 测试 AddSMatrix & SubtSMatrix ===\n");
    TSMatrix A, B, C;
    
    // A = [1 0; 0 2], B = [0 3; 4 0]
    // 输入 A: 2 2 2 → (1,1,1) (2,2,2)
    // 输入 B: 2 2 2 → (1,2,3) (2,1,4)
    CreateSMatrix(&A); CreateSMatrix(&B);
    
    AddSMatrix(A, B, &C);  // C = [1 3; 4 2]
    printf("AddSMatrix 结果:\n");
    PrintSMatrix(C);
    printf("\n");
    
    SubtSMatrix(A, B, &C); // C = [1 -3; -4 2]
    printf("SubSMatrix 结果:\n");
    PrintSMatrix(C);
    
    // 边界: 相加抵消为0
    // A: (1,1,5), B: (1,1,-5) → 结果不应包含(1,1)
}

void test_MultiSMatrix() {
    printf("=== 测试 MultiSMatrix (修复后) ===\n");
    TSMatrix A, B, C;
    
    // A(2×3): [1 0 2; 0 3 0]   B(3×2): [0 1; 4 0; 0 2]
    // 期望 C(2×2): [0 5; 12 0]
    // A输入: 2 3 3 → (1,1,1) (1,3,2) (2,2,3)
    // B输入: 3 2 3 → (1,2,1) (2,1,4) (3,2,2)
    CreateSMatrix(&A); CreateSMatrix(&B);
    
    if (MultiSMatrix(A, B, &C) == OK) {
        PrintSMatrix(C);  // 应输出: 0 5 / 12 0
    }
    
    // 错误用例: 维度不匹配 A.nu=3 != B.mu=2 → 应返回 ERROR
}

void test_Transpose() {
    printf("=== 测试转置 ===\n");
    TSMatrix M, T1, T2;
    
    // M(3×4): 非零元 (1,2,3) (2,1,5) (2,3,1) (3,4,2)
    CreateSMatrix(&M);
    
    TransposeSMatrix(M, &T1);      // 普通转置
    FastTransposeSMatrix(M, &T2);  // 快速转置
    
    printf("普通转置结果:\n"); PrintSMatrix(T1);
    printf("快速转置结果:\n"); PrintSMatrix(T2);
    
    // 验证: T1 和 T2 应完全相同，且为 4×3 矩阵
}

void test_CopyDestroy() {
    printf("=== 测试 Copy & Destroy ===\n");
    TSMatrix M, T;
    CreateSMatrix(&M);
    
    CopySMatrix(M, &T);  // 复制
    PrintSMatrix(T);     // 应与 M 相同
    
    DestroySMatrix(&M);  // 逻辑销毁
    if (M.tu == 0) printf("Destroy 成功\n");
}

int main() 
{
    test_CreateSMatrix();
    test_AddSubSMatrix();
    test_MultiSMatrix();   // 重点验证修复后的乘法
    test_Transpose();
    test_CopyDestroy();
    system("pause");
    return 0;
}

/*
=== AddSMatrix 测试输入 ===
# 矩阵 A (2×2, 2个非零元)
2 2 2
1 1 1
2 2 2
# 矩阵 B (2×2, 2个非零元)
2 2 2
1 2 3
2 1 4

=== MultiSMatrix 测试输入 ===
# 矩阵 A (2×3)
2 3 3
1 1 1
1 3 2
2 2 3
# 矩阵 B (3×2)
3 2 3
1 2 1
2 1 4
3 2 2

=== Transpose 测试输入 ===
# 矩阵 M (3×4)
3 4 4
1 2 3
2 1 5
2 3 1
3 4 2
*/