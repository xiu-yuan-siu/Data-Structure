#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/* ============ 线性表定义和实现(顺序表) ============ */
#define MAXSIZE 100
typedef int ElemType;

typedef struct {
    ElemType data[MAXSIZE];
    int length;
} List;

void InitList(List *L) {
    L->length = 0;
}

int ListEmpty(List L) {
    return L.length == 0;
}

int ListLength(List L) {
    return L.length;
}

Status GetElem(List L, int i, ElemType *e) {
    if (i < 1 || i > L.length) return ERROR;
    *e = L.data[i - 1];                 // 数组下标 0 对应 i = 1 的第一个元素
    return OK;
}

Status ListInsert(List *L, int i, ElemType e) {
    if (i < 1 || i > L->length + 1) return ERROR;
    if (L->length >= MAXSIZE) return ERROR;     // 防溢出检查
    // 后移元素
    for (int j = L->length; j >= i; --j) {
        L->data[j] = L->data[j - 1];
    }
    L->data[i - 1] = e;
    ++(L->length);
    return OK;
}

Status ListDelete(List *L, int i, ElemType *e) {
    if (i < 1 || i > L->length) return ERROR;
    *e = L->data[i - 1];
    // 前移元素
    for (int j = i; j < L->length; ++j) {
        L->data[j - 1] = L->data[j];
    }
    --(L->length);
    return OK;
}

// 输出线形变(显示子集带换行)
void Output(List L) {
    printf("{");
    for (int i = 0; i < L.length; ++i) {
        if (i > 0)
            printf(", ");
        printf("%d", L.data[i]);
    }
    printf("}\n");
}

/* =============== 求幂集 =============== */
// 全局变量，记录解的个数
int powerSetCount = 0;

void GetPowerSet(int i, List A, List *B) {      // i: 当前考察A中第i个元素（1-based，对应A.elem[i-1];A: 原集合;B: 当前已构造的子集（解向量）
    ElemType x;
    int k;                              // 局部变量 k 为进入函数时表 B 的当前长度
    if (i > ListLength(A)) {            // 叶子结点，输出当前 B 值，即当前幂集的第一个元素
        Output(*B);
        ++powerSetCount;
        return;
    } else {
        GetElem(A, i, &x);              // 获取 A 的第 i 个元素
        k = ListLength(*B);
        // x进入当前子集B
        ListInsert(B, k + 1, x);
        GetPowerSet(i + 1, A, B);
        // 回溯：撤销选择，x不进入当前子集B
        ListDelete(B, k + 1, &x);
        GetPowerSet(i + 1, A, B);
    }
}

// 测试
int main() {
    List A, B;
    ElemType e;
    
    // 构造集合A = {1, 2, 3}
    InitList(&A);
    ListInsert(&A, 1, 1);
    ListInsert(&A, 2, 2);
    ListInsert(&A, 3, 3);
    
    printf("原集合A = ");
    Output(A);
    printf("\n\n幂集ρ(A)为:\n\n");
    
    InitList(&B);           // 第一次调用 GetPowerSet，B 为空表
    GetPowerSet(1, A, &B);
    
    printf("\n共 %d 个子集\n", powerSetCount);

    system("pause");
    return 0;
}