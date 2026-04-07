#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

#define MAXSTRLEN 255                           // 最大串长 255 -- 超过则截断
typedef unsigned char SString[MAXSTRLEN + 1];   // 下标为 0 的位置存储串的长度

int Index_BF(SString S, SString T, int pos)
{
    if (T[0] == 0) 
    {
        return pos;  // 空串视为在pos位置匹配
    }
    if (pos < 1 || pos > S[0])  // pos 不合法
    {
        return 0;
    }
    // BF 算法
    int i = pos;
    int j = 1;
    while (i <= S[0] && j <= T[0])
    {
        printf("[debug] i=%d(S[%d]='%c'), j=%d(T[%d]='%c')\n", i, i, S[i], j, j, T[j]);  // 调试行
        if (S[i] == T[j])
        {
            ++i;
            ++j;
        }
        else  // 有字符不相等，回溯，下一位置
        {
            i = i - j + 2;  // (i-j+1)本轮 i 的起始位置，+1后为 i 的下一位置
            j = 1;          // j 从首元素开始比较
        }
    }
    if (j > T[0])
    {
        return i - T[0];  // 或者返回 i - j + 2
    }
    else
    {
        return 0;
    }
}

// 辅助函数：创建串（长度+内容）
void StrAssign(SString S, const char *chars)
{
    int i = 0;
    while (chars[i] && i < MAXSTRLEN)
    {
        S[i + 1] = chars[i];  // 从下标1开始存字符
        i++;
    }
    S[0] = i;  // 下标0存长度
}

// 辅助函数：打印串
void StrPrint(SString S)
{
    printf("长度=%d, 内容=\"", S[0]);
    for (int i = 1; i <= S[0]; i++)
        putchar(S[i]);
    printf("\"\n");
}

int main()
{
    SString S, T;
    int pos, result;
    
    printf("========== BF算法测试 ==========\n");
    
    // 测试1：基础匹配（中间匹配）
    StrAssign(S, "ababcabcacbab");
    StrAssign(T, "abcac");
    pos = 1;
    result = Index_BF(S, T, pos);
    printf("测试1: S=\"ababcabcacbab\", T=\"abcac\", pos=%d\n", pos);
    printf("结果: %d (期望: 6)\n\n", result);
    
    // 测试2：从头匹配
    StrAssign(S, "hello world");
    StrAssign(T, "hello");
    result = Index_BF(S, T, 1);
    printf("测试2: S=\"hello world\", T=\"hello\", pos=1\n");
    printf("结果: %d (期望: 1)\n\n", result);
    
    // 测试3：匹配失败
    StrAssign(S, "abcdef");
    StrAssign(T, "xyz");
    result = Index_BF(S, T, 1);
    printf("测试3: S=\"abcdef\", T=\"xyz\", pos=1\n");
    printf("结果: %d (期望: 0)\n\n", result);
    
    // 测试4：从指定位置开始匹配
    StrAssign(S, "aaaabaaab");
    StrAssign(T, "aab");
    result = Index_BF(S, T, 3);  // 从第3个字符开始找
    printf("测试4: S=\"aaaabaaab\", T=\"aab\", pos=3\n");
    printf("结果: %d (期望: 3)\n\n", result);
    
    // 测试5：模式串为空（边界情况）
    StrAssign(S, "test");
    StrAssign(T, "");  // 空串
    result = Index_BF(S, T, 1);
    printf("测试5: 空模式串匹配\n");
    printf("结果: %d (期望: 1，空串视为在任意位置匹配)\n\n", result);
    
    // 测试6：pos参数越界
    StrAssign(S, "test");
    StrAssign(T, "es");
    result = Index_BF(S, T, 10);  // pos > S长度
    printf("测试6: pos越界 (pos=10, S长度=4)\n");
    printf("结果: %d (期望: 0)\n", result);
    
    printf("\n========== 测试结束 ==========\n");
    system("pause");
    return 0;
}