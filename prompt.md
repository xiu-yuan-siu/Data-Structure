1. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错

2. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

```c
// 稀疏矩阵乘法：经典算法（教材简化版）
Status MultiSMatrix(TSMatrix M, TSMatrix N, TSMatrix *Q)
{
    if (!Q || M.nu != N.mu) return ERROR;  // 列数≠行数，无法相乘
    
    Q->mu = M.mu; Q->nu = N.nu; Q->tu = 0;
    
    // 对M的每一行
    for (int i = 1; i <= M.mu; i++) {
        ElemType ctemp[MAXSIZE + 1] = {0};  // 临时存储结果行
        
        // 遍历M第i行的所有非零元
        for (int p = 1; p <= M.tu; p++) {
            if (M.data[p].i != i) continue;  // 跳过非本行元素
            int col = M.data[p].j;           // M的列 = N的行
            ElemType val = M.data[p].e;
            
            // 遍历N中第col行的所有非零元，累加到ctemp
            for (int q = 1; q <= N.tu; q++) {
                if (N.data[q].i == col) {
                    ctemp[N.data[q].j] += val * N.data[q].e;
                }
            }
        }
        
        // 将ctemp中非零元存入Q（保持行优先）
        for (int j = 1; j <= Q->nu; j++) {
            if (ctemp[j] != 0) {
                Q->data[++Q->tu] = (Triple){i, j, ctemp[j]};
            }
        }
    }
    return OK;
}
```

```cpp

```
