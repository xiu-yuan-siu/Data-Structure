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

void GetPowerSet(int i, List A, List *B) {      // i: 当前考察A中第i个元素（1-based，对应A.data[i-1];A: 原集合;B: 当前已构造的子集（解向量）
    ElemType x;
    int k;                              // 局部变量 k 为进入函数时表 B 的当前长度
    if (i > ListLength(A)) {            // 叶子结点，输出当前 B 值，即幂集的一个元素
        Output(*B);
        ++powerSetCount;
        return;
    } else {
        GetElem(A, i, &x);              // 获取 A 的第 i 个元素
        k = ListLength(*B);
        // 选 x: 先插入，递归，再回溯删除
        ListInsert(B, k + 1, x);        // x 插入 B 最后面
        GetPowerSet(i + 1, A, B);       // 取 x，这里会完成输出
        ListDelete(B, k + 1, &x);       // 实践这里 x 最好改为 temp(无用值，仅作为参数调用函数)
        // 不选 x: 直接递归，什么都不做(前面没有插入，自然就没有选, i = 4)
        GetPowerSet(i + 1, A, B);       // 舍 x
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

/* ==================== 运行过程 ====================
GetPowerSet(i=1, A, B=∅)          ← 根节点，考察 A 的第 1 个元素（x=1）
    │
    ├── 【选 1】ListInsert(B, 1, 1) → B={1}
    │   │
    │   └── ① GetPowerSet(i=2, A, B={1})      ← 第一个GetPowerSet，考察第2个元素（x=2）
    │       │
    │       ├── 【选 2】ListInsert(B, 2, 2) → B={1,2}
    │       │   │
    │       │   └── ① GetPowerSet(i=3, A, B={1,2})      ← 第一个GetPowerSet，考察第3个元素（x=3）
    │       │       │
    │       │       ├── 【选 3】ListInsert(B, 3, 3) → B={1,2,3}
    │       │       │   │
    │       │       │   └── ① GetPowerSet(i=4, A, B={1,2,3})
    │       │       │       i=4 > ListLength(A)=3，叶子结点
    │       │       │       输出 {1, 2, 3}  ✓
    │       │       │       返回
    │       │       │   ListDelete(B, 3, &temp) → B={1,2}   ← 回溯撤销选3
    │       │       │
    │       │       └── 【不选 3】B 保持 {1,2}
    │       │           │
    │       │           └── ② GetPowerSet(i=4, A, B={1,2})      ← 第二个GetPowerSet（Delete后的）
    │       │               i=4 > 3，叶子结点
    │       │               输出 {1, 2}  ✓
    │       │               返回
    │       │   ListDelete(B, 2, &temp) → B={1}   ← 回溯撤销选2
    │       │
    │       └── 【不选 2】B 保持 {1}
    │           │
    │           └── ② GetPowerSet(i=3, A, B={1})      ← 第二个GetPowerSet（Delete后的），考察第3个元素（x=3）
    │               │
    │               ├── 【选 3】ListInsert(B, 2, 3) → B={1,3}
    │               │   │
    │               │   └── ① GetPowerSet(i=4, A, B={1,3})
    │               │       i=4 > 3，叶子结点
    │               │       输出 {1, 3}  ✓
    │               │       返回
    │               │   ListDelete(B, 2, &temp) → B={1}   ← 回溯撤销选3
    │               │
    │               └── 【不选 3】B 保持 {1}
    │                   │
    │                   └── ② GetPowerSet(i=4, A, B={1})      ← 第二个GetPowerSet（Delete后的）
    │                       i=4 > 3，叶子结点
    │                       输出 {1}  ✓
    │                       返回
    │   ListDelete(B, 1, &temp) → B=∅   ← 回溯撤销选1
    │
    └── 【不选 1】B 保持 ∅
        │
        └── ② GetPowerSet(i=2, A, B=∅)      ← 第二个GetPowerSet（Delete后的），考察第2个元素（x=2）
            │
            ├── 【选 2】ListInsert(B, 1, 2) → B={2}
            │   │
            │   └── ① GetPowerSet(i=3, A, B={2})      ← 第一个GetPowerSet，考察第3个元素（x=3）
            │       │
            │       ├── 【选 3】ListInsert(B, 2, 3) → B={2,3}
            │       │   │
            │       │   └── ① GetPowerSet(i=4, A, B={2,3})
            │       │       i=4 > 3，叶子结点
            │       │       输出 {2, 3}  ✓
            │       │       返回
            │       │   ListDelete(B, 2, &temp) → B={2}   ← 回溯撤销选3
            │       │
            │       └── 【不选 3】B 保持 {2}
            │           │
            │           └── ② GetPowerSet(i=4, A, B={2})      ← 第二个GetPowerSet（Delete后的）
            │               i=4 > 3，叶子结点
            │               输出 {2}  ✓
            │               返回
            │   ListDelete(B, 1, &temp) → B=∅   ← 回溯撤销选2
            │
            └── 【不选 2】B 保持 ∅
                │
                └── ② GetPowerSet(i=3, A, B=∅)      ← 第二个GetPowerSet（Delete后的），考察第3个元素（x=3）
                    │
                    ├── 【选 3】ListInsert(B, 1, 3) → B={3}
                    │   │
                    │   └── ① GetPowerSet(i=4, A, B={3})
                    │       i=4 > 3，叶子结点
                    │       输出 {3}  ✓
                    │       返回
                    │   ListDelete(B, 1, &temp) → B=∅   ← 回溯撤销选3
                    │
                    └── 【不选 3】B 保持 ∅
                        │
                        └── ② GetPowerSet(i=4, A, B=∅)      ← 第二个GetPowerSet（Delete后的）
                            i=4 > 3，叶子结点
                            输出 {}  ✓   ← 空集
                            返回
*/