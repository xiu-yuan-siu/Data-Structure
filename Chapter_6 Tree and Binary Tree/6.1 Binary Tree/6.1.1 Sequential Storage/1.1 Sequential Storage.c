#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

/*
* 
* 顺序存储结构的二叉树只适用于完全二叉树，对于一般二叉树会浪费大量的存储空间，不常用，一轮先不做实现
* 
*/

#define MAX_TREE_SIZE 100                       // 二叉树的最大结点数
typedef int TElemType;
typedef TElemType SqBiTree[MAX_TREE_SIZE];      // 0 号结点存储根节点

SqBiTree bt;                                    