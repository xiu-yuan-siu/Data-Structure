#include <stdio.h>
#include <stdlib.h>  // exit，system
#include <string.h>
#include "../../Status.h"

#define MAXSTRLEN 255  // 最大串长 255 -- 超过则截断
typedef unsigned char SString[MAXSTRLEN + 1];  // 下标为 0 的位置存储串的长度

/* ================== 5 个串类型的最小操作子集 ================== */
// 1.串赋值
Status StrAssign(SString *T, const char *chars)
{
    int len = strlen(chars);
    if (len > MAXSTRLEN)
    {
        exit(OVERFLOW);
    }
    (*T)[0] = len;
    for (int i = 1; i <= len; i++)
    {
        (*T)[i] = chars[i - 1];
    }
    return OK;
}

// 2.串比较 -- 若 S > T 返回值 >0；若 S = T 返回 0；若 S < T 返回值 <0
int StrCompare(SString S, SString T)
{
    for (int i = 1; i <= S[0] && i <= T[0]; i++)
    {
        if (S[i] != T[i])  // 字符不同
        {
            return S[i] - T[i];
        }
    }
    return S[0] - T[0];  // 每个字符相同，但长度不同
}

// 3.求串长
int StrLength(SString S)
{
    return S[0];
}

// 4.串联接
Status Concat(SString *T, SString S1, SString S2)
{
    Status unset;  // 变量 -- 没有被截断
    if (S1[0] + S2[0] <= MAXSTRLEN)  // 无截断
    {
        int i;  // 在整个 if 语句内都可用
        for (i = 1; i <= S1[0]; i++)
        {
            (*T)[i] = S1[i];
        }  // 循环结束后 i = S1[0] + 1;
        for (; i <= S1[0] + S2[0]; i++)
        {
            (*T)[i] = S2[i - S1[0]]; 
        }
        (*T)[0] = S1[0] + S2[0];  // 新串的长度
        unset = TRUE; 
    }
    else if (S1[0] < MAXSTRLEN)  // S2 被截断 (S1[0] + S2[0] > MAXSTRLEN)
    {
        int i;
        for (i = 1; i < S1[0]; i++)
        {
            (*T)[i] = S1[i];
        }
        for (; i <= MAXSTRLEN; i++)
        {
            (*T)[i] = S2[i - S1[0]];
        }
        (*T)[0] = MAXSTRLEN;  // 新串的长度
        unset = FALSE;  
    }
    else  // S1 被截断 S1[0] >= MAXSTRLEN
    {
        for (int i = 1; i <= MAXSTRLEN; i++)
        {
            (*T)[i] = S1[i];
        }
        (*T)[0] = MAXSTRLEN;
        unset = FALSE;
    }
    return unset;
}

// 5.求子串 -- 用 Sub 返回串 S 的第 pos 个字符起长度为 len 的子串
Status SubString(SString *Sub, SString S, int pos, int len)
{
    // 先判断 pos 和 len 是否合法
    if (pos < 1 || pos > S[0] || len < 0 || len > S[0] - pos + 1)
    {
        return ERROR;
    }
    for (int i = 1; i <= len; i++, pos++)
    {
        (*Sub)[i] = S[pos];
    }
    (*Sub)[0] = len;
    return OK;
}

/* ================== 基于上述 5 个函数实现的功能 ================== */
// 6.串拷贝
Status StrCopy(SString *T, SString S)
{
    for (int i = 0; i <= S[0]; i++)
    {
        (*T)[i] = S[i];
    }
    return OK;
}

// 7.串判空
bool StrEmpty(SString S)
{
    return S[0] == 0 ? TRUE : FALSE;
}

// 8.返回串 T 在主串 S 中第 pos 个字符之后第一次出现的位置
int Index(SString S, SString T, int pos)
{
    if (pos < 1 || pos > S[0] || T[0] == 0)  // pos 不合法 或 T 为空串
    {
        return 0;
    }
    int n = StrLength(S);
    int m = StrLength(T);
    int i = pos;
    while (i <= n - m + 1)  // 从 pos 开始不断后移，直至在 S 中能找出子串 T
    {
        SString sub;
        SubString(&sub, S, i, m);  // 传参数接受为指针时，要传入地址
        if (StrCompare(sub, T) != 0)  // sub 与 T 不等
        {
            i++;
        }
        else
        {
            return i;
        }
    }
    return 0;  // 未在 S 中找到子串 T
}

// 提前声明，在替换函数中要用到
Status StrInsert(SString *S, int pos, SString T);
Status StrDelete(SString *S, int pos, int len);

// 9.用 V 替换主串 S 中出现的所有与 T 相等的不重叠子串
// 利用插入和删除完成
Status Replace(SString *S, SString T, SString V)
{
    if (T[0] == 0)
    {
        return ERROR;
    }
    int i = 0, pos = 1, index;  // i 计算空间, 保证发生溢出时报错
    while ((index = Index(*S, T, pos)) != 0)  // 在 S 中找到子串 T
    {
        i++;
        // 在 S 中删除子串 T
        if (StrDelete(S, index, T[0]) == ERROR)
        {
            return ERROR;
        }
        // 在 S 中插入子串 V
        if (StrInsert(S, index, V) == ERROR)
        {
            return ERROR;
        }
        pos = index + V[0];  // 保证不发生重叠
    }  // 长度在插入和删除的函数调用中已经更新
    if ((*S)[0] > MAXSTRLEN)  // 溢出直接退出
    {
        exit(OVERFLOW);
    }
    return OK;
}

// 10.在串 S 的第 pos 个字符之前插入串 T
Status StrInsert(SString *S, int pos, SString T)
{
    if (pos < 1 || pos > (*S)[0] || (*S)[0] + T[0] == MAXSTRLEN)  // pos 不合法，或空间已满，插入元素会溢出
    {
        return ERROR;
    }
    // 把原下标为 pos~len 的元素后移 T[0](子串的长度) 个单位(下标)
    for (int i = (*S)[0]; i >= pos; i--)
    {
        (*S)[i + T[0]] = (*S)[i];
    }
    // 插入子串 T
    for (int i = 1; i <= T[0]; i++, pos++)
    {
        (*S)[pos] = T[i];
    }
    (*S)[0] += T[0];  // 更新长度
    return OK;
}

// 11.从串 S 中删除第 pos 个字符起长度为 len 的子串
Status StrDelete(SString *S, int pos, int len)
{
    if (pos < 0 || pos > (*S)[0] || len < 0 || len > (*S)[0] - pos + 1)  // S 为空串 或 pos/len 不合法
    {
        return ERROR;
    }
    // 下标从 pos + len 到 (*S)[0] 的所有元素前移 len 个单位覆盖
    for (int i = pos + len; i <= (*S)[0]; i++)
    {
        (*S)[i - len] = (*S)[i];
    }
    (*S)[0] -= len;  // 更新长度
    return OK;
}

/* ================== 无法基于上面 5 个函数实现的功能 ================== */
// 12.清空 -- 定长顺序串无动态内存，只需进行逻辑置空
Status ClearString(SString *S)
{
    (*S)[0] = 0;
    return OK;
}

// 13.销毁 -- 定长顺序串无动态内存，只需进行逻辑置空
Status DestroyString(SString *S)
{
    (*S)[0] = 0;
    return OK;
}

/*==================== 辅助输出 ====================*/
void PrintString(SString S) {
    int i;
    for (i = 1; i <= S[0]; i++) {
        printf("%c", S[i]);
    }
    printf(" (length=%d)\n", S[0]);
}

/*==================== 测试主函数 ====================*/
int main() {
    SString S, T, V, Sub, Copy, Cat;

    printf("===== 1. StrAssign =====\n");
    StrAssign(&S, "hello");
    StrAssign(&T, "world");
    PrintString(S);
    PrintString(T);

    printf("\n===== 2. StrCopy =====\n");
    StrCopy(&Copy, S);
    PrintString(Copy);

    printf("\n===== 3. StrEmpty =====\n");
    printf("S is empty? %s\n", StrEmpty(S) ? "TRUE" : "FALSE");
    ClearString(&Copy);
    printf("Copy is empty? %s\n", StrEmpty(Copy) ? "TRUE" : "FALSE");

    printf("\n===== 4. StrCompare =====\n");
    printf("Compare(S,T) = %d\n", StrCompare(S, T));

    printf("\n===== 5. StrLength =====\n");
    printf("Length of S = %d\n", StrLength(S));

    printf("\n===== 6. ClearString =====\n");
    StrCopy(&Copy, S);
    ClearString(&Copy);
    PrintString(Copy);

    printf("\n===== 7. Concat =====\n");
    Concat(&Cat, S, T);
    PrintString(Cat);

    printf("\n===== 8. SubString =====\n");
    SubString(&Sub, Cat, 2, 4); // "ello"中的一部分
    PrintString(Sub);

    printf("\n===== 9. Index =====\n");
    StrAssign(&S, "ababcabcacbab");
    StrAssign(&T, "abcac");
    PrintString(S);
    PrintString(T);
    printf("Index(S,T,1) = %d\n", Index(S, T, 1));

    printf("\n===== 10. StrInsert =====\n");
    StrAssign(&S, "HelloWorld");
    StrAssign(&T, " ");
    StrInsert(&S, 6, T);
    PrintString(S);

    printf("\n===== 11. StrDelete =====\n");
    StrDelete(&S, 6, 1);
    PrintString(S);

    printf("\n===== 12. Replace =====\n");
    StrAssign(&S, "abc123abc456abc");
    StrAssign(&T, "abc");
    StrAssign(&V, "XYZ");
    Replace(&S, T, V);
    PrintString(S);

    printf("\n===== 13. DestroyString =====\n");
    DestroyString(&S);
    PrintString(S);

    system("pause");
    return 0;
}