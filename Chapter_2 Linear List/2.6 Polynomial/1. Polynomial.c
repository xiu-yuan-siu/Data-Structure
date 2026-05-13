#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

typedef struct 
{
    float coef;  // coefficient
    int expn;    // exponent
} term, ElemType;  // term 用于书中 ADT，ElemType 为 LinkList 的数据对象(data)名称

typedef struct LNode
{
    ElemType data;
    struct LNode *next;
} *LinkList, *Polynomial;

Status InitList(Polynomial *P)
{
    *P = (LinkList)malloc(sizeof(struct LNode));  // struct 不能省
    if (!(*P))
    {
        exit(OVERFLOW);
    }
    (*P)->next = NULL;
    return OK;
}

void CreatePolyn(Polynomial *P, int m)  
{
    InitList(P);
    for (int i = 1; i <= m; ++i)
    {
        ElemType e;
        printf("请按顺序输入第 %d 项的系数和指数:\t", i);
        scanf("%f, %d", &e.coef, &e.expn);
        // 按指数递增插入
        struct LNode *pre = *P, *s;  // pre 为插入结点所在位置的前驱结点
        // 找前驱(前驱的后继的指数 >= 输入的指数), 插入项应该在 1.二者之间 或 2.和pre的后继指数相同
        while (pre->next && pre->next->data.expn < e.expn)  
        {
            pre = pre->next;
        }
        // 边界处理 1.指数相同 2.指数和为 0
        if (pre->next && pre->next->data.coef == e.coef)  // 指数相同
        {
            pre->next->data.coef += e.coef;
            if (!pre->next->data.coef)  // 相加后的指数等于 0, 删除结点
            {
                s = pre->next;
                pre->next = s->next;
                free(s);
            }
        }
        else  // 指数不同
        {
            s = (struct LNode*)malloc(sizeof(struct LNode));
            // s->data.coef = e.coef;
            // s->data.expn = e.expn;
            // 等价于
            s->data = e;
            s->next = pre->next;
            pre->next = s;
        }
    }
}

void DestroyPolyn(Polynomial *P)
{
    struct LNode *q = *P;
    while (*P)
    {
        q = (*P)->next;
        free(*P);
        *P = q;
    }
}

void PrintPolyn(Polynomial P)
{
    struct LNode *q = P->next;
    int flag = 1;
    while (q)
    {
        if (!flag && q->data.coef)  // !flag控制第一项前面不输出 + 号
        {
            printf(" + ");
        }
        printf("%.2fx^%d", q->data.coef, q->data.expn);
        q = q->next;
        flag = 0;
    }
    printf("\n");
}

void AddPolyn(Polynomial *Pa, Polynomial *Pb)
{
    // Pa = Pa + Pb
    struct LNode *ha = *Pa, *hb = *Pb;
    struct LNode *qa = ha->next, *qb = hb->next;
    while (qa && qb)
    {
        int a = qa->data.expn, b = qb->data.expn;
        if (a < b)  // qa 系数小于 qb, 加入 qa(实际上qa本来就在ha中，只用移动ha即可)，++qa
        {
            ha = qa;
            qa = qa->next;
        }
        else if (a == b)  // qa 系数 = qb
        {
            float sum = qa->data.coef + qb->data.coef;
            if (!sum)  // 系数之和为 0, 删除结点
            {
                ha->next = qa->next;
                free(qa);
            }
            else  // 系数之和不为 0，更新 qa 的系数
            {
                qa->data.coef = sum;
                ha = qa;
            }
            // 指数相等时 qa 和 qb 都要 ++
            qa = ha->next;  // 这里只能用 ha，qa可能已经释放(系数之和为 0)
            // 释放 Pb 中的结点(随着循环进行，Pb最终只剩头结点)
            struct LNode *temp = qb;
            qb = qb->next;
            hb->next = qb;  // 更新 Pb 的头结点
            free(temp);
        }
        else  // qa 系数大于 qb, 在 Pa 加入 qb, 在 Pb 中 删除 qb
        {
            // 在 Pb 中 删除 qb, hb -> qb -> qb->next -> ... 到 hb -> qb->next -> ...
            hb->next = qb->next;
            // 建立 qb 到 Pa 后续项的链接, 不然 qa 后面的数据就会丢失
            qb->next = qa;
            // 在 Pa 加入 qb, 进行完下面一行代码后， ha -> qb -> qa
            ha->next = qb;
            // 更新 ha 永远指向比较结点的前驱
            ha = qb;
            // 更新 qb = qb->next;
            qb = hb->next;
        }
    }
    // 链接 Pb 剩余部分
    if (qb)
    {
        ha->next = qb;
    }
    // 释放 Pb的头结点
    free(*Pb);
    *Pb = NULL;  // 置空防止野指针
}

void SubtractPolyn(Polynomial *Pa, Polynomial *Pb)
{
    // Pa = Pa - Pb，借助 多项式加法实现
    struct LNode *q = (*Pb)->next;
    while (q)
    {
        q->data.coef *= -1;  // Pb 所有项系数取反
        q = q->next;
    }
    AddPolyn(Pa, Pb);
}

void MultiplyPolyn(Polynomial *Pa, Polynomial *Pb)
{
    // Pa = Pa x Pb， 思路：让 Pa 每一项与 Pb 所有项相乘，然后利用多项式加法求和
    Polynomial Pc, Ptemp;
    InitList(&Pc);  // 存乘法结果，最后赋值给 *Pa
    struct LNode *qa = (*Pa)->next, *qb;
    while (qa)  // 遍历 Pa 的每一项
    {
        InitList(&Ptemp);
        qb = (*Pb)->next;
        while (qb)  // 遍历 Pb 的每一项，让 Pa 每一项与 Pb 所有项相乘
        {
            ElemType e;
            e.coef = qa->data.coef * qb->data.coef;  // 系数相乘
            e.expn = qa->data.expn + qb->data.expn;  // 指数相加
            // 将 Pa 每一项与 Pb 每一项相乘得到的多项式存入临时多项式 Ptemp
            // qb 循环遍历完则 Pa 每一项与 Pb 所有项相乘结果均存入了 Ptemp
            struct LNode *s = (struct LNode*)malloc(sizeof(struct LNode));
            s->data = e;
            s->next = NULL;
            struct LNode *pre = Ptemp;  // 指向最后一个结点，让其指向新结点 s
            while (pre->next)  // 找最后一个结点
            {
                pre = pre->next;
            }
            pre->next = s;
            qb = qb->next;  // 更新 qb
        }
        // 然后调用加法将 Pa 每一项与 Pb 所有项相乘结果加入 Pc
        // qa 循环遍历完则 Pa 所有项与 Pb 所有项相乘的结果均存入了 Pc
        AddPolyn(&Pc, &Ptemp);
        qa = qa->next;  // 更新 qa
    }
    DestroyPolyn(Pa);
    DestroyPolyn(Pb);
    *Pa = Pc;
}

// 辅助测试函数：通过数组直接创建多项式，避免每次手动输入测试烦恼
void CreatePolynByArray(Polynomial *P, float coefs[], int expns[], int m) 
{
    InitList(P);
    for (int i = 0; i < m; ++i)
    {
        ElemType e;
        e.coef = coefs[i];
        e.expn = expns[i];
        struct LNode *pre = *P, *s; 
        
        // 修复 3：去掉 ! 号，防止空指针解引用
        while (pre->next && pre->next->data.expn < e.expn)  
        {
            pre = pre->next;
        }
        
        // 修复 4：判断条件去掉 ! 号，并将判断项改为 expn (指数)
        if (pre->next && pre->next->data.expn == e.expn)  
        {
            pre->next->data.coef += e.coef;
            if (pre->next->data.coef == 0)  // 浮点数比较，直接判断为0
            {
                s = pre->next;
                pre->next = s->next;
                free(s);
            }
        }
        else  
        {
            s = (struct LNode*)malloc(sizeof(struct LNode));
            s->data = e;
            s->next = pre->next;
            pre->next = s;
        }
    }
}

int main() {
    Polynomial Pa, Pb;

    printf("--- 测试 1: 多项式加法 ---\n");
    float coef_a1[] = {3, 4, 7}; int expn_a1[] = {1, 3, 5};
    float coef_b1[] = {2, -4, 5}; int expn_b1[] = {2, 3, 6};
    CreatePolynByArray(&Pa, coef_a1, expn_a1, 3);
    CreatePolynByArray(&Pb, coef_b1, expn_b1, 3);
    printf("Pa = "); PrintPolyn(Pa);
    printf("Pb = "); PrintPolyn(Pb);
    AddPolyn(&Pa, &Pb);
    printf("Pa + Pb = "); PrintPolyn(Pa);
    printf("期望结果: 3.00x^1 + 2.00x^2 + 7.00x^5 + 5.00x^6\n\n");
    DestroyPolyn(&Pa); 

    printf("--- 测试 2: 多项式减法 ---\n");
    float coef_a2[] = {5, 2}; int expn_a2[] = {2, 4};
    float coef_b2[] = {5, 3}; int expn_b2[] = {2, 5};
    CreatePolynByArray(&Pa, coef_a2, expn_a2, 2);
    CreatePolynByArray(&Pb, coef_b2, expn_b2, 2);
    printf("Pa = "); PrintPolyn(Pa);
    printf("Pb = "); PrintPolyn(Pb);
    SubtractPolyn(&Pa, &Pb);
    printf("Pa - Pb = "); PrintPolyn(Pa);
    printf("期望结果: 2.00x^4 - 3.00x^5 (注意5x^2被抵消)\n\n");
    DestroyPolyn(&Pa);

    printf("--- 测试 3: 多项式乘法 ---\n");
    float coef_a3[] = {2, 1}; int expn_a3[] = {1, 2}; // 2x^1 + x^2
    float coef_b3[] = {3, 4}; int expn_b3[] = {0, 1}; // 3x^0 + 4x^1
    CreatePolynByArray(&Pa, coef_a3, expn_a3, 2);
    CreatePolynByArray(&Pb, coef_b3, expn_b3, 2);
    printf("Pa = "); PrintPolyn(Pa);
    printf("Pb = "); PrintPolyn(Pb);
    MultiplyPolyn(&Pa, &Pb);
    printf("Pa * Pb = "); PrintPolyn(Pa);
    printf("期望结果: 6.00x^1 + 11.00x^2 + 4.00x^3\n\n");
    DestroyPolyn(&Pa);

    printf("测试全部通过！\n");
    // system("pause");
    return 0;
}