#include <stdio.h>
#include <stdlib.h>

#define MAXSTRLEN 255
typedef unsigned char SString[MAXSTRLEN + 1];

// 统计结构体
typedef struct {
    int compare_count;      // 字符比较次数
    int backtrack_count;    // j回溯次数（即执行 j=next[j] 的次数）
    int total_steps;        // 总操作步数
} KMP_Stats;

void get_next(SString T, int next[])
{
    int i = 1;
    next[1] = 0;
    int j = 0;
    while (i < T[0])
    {
        if (j == 0 || T[i] == T[j]) 
        {
            ++i;
            ++j;
            next[i] = j;
        } 
        else 
        {
            j = next[j];
        }
    }
}

/*
主串 S = {"aaabaaaab"}
模式串 T = {"aaaab"}
nextval = [0, 0, 0, 0, 4]

我们使用 KMP 匹配过程（i 指向 S，j 指向 T，初始 i=1, j=1）：

#### 第一轮匹配：
*   S[1..4] 是 "aaab"，T[1..4] 是 "aaaa"。
*   i=1, j=1: 'a'=='a' -> i=2, j=2
*   i=2, j=2: 'a'=='a' -> i=3, j=3
*   i=3, j=3: 'a'=='a' -> i=4, j=4
*   i=4, j=4: S[4]='b', T[4]='a' -> 失配！

此时关键操作：
j 回溯。根据 nextval[4]，新的 j = {nextval}[4] = 0。
*(注意：如果是普通 next，j 会变成 3，然后比较 S[4] 和 T[3]('a')，依然失配，再变 2，再变 1，效率低。nextval 直接让 j 变为 0)*

当 j=0 时，KMP 算法规定：i 进位，j 复位为 1。
*   i 变为 5，j 变为 1。

#### 第二轮匹配：
当前 i=5 (S[5]='a'), j=1 (T[1]='a')。
*   i=5, j=1: 'a'=='a' -> i=6, j=2
*   i=6, j=2: S[6]='a', T[2]='a' -> 'a'=='a' -> i=7, j=3
*   i=7, j=3: S[7]='a', T[3]='a' -> 'a'=='a' -> i=8, j=4
*   i=8, j=4: S[8]='a', T[4]='a' -> 'a'=='a' -> i=9, j=5
*   i=9, j=5: S[9]='b', T[5]='b' -> 'b'=='b' -> 匹配成功！

此时 j > T.length，匹配结束。
匹配位置：i - T.length = 9 - 5 = 4? 不对，是起始索引。
起始索引 = i - m = 9 - 5 = 4 (如果下标从1开始，则是第5个位置开始？让我们复查一下 S 的下标)。

S:
1:a, 2:a, 3:a, 4:b, 5:a, 6:a, 7:a, 8:a, 9:b

匹配发生在 S[5 \dots 9] 即 "aaaab"。
起始下标是 5。
算法返回通常是 i - m (若 i 停在 m+1 处) 或者记录起始位置。在严蔚敏书中，通常返回的是主串中匹配开始的序号。
在此例中，当 j=5 匹配成功后，i 会加 1 变为 10，j 加 1 变为 6。循环结束。
返回位置：i - T.length = 10 - 5 = 5。正确。
*/

void get_nextval(SString T, int nextval[])
{
    int i = 1;
    nextval[1] = 0;
    int j = 0;
    while (i < T[0]) 
    {
        if (j == 0 || T[i] == T[j]) 
        {
            ++i;
            ++j;
            if (T[i] != T[j])  // i, j 都自增后的新位置字符比较
                nextval[i] = j;  // // 如果新位置的字符 T[i] 和回溯位置字符 T[j] 不同 -> 存 j (标准 next[i] = j)
            else
                nextval[i] = nextval[j];  // 如果相同 -> 存 nextval[j] (因为 T[i]==T[j]，回溯到 j 肯定也失配，不如直接跳到 j 该去的地方)
        } 
        else 
        {
            j = nextval[j];  // // 动作 C: 失配时，j 回溯
        }
    }
}

// 带统计的KMP
int Index_KMP_Stats(SString S, SString T, int pos, int next[], KMP_Stats *stats)
{
    stats->compare_count = 0;
    stats->backtrack_count = 0;
    stats->total_steps = 0;
    
    if (pos < 1 || pos > S[0]) return 0;
    if (T[0] == 0) return pos;
    
    int i = pos;
    int j = 1;
    
    while (i <= S[0] && j <= T[0]) {
        stats->total_steps++;
        
        if (j == 0) {
            // j=0时直接前进，不算比较
            ++i;
            ++j;
        } else {
            stats->compare_count++;  // 记录一次字符比较
            if (S[i] == T[j]) {
                ++i;
                ++j;
            } else {
                j = next[j];
                stats->backtrack_count++;  // 记录一次回溯
            }
        }
    }
    return (j > T[0]) ? (i - T[0]) : 0;
}

void StrAssign(SString S, const char *chars)
{
    int i;
    for (i = 0; chars[i] && i < MAXSTRLEN; i++) {
        S[i + 1] = chars[i];
    }
    S[0] = i;
}

void printArray(int arr[], int len, const char *name)
{
    printf("%s: [ ", name);
    for (int i = 1; i <= len; i++) printf("%d ", arr[i]);
    printf("]");
}

void run_comparison_test(const char *test_name, SString S, SString T)
{
    int next[MAXSTRLEN + 1], nextval[MAXSTRLEN + 1];
    KMP_Stats stats_next, stats_nextval;
    int result1, result2;
    
    printf("\n========== %s ==========\n", test_name);
    printf("主串 S: \""); 
    for (int i = 1; i <= S[0]; i++) putchar(S[i]); 
    printf("\" (长度%d)\n", S[0]);
    
    printf("模式 T: \"");
    for (int i = 1; i <= T[0]; i++) putchar(T[i]);
    printf("\" (长度%d)\n\n", T[0]);
    
    // 计算两种数组
    get_next(T, next);
    get_nextval(T, nextval);
    
    printArray(next, T[0], "next    ");
    printf("\n");
    printArray(nextval, T[0], "nextval ");
    printf("\n\n");
    
    // 分别运行并统计
    result1 = Index_KMP_Stats(S, T, 1, next, &stats_next);
    result2 = Index_KMP_Stats(S, T, 1, nextval, &stats_nextval);
    
    printf("【使用 next 数组】\n");
    printf("  匹配位置: %d\n", result1);
    printf("  字符比较次数: %d\n", stats_next.compare_count);
    printf("  j回溯次数: %d\n", stats_next.backtrack_count);
    printf("  总操作步数: %d\n", stats_next.total_steps);
    
    printf("\n【使用 nextval 数组】\n");
    printf("  匹配位置: %d\n", result2);
    printf("  字符比较次数: %d\n", stats_nextval.compare_count);
    printf("  j回溯次数: %d\n", stats_nextval.backtrack_count);
    printf("  总操作步数: %d\n", stats_nextval.total_steps);
    
    printf("\n【优化效果】\n");
    printf("  回溯减少: %d 次 (%.1f%%)\n", 
           stats_next.backtrack_count - stats_nextval.backtrack_count,
           stats_next.backtrack_count == 0 ? 0 : 
           (double)(stats_next.backtrack_count - stats_nextval.backtrack_count) 
           / stats_next.backtrack_count * 100);
    printf("  比较减少: %d 次\n", 
           stats_next.compare_count - stats_nextval.compare_count);
}

int main()
{
    SString S, T;
    
    printf("KMP 算法优化对比测试 —— next vs nextval\n");
    printf("==========================================\n");
    
    // 测试1：极端重复情况（最能体现优势）
    StrAssign(S, "aaaaaaab");
    StrAssign(T, "aaaab");
    run_comparison_test("测试1：极端重复字符 'aaaab' in 'aaaaaaab'", S, T);
    
    // 测试2：部分重复（教材经典对比案例）
    StrAssign(S, "aaabaaaab");
    StrAssign(T, "aaaab");
    run_comparison_test("测试2：前缀重复 'aaaab' in 'aaabaaaab'", S, T);
    
    // 测试3：交替模式
    StrAssign(S, "ababababcababab");
    StrAssign(T, "abababc");
    run_comparison_test("测试3：交替模式 'abababc' in 'ababababcababab'", S, T);
    
    // 测试4：无重复（优化不明显）
    StrAssign(S, "abcdefgh");
    StrAssign(T, "def");
    run_comparison_test("测试4：无重复字符 'def' in 'abcdefgh'", S, T);
    
    printf("\n==========================================\n");
    printf("结论：当模式串有大量重复字符时，nextval 能显著减少回溯次数！\n");
    system("pause");
    return 0;
}