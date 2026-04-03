#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

#define MAXSTRLEN 255  // 最大串长 255 -- 超过则截断
typedef unsigned char SString[MAXSTRLEN + 1];  // 下标为 0 的位置存储串的长度

/* ================== 5 个串类型的最小操作子集 ================== */
// 1.串赋值
Status StrAssign(SString *T, const char* chars)
{

}

// 2.串比较
int StrCompare(SString S, SString T)
{

}

// 3.求串长
int StrLength(SString S)
{

}

// 4.串联接
Status Concat(SString *T, SString S1, SString S2)
{

}

// 5.求子串
Status SubString(SString *Sub, SString S, int pos, int len)
{

}

/* ================== 基于上述 5 个函数实现的功能 ================== */
// 6.串拷贝
Status StrCopy(SString *T, SString S)
{

}

// 7.串判空
bool StrEmpty(SString S)
{

}

// 8.返回串 T 在主串 S 中第 pos 个字符之后第一次出现的位置
int Index(SString S, SString T, int pos)
{

}

// 9.用 V 替换主串 S 中出现的所有与 T 相等的不重叠子串
Status Replace(SString *S, SString T, SString V)
{

}

// 10.在串 S 的第 pos 个字符之前插入串 T
Status StrInsert(SString *S, int pos, SString T)
{

}

// 11.从串 S 中删除第 pos 个字符起长度为 len 的子串
Status StrDelete(SString *S, int pos, int len)
{

}

/* ================== 无法基于上面 5 个函数实现的功能 ================== */
// 12.清空
Status ClearString(SString *S)
{

}

// 13.销毁
Status DestroyString(SString *S)
{

}

/*==================== 辅助输出 ====================*/
void PrintString(SString S) {
    int i;
    for (i = 1; i <= S[0]; i++) {
        printf("%c", S[i]);
    }
    printf(" (length=%d)\n", S[0]);
}

/*==================== 测试主函数 ====================*/
int main() {
    SString S, T, V, Sub, Copy, Cat;

    printf("===== 1. StrAssign =====\n");
    StrAssign(&S, "hello");
    StrAssign(&T, "world");
    PrintString(S);
    PrintString(T);

    printf("\n===== 2. StrCopy =====\n");
    StrCopy(&Copy, S);
    PrintString(Copy);

    printf("\n===== 3. StrEmpty =====\n");
    printf("S is empty? %s\n", StrEmpty(S) ? "TRUE" : "FALSE");
    ClearString(&Copy);
    printf("Copy is empty? %s\n", StrEmpty(Copy) ? "TRUE" : "FALSE");

    printf("\n===== 4. StrCompare =====\n");
    printf("Compare(S,T) = %d\n", StrCompare(S, T));

    printf("\n===== 5. StrLength =====\n");
    printf("Length of S = %d\n", StrLength(S));

    printf("\n===== 6. ClearString =====\n");
    StrCopy(&Copy, S);
    ClearString(&Copy);
    PrintString(Copy);

    printf("\n===== 7. Concat =====\n");
    Concat(&Cat, S, T);
    PrintString(Cat);

    printf("\n===== 8. SubString =====\n");
    SubString(&Sub, Cat, 2, 4); // "ello"中的一部分
    PrintString(Sub);

    printf("\n===== 9. Index =====\n");
    StrAssign(&S, "ababcabcacbab");
    StrAssign(&T, "abcac");
    PrintString(S);
    PrintString(T);
    printf("Index(S,T,1) = %d\n", Index(S, T, 1));

    printf("\n===== 10. StrInsert =====\n");
    StrAssign(&S, "HelloWorld");
    StrAssign(&T, " ");
    StrInsert(&S, 6, T);
    PrintString(S);

    printf("\n===== 11. StrDelete =====\n");
    StrDelete(&S, 6, 1);
    PrintString(S);

    printf("\n===== 12. Replace =====\n");
    StrAssign(&S, "abc123abc456abc");
    StrAssign(&T, "abc");
    StrAssign(&V, "XYZ");
    Replace(&S, T, V);
    PrintString(S);

    printf("\n===== 13. DestroyString =====\n");
    DestroyString(&S);
    PrintString(S);

    return 0;
}