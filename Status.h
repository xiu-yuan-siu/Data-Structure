#ifndef STATUS_H
#define STATUS_H

#include <stdio.h>
#include <stdbool.h>

/* 状态码 */
#define TRUE        1   // 真/是
#define FALSE       0   // 假/否
#define OK          1   // 通过/成功
#define ERROR       0   // 错误/失败
#define INFEASIBLE  -1  // 不可行的
#define OVERFLOW    -2  // 堆栈上溢
#define UNDERFLOW   -3  // 下溢(数组顺序存储用到)

/* 状态码类型 */
typedef int Status;

/* 布尔类型 */
typedef bool Boolean;

#endif