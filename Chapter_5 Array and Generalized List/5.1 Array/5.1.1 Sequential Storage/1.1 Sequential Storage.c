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

    // 最后给
}
