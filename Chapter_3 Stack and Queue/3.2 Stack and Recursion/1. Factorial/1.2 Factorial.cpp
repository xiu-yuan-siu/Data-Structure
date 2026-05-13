#include <iostream>
using namespace std;

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
    cout << "你要求哪个数的阶乘: ";
    int n = 0;
    cin >> n;
    int res = Fact(n);
    cout << n << " 的阶乘 = " << res << endl;
    
    // system("pause");
    return 0;
}