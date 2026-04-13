#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

#define MAXSTRLEN 255                           // 最大串长 255 -- 超过则截断
typedef unsigned char SString[MAXSTRLEN + 1];   // 下标为 0 的位置存储串的长度

/*
T    =   "abcac" (T[0]=5)
j    =    12345
next[j] = 01112

初始化: i=1, j=0, next[1]=0

第1轮: i=1, j=0 
  j==0 → i=2, j=1, next[2]=1  // 前两个数必定是 0 1

第2轮: i=2, j=1, T[2]='b', T[1]='a'
  不匹配 → j=next[1]=0

第3轮: i=2, j=0
  j==0 → i=3, j=1, next[3]=1  // 上一轮得到结果不匹配在这里给 next[j] 数组赋值

第4轮: i=3, j=1, T[3]='c', T[1]='a'
  不匹配 → j=next[1]=0

第5轮: i=3, j=0
  j==0 → i=4, j=1, next[4]=1

第6轮: i=4, j=1, T[4]='a', T[1]='a'
  匹配! → i=5, j=2, next[5]=2

结束: i=5 == T[0]=5, 退出
*/

void get_next(SString T, int next[])
{
    int i = 1;
    next[1] = 0;
    int j = 0;
    while (i < T[0])  // 自匹配
    {
        if (j == 0 || T[i] == T[j])  // // 所有的 next[j](代码中通过next[i]赋值) 都在这赋值
        {
            ++i;
            ++j;
            next[i] = j;
        }
        else  // j != 0 && T[i] != T[j]，书中 p_k != p_j 的情况
        {
            j = next[j];  // 不断向前找，前面匹配成功的部分，有没有更短的相等前后缀可以利用？有就继续用，没有就回到起点（0）
        }
    }
}

int Index_KMP(SString S, SString T, int pos, int next[])
{
    // 先检查 pos 是否合法
    if (pos < 1 || pos > S[0])
    {
        return 0;
    }
    if (T[0] == 0)
    {
        return pos;  // 空串视为在 pos 进行匹配
    }
    // KMP
    int i = pos;
    int j = 1;
    while (i <= S[0] && j <= T[0])
    {
        if (j == 0 || S[i] == T[j])  // 注意：这里多了 j = 0的情况， next[j] 可以得到 0
        {
            ++i;
            ++j;
        }
        else  // 不等时，i不懂，只移动模式串(j)，把模式串的第 next[j](k) 个元素与 i 比较
        {
            j = next[j];
        }
    }
    if (j > T[0])   // 匹配成功
    {
        return i - T[0];
    }
    else            // 匹配失败
    {
        return 0;
    }
}

// ========== 辅助函数 ==========
void StrAssign(SString S, const char *chars)
{
    int i;
    for (i = 0; chars[i] && i < MAXSTRLEN; i++) {
        S[i + 1] = chars[i];
    }
    S[0] = i;
}

void StrPrint(SString S)
{
    printf("长度=%d, 内容=\"", S[0]);
    for (int i = 1; i <= S[0]; i++) putchar(S[i]);
    printf("\"\n");
}

// 打印next数组（调试用）
void PrintNext(SString T, int next[])
{
    printf("模式串: "); StrPrint(T);
    printf("next[]:  [ ");
    for (int j = 1; j <= T[0]; j++) printf("%d ", next[j]);
    printf("]\n\n");
}

int main()
{
    SString S, T;
    int next[MAXSTRLEN + 1], result, pos;
    
    printf("========== KMP算法测试 ==========\n\n");
    
    // 测试1：经典案例（教材例题）
    printf("【测试1】教材经典案例\n");
    StrAssign(S, "ababcabcacbab");
    StrAssign(T, "abcac");
    get_next(T, next);
    PrintNext(T, next);  // 期望: [0 1 1 1 2]
    result = Index_KMP(S, T, 1, next);
    printf("匹配结果: %d (期望: 6)\n\n", result);
    
    // 测试2：含重复前缀（验证next回溯）
    printf("【测试2】重复前缀：T=\"aaaab\"\n");
    StrAssign(S, "aaaaaaab");
    StrAssign(T, "aaaab");
    get_next(T, next);
    PrintNext(T, next);  // 期望: [0 1 2 3 4]
    result = Index_KMP(S, T, 1, next);
    printf("匹配结果: %d (期望: 4)\n\n", result);
    
    // 测试3：匹配失败
    printf("【测试3】无匹配情况\n");
    StrAssign(S, "abcdefg");
    StrAssign(T, "xyz");
    get_next(T, next);
    result = Index_KMP(S, T, 1, next);
    printf("匹配结果: %d (期望: 0)\n\n", result);
    
    // 测试4：从指定位置开始
    printf("【测试4】pos=3开始匹配\n");
    StrAssign(S, "aaaabaaab");
    StrAssign(T, "aab");
    get_next(T, next);
    result = Index_KMP(S, T, 3, next);
    printf("匹配结果: %d (期望: 3)\n\n", result);
    
    // 测试5：单字符模式串
    printf("【测试5】单字符匹配\n");
    StrAssign(S, "hello");
    StrAssign(T, "e");
    get_next(T, next);
    result = Index_KMP(S, T, 1, next);
    printf("匹配结果: %d (期望: 2)\n\n", result);
    
    // 测试6：空模式串
    printf("【测试6】空模式串边界\n");
    StrAssign(S, "test");
    StrAssign(T, "");
    result = Index_KMP(S, T, 1, next);
    printf("匹配结果: %d (期望: 1)\n\n", result);
    
    printf("========== 测试结束 ==========\n");
    system("pause");
    return 0;
}
