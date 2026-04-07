#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

/*
* 1.堆分配存储表示 与 定长顺序存储表示 都是一片连续的空间表示
* 2.不同的是堆分配存储 存储空间是在程序执行过程中动态分配的(堆区 malloc/free, new/delete)
* 3.只实现五项基本操作, 其他操作与定长顺序存储相同
*/

typedef struct
{
    char *ch;   // 这里指针只能为char*, 而非 ElemType*, 非空串则按串长分配存储区，否则 ch = NULL;
    int length; // 串长度 
}HString;  // Heap String

Status StrAssign(HString *T, char *chars)
{
    if (T->ch)  // 如果原来有空间，先释放
    {
        free(T->ch);
    }
    int len = 0;  // 计算 chars 长度
    for (char *c = chars; *c; ++c)
    {
        ++len;
    }
    if (!len)  // len = 0
    {
        T->ch = NULL;
        T->length = 0;
    }
    else
    {
        if (!(T->ch = (char*)malloc(sizeof(char))))
        {
            exit(OVERFLOW);
        }
        for (int i = 0; i < len; ++i)  // 元素从下标 0 开始存，i 不能等于 len
        {
            T->ch[i] = chars[i];
        }
        T->length = len;
    }
    return OK;
}

int StrLength(HString S)
{
    return S.length;
}

int StrCompare(HString S, HString T)
{
    for (int i = 0; i < S.length && i < T.length; ++i)
    {
        if (S.ch[i] != T.ch[i])  // 字符不同
        {
            return S.ch[i] - T.ch[i];
        }
    }
    return S.length - T.length;  // 字符都相同则比较长度
}

Status ClearString(HString *S)
{
    if (S->ch)
    {
        free(S->ch);
        S->ch = NULL;
    }
    S->length = 0;  // 逻辑清空
    return OK;
}

Status Concat(HString *T, HString S1, HString S2)
{
    if (T->ch)
    {
        free(T->ch);  // 释放旧空间
    }
    // 这里不用考虑阶段，可以根据 S1 和 S2 的长度动态给 T 分配存储空间
    if (!(T->ch = (char*)malloc((S1.length + S2.length) * sizeof(char))))
    {
        exit(OVERFLOW);
    }
    int i;
    for (i = 0; i < S1.length; ++i)  // 把 S1 复制进 T
    {
        T->ch[i] = S1.ch[i];
    }  // 循环结束 i = S1.length
    for (; i < S1.length + S2.length; ++i)  // 把 S2 复制进 T
    {
        T->ch[i] = S2.ch[i - S1.length];
    }
    T->length = S1.length + S2.length;
    return OK;
}

Status SubString(HString *Sub, HString S, int pos, int len)
{
    if (pos < 1 || pos > S.length || len < 0 || len > S.length - pos + 1)  // pos 或 len 不合法
    {
        return ERROR;
    }
    if (Sub->ch)
    {
        free(Sub->ch);  // 释放原有空间
    }
    if (len == 0)
    {
        Sub->ch = NULL;
        Sub->length = 0;
    }
    else
    {
        // 动态给 Sub 分配存储空间
        Sub->ch = (char*)malloc(len * sizeof(char));
        for (int i = 0; i < len; ++i, ++pos)
        {
            Sub->ch[i] = S.ch[pos - 1];
        }
        Sub->length = len;
    }
    return OK;
}