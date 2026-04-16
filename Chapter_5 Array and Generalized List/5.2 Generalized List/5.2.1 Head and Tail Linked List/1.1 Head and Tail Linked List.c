#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../Status.h"

typedef char AtomType;

// 用枚举表示标志 tag: ATOM=0原子，LIST=1子表
typedef enum { ATOM, LIST } ElemTag;

// 头尾链表存储结构
typedef struct GLNode
{
    ElemTag tag;        // 标志域，公共部分，用于区分原子节点和表节点
    union               // 共用体（用于多选一，共享同一片内存空间(与struct的区别，故而可以节省空间)，使用多个会覆盖）：根据 tag 选择
    {
        AtomType atom;          // tag = 0: union a 为原子结点值域
        struct                  // tag = 1: union a 为表节点的两个指针
        {
            struct GLNode *hp;  // 表头指针（指向下一层）
            struct GLNode *tp;  // 表尾指针（指向同层下一个，一定为表节点）
        } ptr;
    } a;
    
} GLNode, *GList;

/* ==================== 基本操作实现 ==================== */
// 1.初始化
void InitGList(GList *L)
{
    *L = NULL;
}

// 辅助函数：分离表头和表尾字符串，a,(b,c), (a, b), (b, c)
void sever(char *str, char *hstr)  // 切断
{
    // str 是广义表(必须已经脱去外层括号)，之后被分离为表尾(带括号和逗号), 作为输出，hstr是分离后的表头, 作为输出
    int i = 0, j = 0, k = 0;    // i:读str, j:写hstr, k:括号深度
    // 获取表头
    while (str[i] && (str[i] != ',' || k != 0))  // 遇到逗号 且 不在任何括号内(k==0) 时才停止
    {
        if (str[i] == '(')
            ++k;
        else if (str[i] == ')')
            --k;
        if (k != 0 || str[i] != ',')  // str[i] 为 char 或 括号
        {
            hstr[j++] = str[i++];
        }
    }
    hstr[j] = '\0';
    // 获取表尾
    if (str[i] == ',')
        ++i;  // 跳过表头后的 ','
    j = 0;
    char rstr[100];             // 临时存储表尾字符串
    while (str[i])
        rstr[j++] = str[i++];
    rstr[j] = '\0';
    strcpy(str, rstr);
}

// 2.创建广义表(S 是广义表的书写形式串, 如 (), (a,(b,c)).)
void CreateGList(GList *L, const char *S)  // 用 const 方便直接传(a,(b,c))
{
    if (strcmp(S, "()") == 0)   // 空表
    {
        *L = NULL;  
    }
    else if (strlen(S) == 1)    // 单原子
    {
        *L = (GList)malloc(sizeof(GLNode));
        (*L)->tag = ATOM;
        (*L)->a.atom = S[0];
    }
    /*
    输入字符串: "(a,(b,c),d)"

    目标结构:
            LIST(tag=1)
        /          \
        hp(表头)      tp(表尾)
        |            |
        'a'          LIST(tag=1)
                    /      \
                hp        tp
                |         |
                "(b,c)"    'd'
    */
    else  // 一般表，如 (a, (b, c))
    {
        *L = (GList)malloc(sizeof(GLNode));
        (*L)->tag = LIST;

        char sub[100], hsub[100];  // (a,b,c)
        strncpy(sub, S + 1, strlen(S) - 2);  // 脱去外层括号
        sub[strlen(S) - 2] = '\0';  // 数组下标从 0 开始，现在里面有 Strlen(S) - 2 个元素，下表最大为 Strlen(S) - 3
        // 此时, "(a,(b,c),d)" → "a,(b,c),d"
        GList p = *L, q;
        do {
            sever(sub, hsub);  // ① 拆分：sub="a,(b,c),d" → hsub="a", sub="(b,c),d"
            CreateGList(&(p->a.ptr.hp), hsub);  // 递归调用, 创建当前层表头
            q = p;  // // ③ 保存当前结点
            if (strlen(sub) > 0)  // 若表尾还有元素
            {
                p = (GList)malloc(sizeof(GLNode));  // 新建同层节点
                p->tag = LIST;
                // 插入同层节点
                q->a.ptr.tp = p;  // 链接同层下一个元素
            }
        } while (strlen(sub) > 0);  // 直到sub为空停止循环
        q->a.ptr.tp = NULL;  // 最后一个的tp置空
    }
}

// 3.销毁
void DestroyGList(GList *L)
{
    if (*L)
    {
        if ((*L)->tag == LIST)
        {
            DestroyGList(&((*L)->a.ptr.hp));  // 递归销毁表头指针，等价于 (*L)->a.ptr.hp 先 free 再置空
            DestroyGList(&((*L)->a.ptr.tp));  // 递归销毁表尾指针
        }
    }
    free(*L);
    *L = NULL;
}

// 4.复制(深拷贝)
void CopyGList(GList *T, GList L)
{
    if (!L)
    {
        *T = NULL;  // 空表复制
        return;
    }
    // 非空表，先给 T 分配空间
    *T = (GLNode*)malloc(sizeof(GLNode));
    if (!*T)
        exit(0);  // 内存上溢
    // 复制标志位
    (*T)->tag = L->tag;
    if ((*T)->tag == ATOM)  // 原子节点 -- 只复制值域
    {
        (*T)->a.atom = L->a.atom;
    }
    else                    // 表节点，递归复制表头、表尾
    {
        CopyGList(&((*T)->a.ptr.hp), L->a.ptr.hp);
        CopyGList(&((*T)->a.ptr.tp), L->a.ptr.tp);
    }
}

// 5.求长度(顶层元素个数)
int GListLength(GList L)
{
    int len = 0;
    if (!L || L->tag == ATOM)
    {
        return (L ? 1 : 0);  // L 不为空返回 1
    }
    // 需要复制，因为原表头可能与其他部分共享
    GList p = L;
    while (p)
    {
        ++len;
        p = p->a.ptr.tp;
    }
    return len;
}

// 6.求深度
int GListDepth(GList L)
{
    if (!L)
        return 1;  // 空表深度为 1
    if (L->tag == ATOM)
        return 0;  // 原子深度为 0
    int max = 0, dep;
    // 找最深的那一棵树(最深的表)，每一个分支都找，直到找完所有表
    for (GList p = L; p != NULL; p = p->a.ptr.tp)
    {
        dep = GListDepth(p->a.ptr.hp);  // 递归求子表深度
        if (dep > max)
            max = dep;
    }
    return max + 1;
}

// 7.取表头 -- 注意：需要深拷贝复制表头，因为原表头可能与其他部分共享
GList GetHead(GList L)
{
    if (!L || L->tag != LIST)  // 传入(的表)为空 或 不是表(结点)
    {
        printf("传入的不是表结点!\n");  // L是原子或NULL
        exit(0);
    }
    if (!L->a.ptr.hp)  //传入的表是表节点，但是为空表
    {
        printf("空表无表头!\n");
        exit(0);
    }
    GList h;
    CopyGList(&h, L->a.ptr.hp);  // 深拷贝表头元素
    return h;
}

// 8.取表尾 -- 表尾一定是一个子表(tag = LIST)
GList GetTail(GList L)
{
    if (!L || L->tag != LIST)  // 传入(的表)为空 或 不是表(结点)
    {
        printf("传入的不是表结点!\n");  // L是原子或NULL
        exit(0);
    }
    if (!L->a.ptr.hp)  //传入的表是表节点，但是为空表
    {
        printf("空表无表头!\n");
        exit(0);
    }
    GList t = (GLNode*)malloc(sizeof(GLNode));
    t->tag = LIST;
    CopyGList(&t, L->a.ptr.tp);  // 复制原表尾给t
    return t;
}

// 9.遍历
// void(*visit)(AtomType)：void -- visit函数无返回值；(*visit)-- visit 是一个指针，存储某个函数的地址，通过它可以间接调用那个函数；(AtomType) --	指向的函数接受一个 AtomType 参数
void Traverse_GL(GList L, void(*visit)(AtomType))  // 允许前向引用，在调用遍历函数前定义 visit 即可
{
    if (!L)
        return;  // 空表直接返回，不报错
    if (L->tag == ATOM)
    {
        visit(L->a.atom);  // 编译器自动解引用
    }
    else
    {
        Traverse_GL(L->a.ptr.hp, visit);  // 递归遍历表头
        Traverse_GL(L->a.ptr.tp, visit);  // 递归遍历表尾
    }
}

void visit(AtomType e)
{
    printf("%c ", e);
}

// 10.打印
void PrintGList(GList L)
{
    if (!L)
    {
        printf("()");
        return;
    }
    if (L->tag == ATOM)
    {
        printf("%c", L->a.atom);
    }
    else
    {
        printf("(");
        GList p = L;
        while (p)
        {
            PrintGList(p->a.ptr.hp);  // 递归打印下一层
            p = p->a.ptr.tp;
            if (p)
            {
                printf(", ");
            }
        }
        printf(")");
    }
}

/* 测试 */
int main() {
    GList L, L2, h, t, copy;
    
    printf("========== 测试1: 创建与打印 ==========\n");
    printf("输入: \"(a,(b,c),d)\"\n");
    InitGList(&L);
    CreateGList(&L, "(a,(b,c),d)");
    printf("期望输出: (a, (b, c), d)\n");
    printf("实际输出: ");
    PrintGList(L);
    printf("\n\n");
    
    printf("========== 测试2: 求长度 ==========\n");
    printf("期望长度: 3 (a, (b,c), d 三个顶层元素)\n");
    printf("实际长度: %d\n\n", GListLength(L));
    
    printf("========== 测试3: 求深度 ==========\n");
    printf("期望深度: 2 (最深层: (b,c) 内的原子)\n");
    printf("实际深度: %d\n\n", GListDepth(L));
    
    printf("========== 测试4: 遍历 ==========\n");
    printf("期望输出: a b c d \n");
    printf("实际输出: ");
    Traverse_GL(L, visit);
    printf("\n\n");
    
    printf("========== 测试5: 取表头 ==========\n");
    printf("原表: ");
    PrintGList(L);
    printf("\n");
    h = GetHead(L);
    printf("期望表头: a\n");
    printf("实际表头: ");
    PrintGList(h);
    printf("\n\n");
    DestroyGList(&h);
    
    printf("========== 测试6: 取表尾 ==========\n");
    printf("原表: ");
    PrintGList(L);
    printf("\n");
    t = GetTail(L);
    printf("期望表尾: ((b, c), d)\n");
    printf("实际表尾: ");
    PrintGList(t);
    printf("\n\n");
    DestroyGList(&t);
    
    printf("========== 测试7: 复制 ==========\n");
    CopyGList(&copy, L);
    printf("期望复制: (a, (b, c), d)\n");
    printf("实际复制: ");
    PrintGList(copy);
    printf("\n");
    printf("复制表长度: %d, 深度: %d\n\n", GListLength(copy), GListDepth(copy));
    DestroyGList(&copy);
    
    printf("========== 测试8: 单元素表 ==========\n");
    InitGList(&L2);
    CreateGList(&L2, "(x)");
    printf("输入: \"(x)\"\n");
    printf("期望输出: (x)\n");
    printf("实际输出: ");
    PrintGList(L2);
    printf("\n长度: %d (期望1)\n", GListLength(L2));
    printf("深度: %d (期望1)\n", GListDepth(L2));
    t = GetTail(L2);
    printf("表尾(应为空表): ");
    PrintGList(t);
    printf(" (期望())\n\n");
    DestroyGList(&t);
    DestroyGList(&L2);
    
    printf("========== 测试9: 销毁 ==========\n");
    printf("销毁L前: L=%p\n", (void*)L);
    DestroyGList(&L);
    printf("销毁L后: L=%p (期望NULL/0)\n\n", (void*)L);
    
    printf("========== 所有测试完成 ==========\n");
    system("pause");
    return 0;
}