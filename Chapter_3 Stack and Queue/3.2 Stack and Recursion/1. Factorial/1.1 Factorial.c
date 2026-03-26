#include <stdio.h>

// 求阶乘
int Fact(int n)
{
    if (n == 0)
    {
        return 1;
    }
    else
    {
        return n * Fact(n - 1);
    }
}

int main()
{
    printf("你要求哪个数的阶乘: ");
    int m = 0;
    scanf("%d", &m);
    int res = Fact(m);
    printf("%d 的阶乘 = %d\n", m, res);
    system("pause");
    return 0;
}