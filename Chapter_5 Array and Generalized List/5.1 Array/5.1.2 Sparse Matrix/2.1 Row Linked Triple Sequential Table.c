#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define MAXSIZE 12500  // 最大非零元素个数
#define MAXRC 500      // 三元组表矩阵允许的最大行数，Maximum Row Count 

typedef int ElemType;

typedef struct
{
    int i, j;       // 非零元素的 行下标、列下标
    ElemType e;     
} Triple;           // 三元组 (i, j, e)

typedef struct
{
    Triple data[MAXSIZE + 1];   // 非零元三元组表，data[0] 未使用
    int rpos[MAXRC + 1];        // 各行第一个非零元的位置表
    int mu, nu, tu;             // 矩阵的 行数、列数、非零元个数
} RLSMatrix;                     // Row Linked Sparse Matrix

Status CreateSMatrix(RLSMatrix *M)
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
    // 构建 rpos[]: 统计每行首个非零元位置
    int num[MAXRC + 1] = {0};
    for (int t = 1; t <= M->tu; ++t)
    {
        ++num[M->data[t].i];
    }
    M->rpos[1] = 1;
    for (int row = 2; row <= M->mu; ++row)
    {
        M->rpos[row] = M->rpos[row - 1] + num[row - 1];
    }
    return OK;
}

Status DestroySMatrix(RLSMatrix *M)  // 逻辑销毁
{
    if (!M)
    {
        return ERROR;
    }
    M->mu = M->nu = M->tu = 0;
    return OK;
}

Status PrintSMatrix(RLSMatrix M)
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

Status CopySMatrix(RLSMatrix M, RLSMatrix *T)
{
    if (!T)
    {
        return ERROR;
    }
    *T = M;  // 结构体整体赋值
    return OK;
}

Status AddSMatrix(RLSMatrix M, RLSMatrix N, RLSMatrix *Q)
{
    if (!Q) 
    {
        printf("Q is NULL!\n");
        return ERROR;
    }
    if (M.mu != N.mu || M.nu != N.nu)  // 两矩阵维度不同
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
                ++k;
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

Status SubtSMatrix(RLSMatrix M, RLSMatrix N, RLSMatrix *Q)
{
    if (!Q) return ERROR;
    if (M.mu != N.mu || M.nu != N.nu) return ERROR;
    
    Q->mu = M.mu;
    Q->nu = M.nu;
    int p = 1, q = 1, k = 1;

    while (p <= M.tu && q <= N.tu)
    {
        // M 的元素更小，直接复制
        if (M.data[p].i < N.data[q].i || 
            (M.data[p].i == N.data[q].i && M.data[p].j < N.data[q].j))
        {
            Q->data[k++] = M.data[p++];
        }
        // N 的元素更小，取负后复制
        else if (M.data[p].i > N.data[q].i || 
                 (M.data[p].i == N.data[q].i && M.data[p].j > N.data[q].j))
        {
            Q->data[k] = N.data[q];
            Q->data[k].e = -N.data[q].e;  // 取负
            ++k;
            ++q;
        }
        // 同一位置，相减
        else
        {
            ElemType diff = M.data[p].e - N.data[q].e;
            if (diff != 0)
            {
                Q->data[k] = M.data[p];
                Q->data[k].e = diff;
                ++k;
            }
            ++p;
            ++q;
        }
    }
    
    // 剩余元素
    while (p <= M.tu)
    {
        Q->data[k++] = M.data[p++];
    }
    while (q <= N.tu)
    {
        Q->data[k] = N.data[q];
        Q->data[k].e = -N.data[q].e;  // 取负
        ++k;
        ++q;
    }
    
    Q->tu = k - 1;
    return OK;
}

Status MultiSMatrix(RLSMatrix M, RLSMatrix N, RLSMatrix *Q)
{
    if (!Q || M.nu != N.mu)  // 指针不合法 或 M的列数 != N 的行数
    {
        return ERROR;
    }
    Q->mu = M.mu;
    Q->nu = N.nu;
    Q->tu = 0;
    if (M.tu * N.tu == 0)  // 任一为 0，则 Q 为 0, 直接返回
    {
        return OK;
    }
    for (int arow = 1; arow <= M.mu; ++arow)  // a_row
    {
        // 1. 初始化累加器
        int ctemp[MAXRC + 1] = {0};
        Q->rpos[arow] = Q->tu + 1;  // 记录 Q 当前行首个非零元在 Q.data 中的位置
        // 2.找 M 第 arow 行非零元区间末尾(不能等) tp
        int tp;
        if (arow < M.mu)
            // 第 arow 行最后一个非零元在 N.data 中的序号为 M.rpos[arow + 1] - 1, 这里 tp 不能等，所以直接让 tp = M.rpos[arow + 1]
            tp = M.rpos[arow + 1];  
        else 
            tp = M.tu + 1;  // 最后一行特殊处理，直接以 tu+1 为界
        // 3.遍历 M 第 arow 行的非零元
        for (int p = M.rpos[arow]; p < tp; ++p)  // [p, tp)
        {
            int brow = M.data[p].j;  // b_row N 的行号 = M 的列号
            // 4.找 N 第 brow 行非零元区间末尾(不能等) t
            int t;
            if (brow < N.mu)
                t = N.rpos[brow + 1];
            else
                t = N.tu + 1;
            // 5.相乘并累加求和
            for (int q = N.rpos[brow]; q < t; ++q)  // [q, t)
            {
                int ccol = N.data[q].j;
                ctemp[ccol] += M.data[p].e * N.data[q].e;
            }
        }
        // 6.压缩储存该行非零元
        for (int ccol = 1; ccol <= Q->nu; ++ccol)  // 
        {
            if (ctemp[ccol])  // 该行累加和不为 0
            {
                if (++Q->tu > MAXSIZE)  // Q->tu 先++，下面再赋值
                    return ERROR;
                Q->data[Q->tu].i = arow;
                Q->data[Q->tu].j = ccol;
                Q->data[Q->tu].e = ctemp[ccol];
            }
        }
    }
    return OK;
}

// 重点:稀疏矩阵转置(按原矩阵的列序扫描) 时间复杂度 O(nu x tu) 空间复杂度 O(1)，适用于稀疏矩阵 (tu << mu x nu)
// 一般的矩阵(二维数组)转置算法(经典算法): T[col][row] = M[row][col];  时间复杂度 O(mu x nu), 空间复杂度 O(1)
Status TransposeSMatrix(RLSMatrix M, RLSMatrix *T)
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

// 快速转置算法 时间复杂度 O(nu + tu) 空间复杂度 O(nu)(实际为O(2nu), 只用 copt 为 O(nu))
Status FastTransposeSMatrix(RLSMatrix M, RLSMatrix *T)
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
    // copt[col]: 原矩阵第col列第一个非零元在T.data中的位置

    // 1.统计每列非零元个数 num[col]
    int num[MAXSIZE + 1];
    int copt[MAXSIZE + 1];
    for (int col = 1; col <= M.nu; ++col)
    {
        num[col] = 0;
    }
    for (int t = 1; t <= M.tu; ++t)
    {
        ++num[M.data[t].j];
    }
    // 2. 计算每列在T中的起始位置
    copt[1] = 1;  // 第一列的第一个元素的位置一定是 T 中的第一个位置
    for (int col = 2; col <= M.nu; ++col)
    {
        copt[col] = copt[col - 1] + num[col - 1]; 
    }
    /* ===================================================
    // 实际上 num 数组没有必要，上述 1、2 部分代码可以简化为:
    for (int t = 1; t <= M.tu; ++t)
    {
        ++copt[M.data[t].j];
    }
    int next = 1;
    for (int col = 1; col <= M.nu; ++col)
    {
        // 下面三行代码顺序不能交换
        int count = copt[col];     // 暂存当前列个数，用于 next += count; 求下一个 copt[col]
        copt[col] = next;          // copt[col] 赋值
        next += count;             // 下一个 copt[col]
    }
    ====================================================== */
    // 3. 一次扫描完成转置
    for (int p = 1; p <= M.tu; ++p)
    {
        int col = M.data[p].j;
        int q = copt[col];
        T->data[q].i = M.data[p].j;
        T->data[q].j = M.data[p].i;
        T->data[q].e = M.data[p].e;
        ++copt[col];  // 每一列当前元素赋值后下一次遇到该列，给下一个元素赋值
    }
    return OK;
}

// === 辅助函数：手动构建测试矩阵（避免 scanf）===
void BuildTestMatrix(RLSMatrix *M, int mu, int nu, int tu, Triple triples[]) {
    M->mu = mu; M->nu = nu; M->tu = tu;
    for (int k = 1; k <= tu; ++k) {
        M->data[k] = triples[k-1];  // triples 从0开始
    }
    // 构建 rpos
    int num[MAXRC+1] = {0};
    for (int t = 1; t <= tu; ++t) ++num[M->data[t].i];
    M->rpos[1] = 1;
    for (int row = 2; row <= mu; ++row)
        M->rpos[row] = M->rpos[row-1] + num[row-1];
}

void PrintTestHeader(const char *title) {
    printf("\n========== %s ==========\n", title);
}

int main() {
    RLSMatrix M, N, Q, T;
    Triple m_triples[] = {
        {1, 2, 3}, {2, 1, 4}, {2, 3, 5}, {3, 2, 1}  // 3x3 矩阵
    };
    Triple n_triples[] = {
        {1, 1, 1}, {2, 2, 2}, {3, 3, 3}  // 3x3 对角矩阵
    };
    
    // 1. 测试 Create + Print
    PrintTestHeader("1. 创建并打印矩阵 M");
    BuildTestMatrix(&M, 3, 3, 4, m_triples);
    PrintSMatrix(M);  // 应输出 3x3 稀疏矩阵
    
    // 2. 测试 Copy
    PrintTestHeader("2. 复制矩阵");
    CopySMatrix(M, &T);
    PrintSMatrix(T);
    
    // 3. 测试 Add: M + N
    PrintTestHeader("3. 矩阵加法 M + N");
    BuildTestMatrix(&N, 3, 3, 3, n_triples);
    if (AddSMatrix(M, N, &Q) == OK) {
        printf("加法成功，非零元个数: %d\n", Q.tu);
        PrintSMatrix(Q);
    }
    
    // 4. 测试 Subt: M - N
    PrintTestHeader("4. 矩阵减法 M - N");
    if (SubtSMatrix(M, N, &Q) == OK) {
        printf("减法成功，非零元个数: %d\n", Q.tu);
        PrintSMatrix(Q);
    }
    
    // 5. 测试 Multiply: M * N
    PrintTestHeader("5. 矩阵乘法 M * N");
    if (MultiSMatrix(M, N, &Q) == OK) {
        printf("乘法成功，非零元个数: %d\n", Q.tu);
        PrintSMatrix(Q);
    }
    
    // 6. 测试 Transpose
    PrintTestHeader("6. 普通转置");
    if (TransposeSMatrix(M, &T) == OK) {
        printf("转置后维度: %dx%d, 非零元: %d\n", T.mu, T.nu, T.tu);
        PrintSMatrix(T);
    }
    
    // 7. 测试 FastTranspose
    PrintTestHeader("7. 快速转置");
    if (FastTransposeSMatrix(M, &T) == OK) {
        printf("快速转置后维度: %dx%d, 非零元: %d\n", T.mu, T.nu, T.tu);
        PrintSMatrix(T);
    }
    
    // 8. 错误处理测试 (修正版)
    PrintTestHeader("8. 错误处理测试");

    // 1. 空指针测试
    printf("空指针创建: %s\n", CreateSMatrix(NULL) == ERROR ? "捕获" : "失败");

    // 2. 构造一个维度不同的矩阵 (2行4列) 用于触发错误
    Triple bad_triples[] = {{1, 1, 1}}; 
    RLSMatrix Bad;
    BuildTestMatrix(&Bad, 2, 4, 1, bad_triples); 

    // 3. 测试加法维度不匹配 (3x3 + 2x4) -> 应返回 ERROR
    printf("维度不匹配加法: %s\n", AddSMatrix(M, Bad, &Q) == ERROR ? "捕获" : "失败");

    // 4. 测试乘法维度不匹配 (3x3 * 2x4) -> M.nu(3) != Bad.mu(2) -> 应返回 ERROR
    printf("非法乘法 (3x3 * 2x4): %s\n", MultiSMatrix(M, Bad, &Q) == ERROR ? "捕获" : "失败");
    
    // 9. 边界测试：零矩阵相加
    PrintTestHeader("9. 边界测试：零矩阵");
    RLSMatrix Z1, Z2, ZQ;
    BuildTestMatrix(&Z1, 2, 2, 0, NULL);
    BuildTestMatrix(&Z2, 2, 2, 0, NULL);
    AddSMatrix(Z1, Z2, &ZQ);
    printf("零矩阵相加非零元个数: %d (应为0)\n", ZQ.tu);
    
    // 10. 抵消测试：相同元素相加为0
    PrintTestHeader("10. 抵消测试：A + (-A)");
    Triple a_triples[] = {{1,1,5}, {2,2,3}};
    Triple neg_a[] = {{1,1,-5}, {2,2,-3}};
    RLSMatrix A, NegA, Zero;
    BuildTestMatrix(&A, 2, 2, 2, a_triples);
    BuildTestMatrix(&NegA, 2, 2, 2, neg_a);
    AddSMatrix(A, NegA, &Zero);
    printf("A + (-A) 非零元个数: %d (应为0)\n", Zero.tu);
    
    printf("\n所有测试完成！\n");
    system("pause");
    return 0;
}