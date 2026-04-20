#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/*
* 1.顺序存储结构的二叉树只适用于完全二叉树，对于一般二叉树会浪费大量的存储空间，不常用，一轮先不做实现
* 
* 2.对于顺序存储结构的二叉树就可以用二叉树的性质5直接找双亲或者左右孩子（顺序存储下标对应编号），时间复杂度为O(1)
*/

#define MAX_TREE_SIZE 100                       // 二叉树的最大结点数
typedef char TElemType;
typedef TElemType SqBiTree[MAX_TREE_SIZE];      // 0 号结点存储根节点

SqBiTree bt;                                    