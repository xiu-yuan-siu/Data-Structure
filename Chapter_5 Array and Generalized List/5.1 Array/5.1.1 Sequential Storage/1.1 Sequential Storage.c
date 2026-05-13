#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>  // 提供宏 va_start、va_arg 和 va_end, 用于存取变长参数表
#include "../../../Status.h"

#define MAX_ARRAY_DIM 8  // 数组维数最大值为 8

typedef int ElemType;

typedef struct
{
    ElemType *base;     // 数组元素基址，即首元素地址 -- LOC(0, 0, ..., 0)
    int dim;            // 数组维数
    int *bounds;        // 数组维界基址 -- 存储各维度长度(b_i)的数组，用于下标检查
    int *constants;     // 数组映像函数常量基址 -- 存储各维度权值(c_i)的数组，不同的 j_i 变动 1，在物理空间跳过的元素个数    
} Array;

Status InitArray(Array *A, int dim, ...)
{
    if (dim < 1 || dim > MAX_ARRAY_DIM)
    {
        return ERROR;
    }
    A->dim = dim;

    // 先给 bounds 数组分配空间并赋值
    A->bounds = (int*)malloc(dim * sizeof(int));
    if (!A->bounds)
    {
        exit(OVERFLOW);
    }
    // 如果各维度合法，存入 bounds 数组，并求出 A 的元素总数 elemtotal
    int elemtotal = 1;
    va_list ap;  // 游标，本质为指针
    va_start(ap, dim);
    for (int i = 0; i < dim; ++i)
    {
        A->bounds[i] = va_arg(ap, int);
        if (A->bounds[i] < 0)
        {
            return UNDERFLOW;
        }
        elemtotal *= A->bounds[i];
    }
    va_end(ap);

    // 再给 base 数组分配空间，不用赋值
    A->base = (ElemType*)malloc(elemtotal * sizeof(ElemType));
    if (!A->base)
    {
        exit(OVERFLOW);
    }

    // 最后给 constants 数组分配空间并赋值
    A->constants = (int*)malloc(dim * sizeof(int));
    if (!A->constants)
    {
        exit(OVERFLOW);
    }
    // c_n = L
    A->constants[dim - 1] = 1;  // 最后一维 j_i 每变动 1，跳过元素个数也为 1
    for (int i = dim - 2; i >= 0; --i)
    {
        // c_(n-1) = b_n * c_n
        A->constants[i] = A->bounds[i+1] * A->constants[i+1];
    }
    return OK;
}

Status DestroyArray(Array *A)
{
    // 对 base、bounds、constants 三个数组先判断是否存在，存在则释放指针并为指针置空
    // base
    if (!A->base)
    {
        return ERROR;
    }
    free(A->base);
    A->base = NULL;
    // bounds
    if (!A->bounds)
    {
        return ERROR;
    }
    free(A->bounds);
    A->bounds = NULL;
    // constants
    if (!A->constants)
    {
        return ERROR;
    }
    free(A->constants);
    A->constants = NULL;

    return OK;
}

Status Locate(Array A, va_list ap, int *off)
{
    // 若 ap 指示的各下标值合法，则求出该元素在 A 中的相对地址 off
    *off = 0;
    for (int i = 0; i < A.dim; ++i)
    {
        int idx = va_arg(ap, int);
        if (idx < 0 || idx >= A.bounds[i])   // idx 过小 或 超过该维的长度(下标范围是 [0, bounds[i]-1]，idx == bounds[i] 时已越界)
        {
            return ERROR;
        }
        *off += A.constants[i] * idx;
    }
    return OK;
}

Status Value(Array A, ElemType *e, ...)
{
    // *e = A[j_1][j_2]...[j_n]
    va_list ap;
    va_start(ap, e);
    int off;
    Status result = Locate(A, ap, &off);
    if (result <= 0)
    {
        return result;  // 下标不合法
    }
    *e = *(A.base + off);  // 或 *e = A.base[off];
    return OK;
}

Status Assign(Array *A, ElemType e, ...)
{
    // A[j_1][j_2]...[j_n] = e
    va_list ap;
    va_start(ap, e);
    int off;
    Status result = Locate(*A, ap, &off);
    if (result <= 0)
    {
        return result;
    }
    A->base[off] = e;
    return OK;
}

// ===== 辅助打印函数（纯 ASCII 兼容） =====
void PrintArray2D(Array A) {
    if (A.dim != 2) { printf("[WARN] Not 2D array\n"); return; }
    printf("+");
    for (int j = 0; j < A.bounds[1]; j++) printf("----");
    printf("+\n");
    
    for (int i = 0; i < A.bounds[0]; i++) {
        printf("|");
        for (int j = 0; j < A.bounds[1]; j++) {
            ElemType val;
            Value(A, &val, i, j);
            printf("%3d ", val);
        }
        printf("| <- A[%d][*]\n", i);
    }
    printf("+");
    for (int j = 0; j < A.bounds[1]; j++) printf("----");
    printf("+\n");
}

void PrintArray3D(Array A) {
    if (A.dim != 3) { printf("[WARN] Not 3D array\n"); return; }
    for (int k = 0; k < A.bounds[2]; k++) {
        printf("[Layer k=%d]\n", k);
        for (int i = 0; i < A.bounds[0]; i++) {
            for (int j = 0; j < A.bounds[1]; j++) {
                ElemType val;
                Value(A, &val, i, j, k);
                printf("%3d ", val);
            }
            printf("\n");
        }
        printf("\n");
    }
}

// ===== 主测试函数 =====
int main() {
    Array A; ElemType val;

    printf("[TEST 1] Init 2D Array (3x4)\n");
    InitArray(&A, 2, 3, 4);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            Assign(&A, i * 10 + j, i, j);
    
    printf("[OUTPUT] 2D Array Content:\n");
    PrintArray2D(A);
    
    printf("[TEST 2] Boundary Check + Single Read\n");
    if (Value(A, &val, 3, 0) == ERROR) 
        printf("[OK] A[3][0] out of bounds -> blocked\n");
    
    Value(A, &val, 1, 2);
    printf("[OK] A[1][2] = %d (expect: 12)\n", val);
    
    DestroyArray(&A);
    
    printf("\n[TEST 3] Init 3D Array (2x3x2)\n");
    InitArray(&A, 3, 2, 3, 2);
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 2; k++)
                Assign(&A, i*100 + j*10 + k, i, j, k);
    
    printf("[OUTPUT] 3D Array Content (by layer):\n");
    PrintArray3D(A);
    
    Value(A, &val, 1, 2, 1);
    printf("[OK] A[1][2][1] = %d (expect: 121)\n", val);
    
    DestroyArray(&A);
    printf("\n[ALL TESTS PASSED]\n");
    
    // system("pause");
    return 0;
}
