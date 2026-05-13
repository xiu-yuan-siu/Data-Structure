#include <stdio.h>
#include <stdlib.h>
#include <math.h>           // abs

#define N 4                 // N 皇后问题

int x[N + 1];               // 全局变量，x[j]记录第 j 行皇后所在列号(1-based -- 即 0 下标不用，从 1 开始)
int solutionCount = 0;      // 全局变量，解的数量

// 检查第 i 行第 j 列是否安全 -- 即在当前位置放第 i 个皇后是否与之前放的 i-1 个皇后冲突
int place(int i, int j) {   // 第 i 行第 j 列是否安全
    for (int k = 1; k < i; ++k) {
        // 行肯定不冲突，只用检查列冲突和对角线冲突
        if (x[k] == j || abs(i - k) == abs(j - x[k])) {     // 前 i-1(k遍历) 个皇后的列(x[k]) 与要防止的位置的列 j 冲突
            return 0;       // 冲突
        }
    }
    return 1;               // 不冲突
}

void Trial(int i, int n) {  // i -- 当前要为第 i 位皇后选择位置，n -- 皇后总数(棋盘大小 n x n)
    if (i > n) {            // n 行都放置完毕，输出解
        ++solutionCount;
        printf("解 %d (", solutionCount);
        // 输出 x[j] 第 j 行的皇后放置在第 x[j] 列
        for (int j = 1; j <= n; ++j) {
            printf("%d", x[j]);
            if (j < n)
                printf(", ");
        }
        printf(") (输出 x[j](1-based), 第 j 行的皇后放置在第 x[j] 列)\n");
        // 输出棋盘
        for (int row = 1; row <= n; ++row) {
            for (int col = 1; col <= n; ++col) {
                if (x[row] == col) {
                    printf(" Q ");
                } else {
                    printf(" · ");
                }
            }
            printf("\n");       // 输出一行换行
        }
        printf("\n");           // 输出棋盘换行
        return;
    } else {                    // 为第i行尝试每一列j（1~n）
        for (int j = 1; j <= n; ++j) {
            x[i] = j;       // 尝试第i行放到第j列
            if (place(i, j)) {  // 安全，则放下一个(i + 1)
                Trial(i + 1, n);
            }
            // 不安全或递归返回：尝试下一列（隐式回溯，x[i]被覆盖）
        }
    }
}

int main() {
    printf("===== %d皇后问题（严书6.7节 Trial算法）=====\n\n", N);
    
    // 初始化
    for (int i = 1; i <= N; i++) {
        x[i] = 0;
    }
    
    // 从第1行开始
    Trial(1, N);
    
    if (solutionCount == 0) {
        printf("无解！\n");
    } else {
        printf("共找到 %d 个解\n", solutionCount);
    }
    
    // system("pause");
    return 0;
}