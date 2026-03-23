/*
* 
* 当前代码为C风格(严格按照原文)代码
* 
*/

#include <iostream>
#include <cstdlib>  // malloc, free, exit
#include <cstdio>   // printf
using namespace std;

#define TRUE        1   // 真/是
#define FALSE       0   // 假/否
#define OK          1   // 通过/成功
#define ERROR       0   // 错误/失败
#define INFEASIBLE  -1  // 不可行的
#define OVERFLOW    -2  // 堆栈上溢

// ElemType 可以进行自定义, 比如 int、char等，我这里定义为int
typedef int ElemType;
typedef int Status;  // 状态码

typedef ElemType * Triplet;  // Triplet 是一个指向 ElemType 的指针

Status InitTriplet(Triplet &T, ElemType v1, ElemType v2, ElemType v3);
Status DestroyTriplet(Triplet &T);
Status Get(Triplet &T, int i, ElemType &e);
Status Put(Triplet &T, int i, ElemType e);
Status IsAscending(Triplet T);
Status IsDescending(Triplet T);
Status Max(Triplet T, ElemType &e);
Status Min(Triplet T, ElemType &e);

Status InitTriplet(Triplet &T, ElemType v1, ElemType v2, ElemType v3)
{
    T = (ElemType *) malloc (3 * sizeof(ElemType));  // 分配 3 个 ElemType 的存储空间
    if (!T)
    {
        exit(OVERFLOW);  // 分配存储空间失败
    }
    T[0] = v1;
    T[1] = v2;
    T[2] = v3;
    return OK;
}

Status DestroyTriplet(Triplet &T)
{
    free(T);  // 释放内存
    T = NULL;  // NULL C风格, 避免野指针
    return OK;
}

Status Get(Triplet &T, int i, ElemType &e)
{
    if (i < 1 || i > 3)
    {
        return ERROR;
    }
    e = T[i - 1];
    return OK;
}

Status Put(Triplet &T, int i, ElemType e)
{
    if (i < 1 || i > 3)
    {
        return ERROR;
    }
    T[i - 1] = e;
    return OK;
}

Status IsAscending(Triplet T)
{
    // 升序返回 1, 否则返回 0
    return (T[0] <= T[1]) && (T[1] <= T[2]);  // 注意可以 =
}
Status IsDescending(Triplet T)
{
    // 降序返回 1, 否则返回 0
    return (T[0] >= T[1]) && (T[1] >= T[2]);
}

Status Max(Triplet T, ElemType &e)
{
    // 嵌套三元运算符 —— 先比较两个, 胜者再比第三个
    e = (T[0] >= T[1]) ? (T[0] >= T[2] ? T[0] : T[2]) : (T[1] >= T[2] ? T[1] : T[2]);
    // 等价于
    // ElemType bigger = T[0] >= T[1] ? T[0] : T[1];
    // e = bigger >= T[2] ? bigger : T[2];
    return OK;
}

Status Min(Triplet T, ElemType &e)
{
    e = (T[0] <= T[1]) ? (T[0] <= T[2] ? T[0] : T[2]) : (T[1] <= T[2] ? T[1] : T[2]);
    return OK;
}

// 主函数测试
int main()
{
    Triplet T;
    ElemType e;
    Status result;

    cout << "=== 初始化三元组 ===" << endl;
    result = InitTriplet(T, 10, 20, 15);
    if (result == OK) cout << "初始化成功: (10, 20, 15)" << endl;

    cout << "\n=== 获取第 2 个元素 ===" << endl;
    Get(T, 2, e);
    cout << "第 2 个元素的值是: " << e << endl;

    cout << "\n=== 修改第 3 个元素为 5 ===" << endl;
    Put(T, 3, 5);
    Get(T, 3, e);
    cout << "修改后第 3 个元素的值是: " << e << endl;
    // 此时三元组为 [10, 20, 5]

    cout << "\n=== 判断顺序 ===" << endl;
    if (IsAscending(T)) 
        cout << "当前是升序" << endl;
    else 
        cout << "当前不是升序" << endl;

    if (IsDescending(T)) 
        cout << "当前是降序" << endl;
    else 
        cout << "当前不是降序" << endl;

    cout << "\n=== 最值查询 ===" << endl;
    Max(T, e);
    cout << "最大值是: " << e << endl;
    Min(T, e);
    cout << "最小值是: " << e << endl;

    cout << "\n=== 销毁三元组 ===" << endl;
    DestroyTriplet(T);
    if (T == NULL) cout << "销毁成功，指针已置空" << endl;

    system("pause");
    return 0;
}