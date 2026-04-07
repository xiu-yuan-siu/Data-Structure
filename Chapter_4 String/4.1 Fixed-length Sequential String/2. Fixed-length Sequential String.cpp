#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace std;

class SString
{
private:
    static const int MAXSTRLEN = 255;
    unsigned char S[MAXSTRLEN + 1];  // S[0] 存长度
public:
    SString()
    {
        S[0] = 0;
    }

};