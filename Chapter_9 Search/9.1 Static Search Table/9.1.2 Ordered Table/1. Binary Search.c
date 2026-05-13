#include <stdio.h>

/*
* 折半查找/二分查找
* 1.优点：查找效率高，平均查找长度 ASL = log2(n+1) - 1(n 较大时近似了)
* 2.缺点：仅适用于有序表，且限于顺序存储结构，线性链表无法有效进行二分查找(必须顺着指针域一步一步移动，low/high不能像数组一样下标改变)
*/

typedef int KeyType;

// 数据元素类型定义
typedef struct {
    KeyType key;                // 关键字域
    // 其他域，如访问频度域（解决查找概率不等的情况，访问概率大的查找距离短，放在后面，依次提高查找效率）
} ElemType;

typedef struct {
    ElemType *elem;             // 数组元素存储空间基址，建表时按实际长度分配，based-1，0 号位置用作哨兵
    int length;
} SSTable;

int Search_Bin(SSTable ST, KeyType key) {
    int low = 1, high = ST.length;
    while (low < high) {
        int mid = (low + high) / 2;
        if (key == ST.elem[mid].key) 
            return mid;
        else if (key < ST.elem[mid].key)            // key 在前半区间，则更新 high
            high = mid - 1;
        else                                        // key 在后半区间，则更新 low
            low = mid + 1;
    }
    return 0;                                       // 顺序表中不存在待查元素
}