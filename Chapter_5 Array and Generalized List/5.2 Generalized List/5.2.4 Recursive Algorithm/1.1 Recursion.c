#include <stdio.h>
#include <stdlib.h>
#include "../../../Chapter_4 String/4.1 Fixed-length Sequential String/1. Fixed-length Sequential String.h"  // 不使用 string.h
#include "../../../Status.h"  //5.2.1 的 Status 版本(非完整)

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

// 求表深
int GListDepth(GList L)
{
    // 基本项(递归终止状态)
    if (!L) return 1;                   // 空表深度为 1
    if (L->tag == ATOM) return 0;       // 原子深度为 0
    // 归纳项(非空表)
    int max = 0;
    // 在同一层中遍历(tp指针) (扩展线性链表头结点 L 的 tp 域永远为空，要先进入 hp 域，即 pp = L->a.hp;)  
    for (GLNode *pp = L; pp; pp = pp->a.ptr.tp)  
    {
        int dep = GListDepth(pp->a.ptr.hp);      // 求以 pp->a.ptr.hp 为头指针的子表深度(扩展线性链表为 dep = GListDepth(pp);)
        if (dep > max)
            max = dep;
    }
    return max + 1;                     // 非空表的深度是各元素最大深度+1
}

// 复制表
Status CopyGList(GList *T, GList L)
{
    if (!L) 
        *T = NULL;          // L 为空表，则复制空表(本质不是复制，但初始化空表就是操作 *T = NULL)
    else                    // 非空可能为原子节点或非空表
    {
        *T = (GList)malloc(sizeof(GLNode));
        if (!(*T)) exit(OVERFLOW);
        (*T)->tag = L->tag;     // 复制标志域
        if ((*T)->tag == ATOM)  // 单原子，则复制原子节点值域
        {
            (*T)->a.atom = L->a.atom;
        }
        else                    // 非空表，则 递归 复制 hp 域和 tp 域
        {
            CopyGList(&((*T)->a.ptr.hp), L->a.ptr.hp);
            CopyGList(&((*T)->a.ptr.tp), L->a.ptr.tp);
        }
    }
    return OK;
}

// ========= 对于头尾链表，表头表尾一定是表 ===========
// 取表头
Status GetHead(GList *head, GList L)
{
    if (!L || L->tag == ATOM)   // 注意：头尾链表, 原子不算做表，无表头(hp)和表尾(tp)
    {
        return ERROR;           // 无表头，返回错误
    }
    // 非空表
    // 实现 CopyGList()(深拷贝)
    // (*head)->tag = LIST;
    // (*head)->a.ptr.tp = NULL;
    // (*head) = L->a.ptr.hp;
    // 浅拷贝，获取指针
    *head = L->a.ptr.hp;        // 通常获取表头可以做相应操作，而非深拷贝不能修改表头
    return OK;
}

// 取表尾
Status GetTail(GList *tail, GList L)
{
    if (!L || L->tag == ATOM) return ERROR;
    // 实现 CopyGList()(深拷贝)
    // (*tail)->tag = LIST;
    // (*tail)->a.ptr.tp = NULL;
    // (*tail) = L->a.ptr.tp;
    // 浅拷贝，获取指针
    *tail = L->a.ptr.tp;
    return OK;
}

// 分割出表头(hstr)和表尾(传入 str 为脱去外层括号的广义表)
// 书中的sever带Status，实际无返回值，我这里直接用 void
void sever(SString str, SString hstr)  // 数组名即为指针
{
    int n = StrLength(str);
    int i = 0, k = 0;           // k 记尚未配对的左括号个数
    char ch;                    // i 对应的当前字符
    do {                        // 搜索最外层的第一个逗号
        ++i;                    // 跳过长度 str[0]
        ch = str[i];            // 由于下面的 SubString 接收的第一个参数必须为SString，而SString[0]又存长度，不好实现，故而直接用这个
        // SubString(&ch, str, i, 1);
        if (ch == '(')
            ++k;
        else if (ch == ')')
            --k;
    } while (i < n && (ch != ',' || k != 0));  // 遍历完 str 或 遇到表头后的逗号 退出循环
    if (i < n)              // 找到表头且表头为 str 子集，i 为 表头后的逗号
    {
        SubString(hstr, str, 1, i - 1);        // 复制表头给 hstr
        SubString(str, str, i + 1, n - i);     // 复制表尾给 str
    }
    else                    // str 即为表头
    {
        StrCopy(hstr, str);                    // 复制表头 str 给 hstr
        ClearString(str);                       // 表尾为'\0'
    }
}

// 使用 SString 字符串结尾没有 '\0'
Status CreateGList(GList *L, SString S)
{
    if (StrCompare(S, "()") == 0)   // 这里的 "()" = 书中的emp
        *L = NULL;  // S 为空表，则 *L 创建空表
    else
    {
        *L = (GList)malloc(sizeof(GLNode));             // 建表节点(原子和非空表都看作表节点)
        if (!(*L)) exit(OVERFLOW);
        if (StrLength(S) == 1)                          // S 为单字符串，则创建单原子广义表, 这里等价于 S[0] == 1
        {
            (*L)->tag = ATOM;
            (*L)->a.atom = S[1];                        // S[0]存长度, 从 S[1] 开始存元素
        }
        else                                            // 非空表，则递归创建
        {
            (*L)->tag = LIST;
            GLNode *p = *L, *q;                         
            SString sub, hsub;                          // 数组名就是指针
            SubString(sub, S, 2, StrLength(S) - 2);     // 脱外层括号 StrLength(S) - 2) 为脱括号后的长度，非最后一个元素的下标
            do {                                        // 重复建 n 个子表
                sever(sub, hsub);                       // 分离表头 hsub 和表尾 sub
                CreateGList(&(p->a.ptr.hp), hsub);      // 递归创建表头
                q = p;                                  // p 作为创建的结点, q 为其前驱
                if (!StrEmpty(sub))                     // 表尾不为空
                {
                    p = (GLNode*)malloc(sizeof(GLNode));
                    if (!p) exit(OVERFLOW);
                }
                p->tag = LIST;
                q->a.ptr.tp = p;
            } while (!StrEmpty(sub));                   // 即 StrLength(sub) > 0
            q->a.ptr.tp = NULL;
        } 
    }
    return OK;
}