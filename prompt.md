1.1 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，没问题后，重写一份测试案例，测试所有函数的功能和所有报错，并在运行框中打印期望输出便于我测试对比

1.2 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，没问题后，重写一份测试案例，测试所有函数的功能，并在运行框中打印期望输出便于我测试对比

2. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

```c
// 扩展线性链表
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char AtomType;
typedef enum { ATOM, LIST } ElemTag;

/* 扩展线性链表存储结构 - 所有结点都是3个域 */
typedef struct GLNode {
    ElemTag tag;      // 标志域
    union {           // 共用体
        AtomType atom;           // tag=0：原子值
        struct GLNode *hp;       // tag=1：指向子表的指针
    } a;
    struct GLNode *tp;  // 指向同层下一个元素（无论原子还是子表都有）
} GLNode, *GList;

/* ==================== 基本操作实现 ==================== */

void InitGList(GList *L) {
    *L = NULL;
}

/* 销毁广义表 */
void DestroyGList(GList *L) {
    if (*L) {
        if ((*L)->tag == LIST) {
            DestroyGList(&((*L)->a.hp));   // 销毁子表
        }
        DestroyGList(&((*L)->tp));         // 销毁同层下一个
        free(*L);
        *L = NULL;
    }
}

/* 创建广义表 */
void CreateGList(GList *L, char *s) {
    void sever(char *str, char *hstr) {
        int i = 0, j = 0, k = 0;
        char rstr[100];
        while (str[i] && (str[i] != ',' || k)) {
            if (str[i] == '(') k++;
            else if (str[i] == ')') k--;
            if (k != 0 || str[i] != ',') hstr[j++] = str[i++];
        }
        hstr[j] = '\0';
        if (str[i] == ',') i++;
        j = 0;
        while (str[i]) rstr[j++] = str[i++];
        rstr[j] = '\0';
        strcpy(str, rstr);
    }
    
    if (strcmp(s, "()") == 0) {
        // 空表：创建一个tag=LIST但hp=NULL的结点
        *L = (GList)malloc(sizeof(GLNode));
        (*L)->tag = LIST;
        (*L)->a.hp = NULL;
        (*L)->tp = NULL;
    } else if (strlen(s) == 1) {
        // 单原子
        *L = (GList)malloc(sizeof(GLNode));
        (*L)->tag = ATOM;
        (*L)->a.atom = s[0];
        (*L)->tp = NULL;
    } else {
        // 一般表：先创建表头结点
        *L = (GList)malloc(sizeof(GLNode));
        (*L)->tag = LIST;
        (*L)->tp = NULL;
        
        char sub[100], hsub[100];
        strncpy(sub, s + 1, strlen(s) - 2);
        sub[strlen(s) - 2] = '\0';
        
        sever(sub, hsub);
        CreateGList(&((*L)->a.hp), hsub);  // 创建第一个元素
        
        GList p = (*L)->a.hp;
        while (strlen(sub) > 0) {
            sever(sub, hsub);
            CreateGList(&(p->tp), hsub);   // tp链接同层下一个
            p = p->tp;
        }
    }
}

/* 求长度 - 遍历tp链即可，更简单！ */
int GListLength(GList L) {
    if (!L || (L->tag == LIST && !L->a.hp)) return 0;  // 空表
    if (L->tag == ATOM) return 1;
    
    int len = 0;
    GList p = L->a.hp;  // 从表头结点指向的第一个元素开始
    while (p) {
        len++;
        p = p->tp;      // 直接沿tp遍历，无需递归判断
    }
    return len;
}

/* 求深度 */
int GListDepth(GList L) {
    if (!L || (L->tag == LIST && !L->a.hp)) return 1;
    if (L->tag == ATOM) return 0;
    
    int max = 0, dep;
    GList p;
    for (p = L->a.hp; p; p = p->tp) {
        dep = GListDepth(p);  // p可以是原子或子表，统一处理
        if (dep > max) max = dep;
    }
    return max + 1;
}

/* 取表头 - 直接返回a.hp，无需复杂操作 */
GList GetHead(GList L) {
    if (!L || (L->tag == LIST && !L->a.hp)) {
        printf("空表无表头!\n");
        exit(0);
    }
    return L->a.hp;  // 直接返回，注意：这是引用，不是复制
}

/* 取表尾 - 创建新表头结点，hp指向原表的第二个元素 */
GList GetTail(GList L) {
    if (!L || (L->tag == LIST && !L->a.hp)) {
        printf("空表无表尾!\n");
        exit(0);
    }
    GList t = (GList)malloc(sizeof(GLNode));
    t->tag = LIST;
    t->a.hp = L->a.hp->tp;  // 表尾是除去第一个元素的剩余部分
    t->tp = NULL;
    return t;
}

/* ==================== 扩展操作：插入和删除（头尾链表难以实现） ==================== */

/* 在第i个位置插入元素e - O(n) */
int Insert_GL(GList *L, int i, GList e) {
    if (!L || !(*L) || (*L)->tag != LIST) return 0;
    
    GList p = *L;
    // 找到第i-1个位置
    for (int j = 1; j < i && p->tp; j++) {
        p = p->tp;
    }
    
    // 插入：e的tp指向p的tp，p的tp指向e
    e->tp = p->tp;
    p->tp = e;
    return 1;
}

/* 删除第i个元素 - O(n) */
int Delete_GL(GList *L, int i, GList *e) {
    if (!L || !(*L) || (*L)->tag != LIST) return 0;
    
    GList p = *L;
    // 找到第i-1个位置
    for (int j = 1; j < i && p->tp; j++) {
        p = p->tp;
    }
    
    if (!p->tp) return 0;  // 位置超出范围
    
    *e = p->tp;            // 被删除的元素
    p->tp = (*e)->tp;      // 绕过被删除元素
    (*e)->tp = NULL;       // 断开连接
    return 1;
}

/* 按位置查找第i个元素 - O(n) */
GList GetElem(GList L, int i) {
    if (!L || L->tag != LIST) return NULL;
    
    GList p = L->a.hp;
    int j = 1;
    while (p && j < i) {
        p = p->tp;
        j++;
    }
    return p;  // 返回第i个元素的指针，可以是原子或子表
}

/* 遍历 */
void Traverse_GL(GList L, void(*visit)(AtomType)) {
    if (!L) return;
    if (L->tag == ATOM) {
        visit(L->a.atom);
    } else {
        Traverse_GL(L->a.hp, visit);
    }
    Traverse_GL(L->tp, visit);  // 遍历同层下一个
}

void PrintGList(GList L) {
    if (!L) return;
    if (L->tag == ATOM) {
        printf("%c", L->a.atom);
    } else {
        if (!L->a.hp) {
            printf("()");
        } else {
            printf("(");
            GList p = L->a.hp;
            while (p) {
                PrintGList(p);
                p = p->tp;
                if (p) printf(", ");
            }
            printf(")");
        }
    }
}

void visit(AtomType e) {
    printf("%c ", e);
}

/* 测试扩展操作 */
int main() {
    GList L, e;
    InitGList(&L);
    
    char str[] = "(a,b,(c,d),e)";
    CreateGList(&L, str);
    
    printf("原广义表: ");
    PrintGList(L);
    printf("\n长度: %d\n", GListLength(L));
    
    // 测试按位置查找
    printf("\n第3个元素: ");
    GList elem = GetElem(L, 3);
    if (elem) PrintGList(elem);
    printf("\n");
    
    // 测试插入：在第2个位置插入原子'x'
    e = (GList)malloc(sizeof(GLNode));
    e->tag = ATOM;
    e->a.atom = 'x';
    e->tp = NULL;
    Insert_GL(&(L->a.hp), 2, e);
    
    printf("插入x后的广义表: ");
    PrintGList(L);
    printf("\n长度: %d\n", GListLength(L));
    
    // 测试删除
    GList deleted;
    Delete_GL(&(L->a.hp), 2, &deleted);
    printf("删除第2个元素后: ");
    PrintGList(L);
    printf("\n被删除的元素: ");
    PrintGList(deleted);
    printf("\n");
    
    DestroyGList(&L);
    return 0;
}
```

```cpp

```
