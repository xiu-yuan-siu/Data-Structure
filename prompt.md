1.1 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，没问题后，重写一份测试案例，测试所有函数的功能和所有报错，并在运行框中打印期望输出便于我对比

1.2 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，没问题后，重写一份测试案例，测试所有函数的功能，并在运行框中打印期望输出便于我对比

2. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

```c
#include <stdio.h>
#include <stdlib.h>

// ==================== 数据类型定义 ====================

char vars[10];  // 变元名数组
int var_count;  // 变元个数

typedef enum { ATOM, LIST } NodeTag;

typedef struct MPNode {
    NodeTag tag;      // ATOM=原子结点(常数), LIST=表结点(还有子变元)
    int exp;          // 对于表头:变元下标; 对于项:该变元的指数
    union {
        float coef;   // ATOM:系数
        struct MPNode *hp;  // LIST:指向子表(下一层表头或子多项式)
    } val;
    struct MPNode *tp;  // 指向同一层的下一项
} MPNode, *MPList;

// ==================== 辅助函数 ====================

// 【修改点1】创建表头结点：hp为空，通过tp指向第一项
MPNode* CreateHeader(int var_idx, MPNode *first_item) {
    MPNode *p = (MPNode*)malloc(sizeof(MPNode));
    if (!p) exit(1);
    p->tag = LIST;
    p->exp = var_idx;      // 变元下标
    p->val.hp = NULL;      // 表头结点的 hp 域不存储项
    p->tp = first_item;    // tp 指向该层的第一个项结点
    return p;
}

// 创建项结点 - 系数为子多项式 (tag=LIST, exp=指数, hp=子表头)
MPNode* CreateItemList(int exp, MPNode *sub_header, MPNode *next) {
    MPNode *p = (MPNode*)malloc(sizeof(MPNode));
    if (!p) exit(1);
    p->tag = LIST;
    p->exp = exp;          // 当前变元的指数
    p->val.hp = sub_header; // 指向下一层表头
    p->tp = next;
    return p;
}

// 创建项结点 - 系数为常数 (tag=ATOM, exp=指数, coef=系数)
MPNode* CreateItemAtom(int exp, float coef, MPNode *next) {
    MPNode *p = (MPNode*)malloc(sizeof(MPNode));
    if (!p) exit(1);
    p->tag = ATOM;
    p->exp = exp;          // 当前变元的指数
    p->val.coef = coef;    // 系数
    p->tp = next;
    return p;
}

// ==================== 构建示例多项式 ====================
// P(x,y,z) = x^10*y^3*z^2 + 2*x^6*y^3*z^2 + 3*x^5*y^2*z^2 
//            + x^4*y^4*z + 6*x^3*y^4*z + 2*y*z + 15

MPList CreateExamplePoly() {
    var_count = 3;
    vars[0] = 'z'; vars[1] = 'y'; vars[2] = 'x';
    
    // ===== 最内层：x层 =====
    MPNode *C = CreateHeader(2,           // var_idx=2 (x)
        CreateItemAtom(10, 1.0f,          // (1,10)
        CreateItemAtom(6, 2.0f, NULL)));  // (2,6)
    
    MPNode *D = CreateHeader(2,
        CreateItemAtom(5, 3.0f, NULL));
    
    MPNode *E = CreateHeader(2,
        CreateItemAtom(4, 1.0f,
        CreateItemAtom(3, 6.0f, NULL)));
    
    MPNode *F = CreateHeader(2,
        CreateItemAtom(0, 2.0f, NULL));
    
    // ===== 中间层：y层 =====
    MPNode *A = CreateHeader(1,          // var_idx=1 (y)
        CreateItemList(3, C,             // (C,3)
        CreateItemList(2, D, NULL)));    // (D,2)
    
    MPNode *B = CreateHeader(1,
        CreateItemList(4, E,
        CreateItemList(1, F, NULL)));
    
    // ===== 最外层：z层 =====
    MPList P = CreateHeader(0,           // var_idx=0 (z)
        CreateItemList(2, A,             // (A,2)
        CreateItemList(1, B,             // (B,1)
        CreateItemAtom(0, 15.0f, NULL)))); // (15,0)
    
    return P;
}

// ==================== 输出方式一：正常三元多项式形式 ====================

typedef struct {
    int exps[10];   // 各变元的指数 [z_idx, y_idx, x_idx]
    float coef;
} Term;

void CollectTerms(MPNode *header, Term *terms, int *term_count, int *current_exps) {
    if (!header || header->tag != LIST) return;
    
    int var_idx = header->exp;  // 当前层的变元下标
    
    // 【修改点2】从 tp 开始遍历当前层的所有项，而不是从 hp
    MPNode *item = header->tp;
    while (item) {
        current_exps[var_idx] = item->exp;
        
        if (item->tag == LIST) {
            // 系数是子多项式，递归处理下一层表头
            CollectTerms(item->val.hp, terms, term_count, current_exps);
        } 
        else if (item->tag == ATOM) {
            // 系数是常数，保存这一项
            terms[*term_count].coef = item->val.coef;
            for (int i = 0; i < var_count; i++) {
                terms[*term_count].exps[i] = current_exps[i];
            }
            (*term_count)++;
        }
        
        item = item->tp;
    }
    
    // 回溯
    current_exps[var_idx] = 0;
}

void PrintVarPower(char var, int exp) {
    if (exp == 0) return;
    if (exp == 1) printf("%c", var);
    else printf("%c^%d", var, exp);
}

void PrintNormal(MPList P) {
    printf("【正常三元多项式形式】\n");
    printf("P(x,y,z) = ");
    
    Term terms[50];
    int term_count = 0;
    int current_exps[10] = {0};
    
    CollectTerms(P, terms, &term_count, current_exps);
    
    for (int i = 0; i < term_count; i++) {
        float coef = terms[i].coef;
        int *exps = terms[i].exps;
        
        if (i > 0) {
            if (coef >= 0) printf(" + ");
            else { printf(" - "); coef = -coef; }
        } else if (coef < 0) {
            printf("-"); coef = -coef;
        }
        
        int has_var = 0;
        for (int j = 0; j < var_count; j++) 
            if (exps[j] != 0) has_var = 1;
        
        if (!has_var || coef != 1.0f) {
            if (coef == (int)coef) printf("%d", (int)coef);
            else printf("%.1f", coef);
        }
        
        for (int j = var_count - 1; j >= 0; j--) {
            PrintVarPower(vars[j], exps[j]);
        }
    }
    printf("\n");
}

// ==================== 输出方式二：分解变元形式 ====================

// 【修改点3】重写分解输出，依据新的表头特征 (hp == NULL) 识别嵌套结构
void PrintDecomposed(MPNode *header) {
    if (!header) return;
    
    // 识别出这是一个表头结点
    if (header->tag == LIST && header->val.hp == NULL) {
        printf("%c(", vars[header->exp]);
        
        MPNode *item = header->tp; // 数据项从 tp 拿
        int first = 1;
        while (item) {
            if (!first) printf(", ");
            first = 0;
            
            if (item->tag == ATOM) {
                if (item->val.coef == (int)item->val.coef) 
                    printf("(%d,%d)", (int)item->val.coef, item->exp);
                else 
                    printf("(%.1f,%d)", item->val.coef, item->exp);
            } else {
                // 如果是项结点的系数为子多项式，打印括号并向下递归 hp（指向下一层表头）
                printf("(");
                PrintDecomposed(item->val.hp);
                printf(",%d)", item->exp);
            }
            item = item->tp;
        }
        printf(")");
    }
}

void PrintDecomposedForm(MPList P) {
    printf("\n【分解变元形式（广义表结构）】\n");
    PrintDecomposed(P);
    printf("\n\n【层次化分解展示】\n");
    printf("P = z((A,2), (B,1), (15,0))\n");
    printf("A = y((C,3), (D,2))\n");
    printf("B = y((E,4), (F,1))\n");
    printf("C = x((1,10), (2,6))\n");
    printf("D = x((3,5))\n");
    printf("E = x((1,4), (6,3))\n");
    printf("F = x((2,0))\n");
}

// ==================== 内存释放 ====================

void DestroyMPList(MPNode *p) {
    if (!p) return;
    // 如果是子多项式项结点，hp 指向子表头，需要递归进入释放
    if (p->tag == LIST && p->val.hp) DestroyMPList(p->val.hp);
    // 沿着同级链表释放后续结点
    DestroyMPList(p->tp);
    free(p);
}

// ==================== 主函数 ====================

int main() {
    printf("========================================\n");
    printf("  严蔚敏《数据结构》m元多项式实现\n");
    printf("  三元多项式示例（标准表头形式修复版）\n");
    printf("========================================\n\n");
    
    MPList P = CreateExamplePoly();
    
    PrintNormal(P);
    PrintDecomposedForm(P);
    
    printf("\n【书中原始多项式】\n");
    printf("P(x,y,z) = x^10y^3z^2 + 2x^6y^3z^2 + 3x^5y^2z^2 + x^4y^4z + 6x^3y^4z + 2yz + 15\n");
    
    DestroyMPList(P);
    return 0;
}
```

```cpp

```
