#include <iostream>
using namespace std;

int c = 0;

void move(char x, int n, char z)
{
    cout << "Step " << ++c << " :  " << "Move disk " << n << " from " << x << " to " << z << endl;
}

void hanoi(int n, char x, char y, char z)
{
    if (n == 1)
    {
        move(x, 1, z);
    }
    else
    {
        hanoi(n - 1, x, z, y);
        move(x, n, z);
        hanoi(n - 1, y, x, z);
    }
}

int main()
{
    int n = 2;
    hanoi(n, 'X', 'Y', 'Z');

    // system("pause");
    return 0;
}