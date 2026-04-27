/*
 * 注：
 * 本次修订的目的包括降低耦合，争取每个模块都可以单独运行
 * 但是Status这个模块会被所有其他模块引用，引用次数很多。
 * 如果直接将Status模块复制到其它模块中，则会导致太多重复代码，
 * 因此这里生成一个公共静态库让其它模块共享比较划算
 */

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