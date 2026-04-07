#include <stdio.h>
#include <stdlib.h>
#include "../../Status.h"

/*
* 类似链表(可自由选择单链表，双向链表，循环链表等)，略
*/

#define CHUNKSIZE 80  // 块大小

typedef struct Chunk
{
    char ch[CHUNKSIZE];
    struct Chunk *next;
} Chunk;

typedef struct 
{
    Chunk *head, *tail;  // 串的 头、尾指针
    int curlen;          // 当前长度, current length
} LString;  // Linked String
