#include <stdio.h>
#include <stdlib.h>  // system()
#include <windows.h>  // 设置输出 UTF-8
#include "1. HString.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);    // 设置输出为UTF-8

    HString S1 = {NULL, 0};  // 必须初始化为空
    HString S2 = {NULL, 0};
    HString T = {NULL, 0};
    
    StrAssign(&S1, "Hello");
    StrAssign(&S2, "World");
    
    printf("S1长度: %d\n", StrLength(S1));
    printf("S2长度: %d\n", StrLength(S2));
    
    Concat(&T, S1, S2);
    printf("联接结果: ");
    for (int i = 0; i < T.length; i++) {
        putchar(T.ch[i]);
    }
    printf("\n");
    
    // 清理内存
    ClearString(&S1);
    ClearString(&S2);
    ClearString(&T);
    
    system("pause");
    return 0;
}