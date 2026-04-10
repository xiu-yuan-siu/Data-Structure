#ifndef HSTRING_H
#define HSTRING_H
#include "../../Status.h"

typedef struct
{
    char *ch;   // 这里指针只能为char*, 而非 ElemType*, 非空串则按串长分配存储区，否则 ch = NULL;
    int length; // 串长度 
}HString;  // Heap String

// 生成一个其值等于串常量chars的串T
Status StrAssign(HString *T, const char *chars);

// 返回串S的元素个数（串长度）
int StrLength(HString S);

// 若S>T，返回值>0；若S=T，返回0；若S<T，返回值<0
int StrCompare(HString S, HString T);

// 将S清为空串
Status ClearString(HString *S);

// 用T返回由S1和S2联接而成的新串
Status Concat(HString *T, HString S1, HString S2);

// 用Sub返回串S的第pos个字符起长度为len的子串
Status SubString(HString *Sub, HString S, int pos, int len);

Status StrCopy(HString *T, HString S);

#endif