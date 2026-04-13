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
    while (i < T[0]) {
        if (j == 0 || T[i] == T[j]) {
            ++i;
            ++j;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

void get_nextval(SString T, int nextval[])
{
    int i = 1;
    nextval[1] = 0;
    int j = 0;
    while (i < T[0]) {
        if (j == 0 || T[i] == T[j]) {
            ++i;
            ++j;
            if (T[i] != T[j])
                nextval[i] = j;
            else
                nextval[i] = nextval[j];  // 优化：跳过重复
        } else {
            j = nextval[j];
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