#include <stdio.h>

/*
* 顺序查找/线性查找
* 1.优点：算法理解，实现简单，适用面广(无序表有序表，顺序表和线性链表均适用)
* 2.缺点：平均查找长度大
* 3.如何解决各个记录查找频率不等：设访问频度域，访问概率大的查找距离短，放在后面(使用的顺序查找为倒序查找)，以便提高查找效率
*/

typedef int KeyType;
// typedef char *KeyType;      // 字符串类型 -- 注意这里用指针表示char数组，即字符串

// 数据元素类型定义
typedef struct {
    KeyType key;                // 关键字域
    // 其他域，如访问频度域（解决查找概率不等的情况，访问概率大的查找距离短，放在后面，依次提高查找效率）
} ElemType;

typedef struct {
    ElemType *elem;             // 数组元素存储空间基址，建表时按实际长度分配，based-1，0 号位置用作哨兵
    int length;
} SSTable;

// 倒序查找
int Search_Seq(SSTable ST, KeyType key) {
    ST.elem[0].key = key;                                  // 哨兵(Sentinal) -- 避免 i 的边界处理，保证就算没找到对应的 key 也会在 i = 0 时停止
    int i;
    for (i = ST.length; !(ST.elem[i].key == key); --i);    // 与下面的Search_Seq_2函数对比，可以看到使用"哨兵"只用比较一次(不用关心i的边界处理)
    return i;                                              // 未找到时, i = 0
}

int Search_Seq_2(SSTable ST, KeyType key) {
    for (int i = ST.length; i >= 1; --i) {
        if (ST.elem[i].key == key) {
            return i;
        }
    }
    return 0;
}