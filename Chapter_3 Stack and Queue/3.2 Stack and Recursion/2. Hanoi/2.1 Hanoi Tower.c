#include <stdio.h>
#include <stdlib.h>

// 全局变量 c，对搬动计数
int c = 0;

// char x, y, z 为三个塔座, int n 为圆盘编号

// 将 盘n 从 塔座x 移动到 塔座z
void move(char x, int n, char z)
{
    printf("%i. Move disk %i from %c to %c\n", ++c, n, x, z);  // 第几次搬动 将哪个盘(n) 从 x 塔座搬到了 z 塔座
}

// 以 塔座y 为中间塔座, 将 n个盘 从 塔座x 移动到 塔座 z
void hanoi(int n, char x, char y, char z)
{
    if (n == 1)
    {
        move(x, 1, z);  // 将编号为 1 的盘 从 x 移动到 z
    }   
    else
    {
        hanoi(n - 1, x, z, y);  // 以 z 为辅助塔座 把 x上 编号为 1~n-1 个盘移动到 y
        move(x, n, z);          // 把 编号为 n 的盘 从 x 移动到 z
        hanoi(n - 1, y, z, x);  // 以 x 为辅助塔座 把 y上 编号为 1~n-1 个盘移动到 z, 这一步会直接调用递归
    }
}

int main()
{
    int n = 3;
    hanoi(3, 'X', 'Y', 'Z');

    system("pause");
    return 0;
}