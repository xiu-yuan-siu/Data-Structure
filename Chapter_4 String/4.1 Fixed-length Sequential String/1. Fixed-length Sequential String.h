#include <stdio.h>
#include <stdlib.h>  // exit，system
#include <string.h>
#include "../../Status.h"

#define MAXSTRLEN 255  // 最大串长 255 -- 超过则截断
typedef unsigned char SString[MAXSTRLEN + 1];  // 下标为 0 的位置存储串的长度

/* ================== 5 个串类型的最小操作子集 ================== */
// 1.串赋值
Status StrAssign(SString *T, const char *chars);

// 2.串比较 -- 若 S > T 返回值 >0；若 S = T 返回 0；若 S < T 返回值 <0
int StrCompare(SString S, SString T);

// 3.求串长
int StrLength(SString S);

// 4.串联接
Status Concat(SString *T, SString S1, SString S2);

// 5.求子串 -- 用 Sub 返回串 S 的第 pos 个字符起长度为 len 的子串
Status SubString(SString *Sub, SString S, int pos, int len);

/* ================== 基于上述 5 个函数实现的功能 ================== */
// 6.串拷贝
Status StrCopy(SString *T, SString S);

// 7.串判空
bool StrEmpty(SString S);

// 8.返回串 T 在主串 S 中第 pos 个字符之后第一次出现的位置
int Index(SString S, SString T, int pos);

// 9.用 V 替换主串 S 中出现的所有与 T 相等的不重叠子串
// 利用插入和删除完成
Status Replace(SString *S, SString T, SString V);

// 10.在串 S 的第 pos 个字符之前插入串 T
Status StrInsert(SString *S, int pos, SString T);

// 11.从串 S 中删除第 pos 个字符起长度为 len 的子串
Status StrDelete(SString *S, int pos, int len);

/* ================== 无法基于上面 5 个函数实现的功能 ================== */
// 12.清空 -- 定长顺序串无动态内存，只需进行逻辑置空
Status ClearString(SString *S);

// 13.销毁 -- 定长顺序串无动态内存，只需进行逻辑置空
Status DestroyString(SString *S);

/*==================== 辅助输出 ====================*/
void PrintString(SString S);
