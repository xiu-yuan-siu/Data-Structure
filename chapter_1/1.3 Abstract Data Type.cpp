#include <iostream>
using namespace std;

typedef struct
{
    float real_part;
    float imag_part;
} Complex;

// 赋值
void assign (Complex *A, float real, float imag)
{
    A->real_part = real;
    A->imag_part = imag;
}
// +
void add (Complex *c, Complex A, Complex B)
{
    c->real_part = A.real_part + B.real_part;
    c->imag_part = A.imag_part + B.imag_part;
    /* End of Add() */
}
// -
void minus (Complex *c, Complex A, Complex B)
{
    c->real_part = A.real_part - B.real_part;
    c->imag_part = A.imag_part - B.imag_part;
}
// multiply
void multiply (Complex *c, Complex A, Complex B)
{
    c->real_part = A.real_part*B.real_part - A.imag_part*B.imag_part;
    c->imag_part = A.real_part*B.imag_part + A.imag_part*B.real_part;
}
// divide
bool divide (Complex *c, Complex A, Complex B)
{
    if (B.real_part == 0 && B.imag_part == 0)
    {
        return false;
    }
    else
    {
        c->real_part = (A.real_part * B.real_part + A.imag_part * B.imag_part) / (B.real_part * B.real_part + B.imag_part * B.imag_part);
        c->imag_part = (A.imag_part * B.real_part - A.real_part * B.imag_part) / (B.real_part * B.real_part + B.imag_part * B.imag_part);
        return true;
    }
}

void GetReal(Complex &c, float &real)  // float的real必须传引用
{
    real = c.real_part;
}

void GetImag(Complex &c, float &imag)
{
    imag = c.imag_part;
}

int main()
{
    Complex z1, z2, z3, z4, z;
    float RealPart = 0, ImagPart = 0;
    assign(&z1, 8.0, 6.0);
    assign(&z2, 4.0, 3.0);
    add(&z3, z1, z2);
    multiply(&z4, z1, z2);
    if (divide(&z, z4, z3))
    {
        GetReal(z, RealPart);
        GetImag(z, ImagPart);
        cout << "结果: " << RealPart << " + " << ImagPart << "i" << endl;
    }
    else
    {
        cout << "分母为 0, 不能进行除法!" << endl;
    }
    
    system("pause");
    return 0;
}
