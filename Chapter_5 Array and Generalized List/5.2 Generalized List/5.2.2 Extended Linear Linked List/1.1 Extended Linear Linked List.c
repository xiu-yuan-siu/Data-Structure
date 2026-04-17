#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../Status.h"

typedef char AtomType;
typedef enum{ ATOM, LIST } ElemTag;

// 扩展线性链表存储结构(所有节点都是 3 个域)
typedef struct GLNode
{
    ElemTag tag;    // 标志域，用于区分原子节点和表节点
    union
    {
        AtomType atom;      // 原子节点的值域
        struct GLNode *hp;  // 表节点的表头指针域 -- 指向下一层
    } a;
    struct GLNode *tp;      // 相当于线性链表的 next，指向同层下一个元素节点(原子节点和表节点均有)
} GLNode, *GList;

/* ==================== 基本操作实现 ==================== */
// 优势体现在：插入、删除、查找 三个函数上
// 1.初始化
Status InitGList(GList *L)
{
    *L = (GList)malloc(sizeof(GLNode));
    if (!*L)
        exit(OVERFLOW);
    (*L)->tag = LIST;
    (*L)->a.hp = NULL;
    (*L)->tp = NULL;        // 扩展线性链表 tp 域永远为 NULL; hp 域才用于指示子表中的第一个元素
    return OK;
}

// 2.创建
// 字符串分离：提取第一个顶层元素至 hstr，剩余部分存回 str
void sever(char *str, char *hstr)
{
    // 传入的 str 要脱去外层括号
    int i = 0, j = 0, k = 0;  // i:指示str; j:指示hstr; k: 括号深度
    while (str[i] && (str[i] != ',' || k != 0))  // 只有 str[i] = ',' 且 k = 0时(即str[i]位于表头后的逗号)才退出循环
    {
        if (str[i] == '(')
            ++k;
        else if (str[i] == ')')
            --k;
        // 只有 str[i] = ',' 且 k = 0时(即str[i]位于表头后的逗号)才不进入下面的分支
        if (str[i] != ',' || k != 0)  // 加入表头的括号和字符
        {
            hstr[j++] = str[i++];
        }
    }
    hstr[j] = '\0';  // 表头字符串结束符
    if (str[i] == ',')
        ++i;  // 跳过表头后面的 ','
    // 通过临时数组 tstr 复制表尾到 str
    j = 0;
    char tstr[100];
    while (str[i])
        tstr[j++] = str[i++];
    tstr[j] = '\0';  // 表尾字符串结束符
    strcpy(str, tstr);
}

Status CreateGList(GList *L, const char *S)  // 传 const char* 可直接传左值 CreateGList(&L, (a, (b, c), d));
{
    // 创建节点
    *L = (GList)malloc(sizeof(GLNode));
    if (!(*L)) exit(OVERFLOW);

    if (strcmp(S, "()") == 0)   // 空表
    {
        (*L)->tag = LIST;
        (*L)->a.hp = NULL;
        (*L)->tp = NULL;
    }
    else if (strlen(S) == 1)
    {
        (*L)->tag = ATOM;
        (*L)->a.atom = S[0];
        (*L)->tp = NULL;
    }
    else                        // 一般表
    {
        (*L)->tag = LIST;
        (*L)->tp = NULL;
        // 脱外层括号
        char sub[100], hsub[100];
        strncpy(sub, S + 1, strlen(S) - 2);
        sub[strlen(S) - 2] = '\0';
        // 分割表头，并递归调用创建第一个元素
        sever(sub, hsub);
        CreateGList(&((*L)->a.hp), hsub);
        // 遍历 sub 直至遍历完所有节点
        GLNode *p = (*L)->a.hp;
        while (strlen(sub) > 0)
        {
            sever(sub, hsub);  // 获取新表头
            CreateGList(&(p->tp), hsub);  // 链接到同层下一个
            p = p->tp;
        }
    }
    return OK;
}

// 3.销毁
Status DestroyGList(GList *L)
{
    if (!L || !(*L))  // 指针为空(未初始化或已销毁)或者指针指向为空
        return OK;
    // 空表和非空表都会在下面释放头结点
    if ((*L)->tag == LIST)
    {
        DestroyGList(&((*L)->a.hp));    // 递归销毁下一层
    }
    DestroyGList(&((*L)->tp));          // 递归销毁同层下一个
    free(*L);  // 如果是表结点，先释放原子节点，再释放表节点
    *L = NULL;
    return OK;
}

// 4.复制(深拷贝)
Status CopyGList(GList *T, GList L)
{
    if (!L)  // 
    {
        *T = NULL;
        return OK;
    }
    *T = (GList)malloc(sizeof(GLNode));
    if (!(*T)) exit(OVERFLOW);

    (*T)->tag = L->tag;
    if (L->tag == ATOM)  // 原子节点, 深拷贝值域
    {
        (*T)->a.atom = L->a.atom;
    }
    else                // 表节点，递归深拷贝下一层
    {
        CopyGList(&((*T)->a.hp), L->a.hp);
    }
    // 无论是原子节点还是表节点，都可能有后继需要拷贝
    CopyGList(&((*T)->tp), L->tp);
    return OK;
}

// 5.长度 -- 直接 next(tp) 遍历
int GListLength(GList L)  // 参数是头结点，一定是LIST 
{
    if (!L) return 0;  // 未初始化、销毁长度均为 0
    if (L->tag == ATOM) return 0;  // 创建单个原子(不是表), 长度应为 0
    int len = 0;
    GLNode *p = L->a.hp;  // 进入表
    while (p)  // 空表不会进入循环
    {
        ++len;
        p = p->tp;
    }
    return len;
}

// 6.深度 -- 原子(depth=0)、空表(depth=1)、非空表(depth=max(所有元素depth)+1)(+1为原子那一层)
int GListDepth(GList L)
{
    // 基本型
    if (!L)  return 0; // 未初始化或已销毁长度为 0;
    if (L->a.hp == NULL) return 1;  // 空表长度为 1
    if (L->tag == ATOM) return 0;   // 创建单个原子(不是表)，深度为 0
    // 归纳项
    int max = 0;
    for (GLNode *p = L->a.hp; p != NULL; p = p->tp)  // 同层遍历
    {
        int dep = GListDepth(p);  // 递归求该节点深度
        if (dep > max)
            max = dep;
    }
    return max + 1;
}

// 7.判空
int GListEmpty(GList L)  // 传入一定是表
{
    if (!L)  // 未初始化或已销毁
    {
        return 1;  // 视为空表
    }
    return (L->a.hp == NULL);
}

// 8.取表头
Status GetHead(GList *H, GList L)  // 参数是头结点，一定是LIST 
{
    if (!L || !L->a.hp) return ERROR;   // 未初始化 或 空表
    CopyGList(H, L->a.hp);
    return OK;
}

// 9.取表尾 -- 表尾一定是表结点
Status GetTail(GList *T, GList L)  // 参数是头结点，一定是LIST
{
    if (!L || !L->a.hp) return ERROR;   // 未初始化 或 空表

    *T = (GList)malloc(sizeof(GLNode));
    if (!(*T)) exit(OVERFLOW);
    (*T)->tag = LIST;
    (*T)->tp = NULL;

    if (L->a.hp->tp == NULL)        // 表 L 只有一个元素，无后继
    {
        (*T)->a.hp = NULL;          // 表尾为空表   
    }
    else
    {
        CopyGList(&((*T)->a.hp), L->a.hp->tp);  // L->a.hp从头结点进入表，表头后继所有部分(L->a.hp->tp)即是表尾，这里面会处理 ATOM
    }
    return OK;
}

// 10.插入(在第i个位置之前插入，i从1开始)
Status Insert_GL(GList *L, int i, GList e)  // 参数是头结点，一定是LIST
{
    if (!L || !(*L) || i < 1) return ERROR;
    // 插入位置为广义表的第一个位置
    if (i == 1)  
    {
        e->tp = (*L)->a.hp;
        (*L)->a.hp = e;
        return OK;
    }
    // 找第 i-1 个位置
    GList p = (*L)->a.hp;   // 从第一个元素开始找
    for (int j = 1; j < i-1 && p != NULL; ++j)
    {
        p = p->tp;
    }
    if (p == NULL) return ERROR;  // i 偏大 
    // 插入
    e->tp = p->tp;
    p->tp = e;
    return OK;
}

// 11.删除(删除第i个元素，e返回被删元素)
Status Delete_GL(GList *L, int i, GList *e)  // 参数 L 是头结点，一定是LIST
{
    if (!L || !(*L) || i < 1) return ERROR;
    // 删除第1个结点
    if (i == 1)
    {
        if ((*L)->a.hp == NULL) return ERROR;  // 空表无法删除
        *e = (*L)->a.hp;    // 保存被删除的结点
        (*L)->a.hp = (*e)->tp;      // 删除第 1 个结点
        (*e)->tp = NULL;    // 断开被删除的结点
        return OK;
    }
    // 找第 i-1 个结点
    GList p = (*L)->a.hp;
    for (int j = 1; j < i - 1 && p != NULL; ++j)
    {
        p = p->tp;
    }
    if (p == NULL || p->tp == NULL)  return ERROR;// i 偏大，超出范围
    // 删除
    (*e) = p->tp;
    p->tp = (*e)->tp;
    // 删除的结点无后继，注意置空
    (*e)->tp = NULL;
    return OK;
}

// 12.按位置查找
GList GetElem(GList L, int i)  // 参数是头结点，一定是LIST
{
    if (!L || i < 1) return NULL;  // 未初始化或已销毁 返回 头结点 = NULL(未初始化或已销毁)

    GList p = L->a.hp;
    for (int j = 1; j < i && p != NULL; ++j)
    {
        p = p->tp;
    }
    return p;
}

// 13.遍历
Status Traverse_GL(GList L, void(*visit)(AtomType))  // 会递归到ATOM节点，必须判断tag
{
    if (!L) return ERROR;

    if (L->tag == ATOM)
    {
        visit(L->a.atom);
    }
    else
    {
        GLNode *p = L->a.hp;
        while (p)
        {
            Traverse_GL(p, visit);  // 递归调用
            p = p->tp;
        }
    }
    return OK;
}

// 14.打印
Status PrintGList(GList L)  // 会递归到ATOM节点，必须判断tag
{
    if (!L) return OK;

    if (L->tag == ATOM)
    {
        printf("%c", L->a.atom);
    }
    else
    {
        printf("(");
        GLNode *p = L->a.hp;
        while (p)
        {
            PrintGList(p);  // 递归调用
            if (p->tp) 
                printf(",");
            p = p->tp;
        }
        printf(")");
    }
    return OK;
}

/* 辅助：visit函数 */
void visit(AtomType e)
{
    printf("%c ", e);
}

/* ==================== 测试函数 ==================== */

void print_separator(const char* title) {
    printf("\n========== %s ==========\n", title);
}

int main()
{
    GList L = NULL, T = NULL, H = NULL, e = NULL, temp = NULL;
    
    printf("广义表(GList)功能测试 (Strictly based on Yan Weimin's Data Structures)\n");
    printf("======================================================================\n");
    
    /* ==================== 基础创建与属性测试 ==================== */
    
    // 测试1: 初始化空表
    print_separator("Test 1: InitGList - 初始化空表");
    InitGList(&L);
    printf("操作: InitGList(&L)\n");
    printf("实际输出: "); PrintGList(L);
    printf("\n[检查] 长度:%d(期望0), 深度:%d(期望1), 空表:%d(期望1)\n", 
           GListLength(L), GListDepth(L), GListEmpty(L));

    // 测试2: 创建空表字符串 "()"
    print_separator("Test 2: CreateGList - 创建空表 \"()\"");
    DestroyGList(&L);
    CreateGList(&L, "()");
    printf("操作: CreateGList(&L, \"()\")\n");
    printf("实际输出: "); PrintGList(L);
    printf("\n[检查] 长度:%d(期望0), 深度:%d(期望1)\n", 
           GListLength(L), GListDepth(L));

    // 测试3.1: 创建单原子 "a" (注意：原子不是表)
    print_separator("Test 3.1: CreateGList - 单原子 \"a\" (非表结构)");
    DestroyGList(&L);
    CreateGList(&L, "a");
    printf("操作: CreateGList(&L, \"a\") -> 这是一个原子节点\n");
    printf("实际输出: "); PrintGList(L);
    printf("\n[检查] 长度:%d(期望0), 深度:%d(期望0)\n", 
           GListLength(L), GListDepth(L));

    // 测试3.2: 创建单元素表 "(a)"
    print_separator("Test 3.2: CreateGList - 单元素表 \"(a)\"");
    DestroyGList(&L);
    CreateGList(&L, "(a)");
    printf("操作: CreateGList(&L, \"(a)\") -> 这是一个包含原子a的表\n");
    printf("实际输出: "); PrintGList(L);
    printf("\n[检查] 长度:%d(期望1), 深度:%d(期望1)\n", 
           GListLength(L), GListDepth(L));

    // 测试4: 创建复杂表
    print_separator("Test 4: CreateGList - 一般表 \"(a,(b,c),d)\"");
    DestroyGList(&L);
    CreateGList(&L, "(a,(b,c),d)");
    printf("操作: CreateGList(&L, \"(a,(b,c),d)\")\n");
    printf("实际输出: "); PrintGList(L);
    printf("\n[检查] 长度:%d(期望3), 深度:%d(期望2)\n", 
           GListLength(L), GListDepth(L));

    /* ==================== 遍历与拷贝 ==================== */

    // 测试5: 遍历
    print_separator("Test 5: Traverse_GL - 线性遍历");
    printf("操作: Traverse_GL(L, visit)\n");
    printf("期望序列: a b c d \n实际序列: ");
    Traverse_GL(L, visit);
    printf("\n");

    // 测试6: 深拷贝
    print_separator("Test 6: CopyGList - 深拷贝");
    CopyGList(&T, L);
    printf("操作: CopyGList(&T, L)\n");
    printf("原表 L: "); PrintGList(L);
    printf("\n拷贝 T: "); PrintGList(T);
    printf("\n[检查] L与T是否独立? 修改T不应影响L (后续验证)\n");

    /* ==================== 取头尾操作 ==================== */

    // 测试7: 取表头
    print_separator("Test 7: GetHead - 取表头");
    GetHead(&H, L);
    printf("操作: GetHead(&H, L) [L=(a,(b,c),d)]\n");
    printf("期望: a\n实际: "); PrintGList(H);
    printf("\n");
    DestroyGList(&H); // 清理临时变量

    // 测试8: 取表尾
    print_separator("Test 8: GetTail - 取表尾");
    GetTail(&H, L);
    printf("操作: GetTail(&H, L) [L=(a,(b,c),d)]\n");
    printf("期望: ((b,c),d)\n实际: "); PrintGList(H);
    printf("\n");
    DestroyGList(&H);

    /* ==================== 插入操作链 (关键逻辑验证) ==================== */
    // 当前 L = (a, (b,c), d)
    // 索引: 1:a, 2:(b,c), 3:d

    // 测试9: 头部插入
    print_separator("Test 9: Insert_GL - 在位置1插入原子 \"e\"");
    CreateGList(&e, "e");
    Insert_GL(&L, 1, e); 
    // 预期: e 插在 a 前面 -> (e, a, (b,c), d)
    printf("操作: Insert_GL(&L, 1, e)\n");
    printf("期望: (e,a,(b,c),d)\n实际: "); PrintGList(L);
    printf("[检查] 长度:%d(期望4)\n", GListLength(L));
    e = NULL; // e 已归入 L，指针置空防重复释放

    // 测试10: 中间插入
    print_separator("Test 10: Insert_GL - 在位置3插入子表 \"(x,y)\"");
    CreateGList(&e, "(x,y)");
    Insert_GL(&L, 3, e);
    // 当前: 1:e, 2:a, 3:(b,c), 4:d
    // 操作: 在第3个元素 ((b,c)) 之前插入 (x,y)
    // 预期: (e, a, (x,y), (b,c), d)
    printf("操作: Insert_GL(&L, 3, (x,y))\n");
    printf("期望: (e,a,(x,y),(b,c),d)\n实际: "); PrintGList(L);
    printf("[检查] 长度:%d(期望5)\n", GListLength(L));
    e = NULL;

    /* ==================== 查找操作 ==================== */
    
    print_separator("Test 11: GetElem - 按位置查找");
    // 当前列表: 1:e, 2:a, 3:(x,y), 4:(b,c), 5:d
    
    temp = GetElem(L, 1);
    printf("GetElem(L, 1) -> 期望: e, 实际: "); PrintGList(temp); printf("\n");
    
    temp = GetElem(L, 3);
    printf("GetElem(L, 3) -> 期望: (x,y), 实际: "); PrintGList(temp); printf("\n");
    
    temp = GetElem(L, 5);
    printf("GetElem(L, 5) -> 期望: d, 实际: "); PrintGList(temp); printf("\n");
    
    temp = GetElem(L, 10);
    printf("GetElem(L, 10)-> 期望: (null), 实际: %s\n", temp == NULL ? "(null)" : "非空(错误)");

    /* ==================== 删除操作链 (关键逻辑验证) ==================== */
    // 当前 L = (e, a, (x,y), (b,c), d)

    // 测试12: 删除中间元素
    print_separator("Test 12: Delete_GL - 删除第2个元素");
    e = NULL;
    Delete_GL(&L, 2, &e);
    // 操作: 删除第2个 (a)
    // 预期剩余: (e, (x,y), (b,c), d)
    printf("操作: Delete_GL(&L, 2, &e)\n");
    printf("被删元素期望: a, 实际: "); PrintGList(e); printf("\n");
    printf("剩余列表期望: (e,(x,y),(b,c),d)\n实际: "); PrintGList(L);
    printf("[检查] 长度:%d(期望4)\n", GListLength(L));
    DestroyGList(&e); // 释放被删节点

    // 测试13: 删除头部元素
    print_separator("Test 13: Delete_GL - 删除第1个元素");
    e = NULL;
    Delete_GL(&L, 1, &e);
    // 当前: 1:e, 2:(x,y), 3:(b,c), 4:d
    // 操作: 删除第1个 (e)
    // 预期剩余: ((x,y), (b,c), d)
    printf("操作: Delete_GL(&L, 1, &e)\n");
    printf("被删元素期望: e, 实际: "); PrintGList(e); printf("\n");
    printf("剩余列表期望: ((x,y),(b,c),d)\n实际: "); PrintGList(L);
    printf("[检查] 长度:%d(期望3)\n", GListLength(L));
    DestroyGList(&e);

    // 测试14: 删除尾部元素
    print_separator("Test 14: Delete_GL - 删除最后一个元素(第3个)");
    e = NULL;
    Delete_GL(&L, 3, &e);
    // 当前: 1:(x,y), 2:(b,c), 3:d
    // 操作: 删除第3个 (d)
    // 预期剩余: ((x,y), (b,c))
    printf("操作: Delete_GL(&L, 3, &e)\n");
    printf("被删元素期望: d, 实际: "); PrintGList(e); printf("\n");
    printf("剩余列表期望: ((x,y),(b,c))\n实际: "); PrintGList(L);
    printf("[检查] 长度:%d(期望2)\n", GListLength(L));
    DestroyGList(&e);

    /* ==================== 高级测试 ==================== */

    // 测试15: 深度递归测试
    print_separator("Test 15: Depth Test - 嵌套表 \"((a,b),(c,(d,e)))\"");
    DestroyGList(&L);
    CreateGList(&L, "((a,b),(c,(d,e)))");
    printf("操作: CreateGList(&L, \"((a,b),(c,(d,e)))\")\n");
    printf("实际输出: "); PrintGList(L);
    printf("\n[检查] 长度:%d(期望2), 深度:%d(期望3)\n", 
           GListLength(L), GListDepth(L));
    // 解析深度: 
    // Layer 1: ((...), (...)) -> depth = max(dep1, dep2) + 1
    // Layer 2: (a,b) -> 1, (c, (d,e)) -> max(0, 2)+1 = 2
    // Max(1, 2) + 1 = 3. Correct.

    /* ==================== 收尾 ==================== */

    print_separator("Test 16: DestroyGList - 彻底销毁");
    DestroyGList(&L);
    DestroyGList(&T);
    printf("操作: DestroyGList(&L), DestroyGList(&T)\n");
    printf("L 是否为 NULL? %s\n", L == NULL ? "是 (Pass)" : "否 (Fail)");
    printf("T 是否为 NULL? %s\n", T == NULL ? "是 (Pass)" : "否 (Fail)");
    
    printf("\n======================================================================\n");
    printf("所有测试完成!\n");

    system("pause");
    return 0;
}