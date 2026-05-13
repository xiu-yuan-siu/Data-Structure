#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 先包含 Status.h，确保 ElemType 和 Status 定义在前
#include "../../../Status.h"
#include "../../4.2 Head Allocation String/1. HString.h"  // HString
#include "../../../Chapter_2 Linear List/2. Linked List/2.1 LinkList.h"  // LinkList


#define MaxBookNum 1000  // 对 1000 本书建立索引表
#define MaxKeyNum 2500   // 索引表最大容量
#define MaxLineLen 500   // 书目串的最大长度
#define MaxWordNum 10    // 词表的最大容量(关键词)

typedef struct
{
    char* item[MaxWordNum];     // 字符串数组
    int last;                   // 词表的长度
} WordListType;                 // 词表类型(顺序表)

typedef int ElemType;   // 定义链表的数据元素类型为整型(书号类型)

typedef struct 
{
    HString key;        // 关键词
    LinkList bnolist;   // 存放书号索引的链表
} IdxTermType;          // 索引项类型

typedef struct
{
    IdxTermType item[MaxKeyNum + 1];    // 索引表关键词
    int last;                           // 索引表长度
} IdxListType;                          // 索引表类型(有序表)

// 主要变量
char *buf;      // 书目串缓冲区 buffer
WordListType wdlist;        // 词表

// 常用词表
const char *stopWords[] = {"a", "an", "the", "of", "to", "and", "in", "for", "on", "with", "by", NULL};

// 函数声明
void GetWord(int i, HString *wd);
int Locate(IdxListType idxlist, HString wd, bool *b);
void InsertNewKey(IdxListType *idxlist, int i, HString wd);
Status InsertBook(IdxListType *idxlist, int i, int bno);
bool IsStopWord(char *word);
void ExtractKeyWord(ElemType *bno);
void InitIdxList(IdxListType *idxlist);
bool GetLine(FILE *f);
Status InsIdxList(IdxListType *idxlist, ElemType bno);
void PutText(FILE *g, IdxListType idxlist);

// =================== 基本操作 ===================
// 1.初始化操作，置索引表 idxlist 为空表，且在 idxlist.item[0] 设一空串
void InitIdxList(IdxListType *idxlist)
{
    idxlist->last = 0;
    // 初始化所有项的指针为 NULL，避免野指针
    for (int i = 0; i <= MaxKeyNum; i++) {
        idxlist->item[i].key.ch = NULL;
        idxlist->item[i].key.length = 0;
        idxlist->item[i].bnolist = NULL;
    }
    StrAssign(&(idxlist->item[0].key), "");  // HString
    InitList(&(idxlist->item[0].bnolist));   // LinkList
}

// 2.从文件 f 读入一个书目信息到书目串缓冲区 buf
bool GetLine(FILE *f)
{
    /*
    * char *fgets(char *str, int n, FILE *stream);
    * 从文件流中读取一行字符串（包括换行符 \n），直到遇到以下情况之一停止：
    * 1.读取了 n-1 个字符
    * 2.遇到换行符 \n
    * 3.到达文件末尾 EOF
    * 成功：返回 str（缓冲区的首地址）
    * 失败/文件结束：返回 NULL    
    */
    if (fgets(buf, MaxLineLen, f) != NULL) 
    {
        int len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') 
        {
            buf[len-1] = '\0';
        }
        return true;
    } 
    else 
    {
        buf[0] = '\0';  // 读取失败，清空 buf
        return false;
    }
}

// 判断是否为常用词
bool IsStopWord(char *word)
{
    for (int i = 0; stopWords[i] != NULL; ++i)
    {
        if (strcmp(word, stopWords[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

// 3.从 buf 中提取书名关键词到词表 wdlist，书号存入 bno
// 书目文件: 例: 005 Computer Data Structures
void ExtractKeyWord(ElemType *bno)
{
    wdlist.last = 0;
    char *p = buf;
    // 跳过空格/制表符 到 书号
    while (*p == ' ' || *p == '\t')
    {
        ++p;
    }
    *bno = 0;
    while (*p >= '0' && *p <= '9')
    {
        *bno = *bno * 10 + (*p - '0');  // char 转 int
        ++p;
    }
    // 跳过空格/制表符 到书名
    while (*p == ' ' || *p == '\t')
    {
        ++p;
    }

    char word[MaxLineLen];
    int wordlen = 0;

    while (*p != '\0')
    {
        if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z'))
        {
            word[wordlen++] = (*p >= 'A' && *p <= 'Z') ? (*p - 'A' + 'a') : *p;  // 如果是大写字母则转换为小写字母
        }
        else  // 读完一个词了
        {
            if (wordlen > 0)
            {
                word[wordlen] = '\0';
                if (!IsStopWord(word) && wdlist.last < MaxWordNum)
                {
                    wdlist.item[wdlist.last] = (char*)malloc((strlen(word) + 1) * sizeof(char));  // sizeof(char) = 1 可省略
                    strcpy(wdlist.item[wdlist.last], word);  // 将关键词加入词表
                    ++wdlist.last;
                }
                wordlen = 0;
            }
        }
        ++p;
    }  // 循环推出 word 还表示着一个词
    // 把最后一个词加入词表 wdlist
    if (wordlen > 0)
    {
        word[wordlen] = '\0';
        if (!IsStopWord(word) && wdlist.last < MaxWordNum)
        {
            wdlist.item[wdlist.last] = (char*)malloc(strlen(word) + 1);
            strcpy(wdlist.item[wdlist.last], word);
            ++wdlist.last;
        }
        wordlen = 0;
    }
}

// 4.将书号为 bno 的书名关键词按词典顺序插入索引表 idxlist
Status InsIdxList(IdxListType *idxlist, ElemType bno)
{
    HString wd;
    wd.ch = NULL;
    wd.length = 0;

    for (int i = 0; i < wdlist.last; ++i)
    {
        bool b;
        GetWord(i, &wd);
        int j = Locate(*idxlist, wd, &b);
        if (!b)  // 关键词 wd 不存在索引表中 -- 插入新关键词索引
        {
            InsertNewKey(idxlist, j, wd);
        }
        if (!InsertBook(idxlist, j, bno))  // 插入新书号索引
        {
            ClearString(&wd);
            return OVERFLOW;
        }
    }
    ClearString(&wd);
    return OK;
}

// 5.将生成的索引表 idxlist 输入到文件 g
void PutText(FILE *g, IdxListType idxlist)
{
    for (int i = 1; i <= idxlist.last; ++i)
    {
        for (int k = 0; k < idxlist.item[i].key.length; ++k)
        {
            // 写关键词
            fprintf(g, "%c", idxlist.item[i].key.ch[k]);  // fprintf 向文件中写 file printf
        }
        // 写书号
        LNode* p = idxlist.item[i].bnolist->next;
        while (p != NULL)
        {
            fprintf(g, ",%03d", p->data);
            p = p->next;
        }
        fprintf(g, "\n");  // 写完一行(一行即一个关键词及其对应的书号)
    }
}

// =================== 为在索引表上进行插入要实现的操作 ===================
// 6.用 wd 返回词表 wdlist 中第 i 个关键词
void GetWord(int i, HString *wd)
{
    char *p = wdlist.item[i];   // 取词表中第 i 个字符串
    StrAssign(wd, p);           // 生成关键字字符串
}

// 7.在索引表 idxlist 中查询是否存在与 wd 相等的关键词(若存在，则返回其在索引表中的位置,且 b 取值 TRUE; 否则返回插入位置，且 b 取值 FALSE)
int Locate(IdxListType idxlist, HString wd, bool *b)
{
    int i;
    int m = 0;
    for (i = idxlist.last; i >= 1; --i)
    {
        m = StrCompare(idxlist.item[i].key, wd);
        if (m == 0)  // 找到与 wd 相等的 key
        {
            *b = TRUE;
            return i;
        }
        if (m < 0)  // i 对应的 key < wd
        {
            break;  // 退出循环(m > 0 继续循环)
        }
    }  // 退出循环 
    *b = FALSE;  // i 对应的 key < wd, 插入位置为 i + 1
    return i + 1;
}

// 8.在索引表 idxlist 的第 i 项上插人新关键词 wd，并初始化书号索引的链表为空表
void InsertNewKey(IdxListType *idxlist, int i, HString wd)
{
    // 索引项后移一个位置
    for (int j = idxlist->last; j >= i; --j)
    {
        idxlist->item[j + 1] = idxlist->item[j];
    }

    // 关键：断开 item[i] 和 item[i+1] 的 key 共享，防止 StrCopy 时释放 item[i+1] 的内存
    idxlist->item[i].key.ch = NULL;
    idxlist->item[i].key.length = 0;
    idxlist->item[i].bnolist = NULL;

    // 插入新的索引项
    StrCopy(&(idxlist->item[i].key), wd);
    InitList(&(idxlist->item[i].bnolist));
    ++(idxlist->last);
}

// 9.在索引表 idxlist 的第 i 项中插人书号为 bno 的索引
Status InsertBook(IdxListType *idxlist, int i, int bno)
{
    // idxlist->item[i].bnolist -- 第 i 个关键词对应的书号链表的头节点，其 data 域存链表长度
    int pos = idxlist->item[i].bnolist->data + 1;
    return ListInsert(idxlist->item[i].bnolist, pos, &bno);
}

// =================== 主函数 ===================
int main()
{
    FILE *f, *g;
    IdxListType idxlist;
    ElemType BookNo;
    
    printf("程序开始运行...\n");  // 添加调试输出

    // 为 buf 分配内存空间（使用指针形式必须分配）
    buf = (char*)malloc(MaxLineLen * sizeof(char));
    if (buf == NULL) {
        printf("内存分配失败\n");
        return 1;
    }

    printf("正在打开 BookInfo.txt...\n");  // 添加调试输出
    
    if ((f = fopen("BookInfo.txt", "r")) == NULL) {
        printf("无法打开输入文件 BookInfo.txt\n");
        free(buf);  // 释放已分配的内存
        return 1;
    }

    printf("BookInfo.txt 打开成功\n");  // 添加调试输出
    
    if ((g = fopen("BookIdx.txt", "w")) == NULL) {
        printf("无法打开输出文件 BookIdx.txt\n");
        fclose(f);
        free(buf);  // 释放已分配的内存
        return 1;
    }
    
    InitIdxList(&idxlist);
    
    while (GetLine(f), buf[0] != '\0') {  // 先执行 GetLine，再检查是否为空
    ExtractKeyWord(&BookNo);
    
    if (wdlist.last > 0) {
        if (InsIdxList(&idxlist, BookNo) != OK) {
            printf("插入索引失败，可能是内存溢出\n");
            break;
        }
    }
    }
    
    PutText(g, idxlist);
    
    fclose(f);
    fclose(g);
    
    // 释放 buf 内存
    free(buf);
    
    printf("索引表生成完成！共 %d 个关键词\n", idxlist.last);
    
    // system("pause");
    return 0;
}