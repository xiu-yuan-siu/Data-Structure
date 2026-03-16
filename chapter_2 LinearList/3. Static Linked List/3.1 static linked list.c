#include <stdio.h>
#include <stdlib.h>
#include "../Status.h"

#define MAXSIZE 1000  // 链表最大长度

typedef int ElemType;

typedef struct 
{
    ElemType data;
    int cur;
}component, SLinkList[MAXSIZE];  // static linklist

void InitList(SLinkList space)
{
    // space[0].cur 为头指针
    for (int i = 0; i < MAXSIZE - 1; ++i)
    {
        space[i].cur = i + 1;
    }
    // 备用链表最后一个节点
    space[MAXSIZE - 1].cur = 0;  // 0 表示 空指针 NULL
}

void DestroyList(SLinkList space, int *S)
{
    InitList(space);  // 直接重新初始化整个空间
    *S = 0;  // 头结点置空
}

int Malloc_SL(SLinkList space)
{
    // 若备用空间链表非空，则返回分配的结点下标，否则返回0
    int i = space[0].cur;
    if (space[0].cur)
    {
        space[0].cur = space[i].cur;  // 再次让备用链表头结点指向空闲下标 0 . 1 . 2 . ...
    }
    return i;
}

void Free_SL(SLinkList space, int k)
{
    // 将下标为 k 的结点头插回备用链表头结点
    space[k].cur = space[0].cur;  // 让下标为 k 的结点变为可用结点
    space[0].cur = k;  // 让头指针指向可用结点(下标为 k)
}

int LocateElem(SLinkList space, int H, ElemType *e)
{
    // 在静态单链表中查找第一个为e的元素的下标
    // 找到返回在 L 中的位序，未找到返回 0
    int i = space[H].cur;
    while (i && space[i].data != *e)  // 头指针为空 或 找到第一个为 e 的元素则退出循环
    {
        i = space[i].cur;  // 头指针移动
    }
    return i;
}

Status ListInsert(SLinkList space, int S, int i, ElemType *e)
{
    // 备用链表space, 数据链表S, 将元素 e 插入位序为 i 的位置
    int j = S;  // 头指针
    for (int k = 1; k < i && j; ++k)// 找到第 i - 1 个结点(k < i), 且输入的 i 合法(j != 0)
    {
        j = space[j].cur;
    }
    if (!j)  // j 为空, 即用户输入的 i 值偏大
    {
        return ERROR;
    }
    int l = Malloc_SL(space);  // 从备用链表给新节点分配空间
    if (l)  // 如果空间分配成功(可能空间已经满了)
    {
        space[l].data = *e;
        space[l].cur = space[j].cur;  // 让插入的结点指向第 i+1 个结点(原第 i 个结点)
        space[j].cur = l;  // 让第 i-1 个结点指向当前的第 i 个结点
        return OK;
    }
    return ERROR;
}

Status ListDelete(SLinkList space, int S, int i, ElemType *e)
{
    int j = S;
    for (int k = 1; k < i && j; ++k)  // 找到第 i-1 个结点
    {
        j = space[j].cur;
    }
    int l = space[j].cur;  // 第 i 个结点
    if (l)  // 要判断 l 是否为 0，即 i 值偏大
    {
        *e = space[l].data;  // 记录第 i 个结点的数据
        space[j].cur = space[l].cur;  // 让第 i-1 个结点指向第 i+1 个结点
        Free_SL(space, l);  // 将结点回收回备用链表
        return OK;
    }
    return ERROR;
}

// 头插法建表, 返回头指针
int CreateList(SLinkList space, int n)
{
    int S = Malloc_SL(space);
    space[S].cur = 0;
    for (int i = 0; i < n; ++i)
    {
        ElemType val;
        scanf("%d", &val);
        ListInsert(space, S, 1, &val);  // 参数 1 表示插入位序为 1 -- 即 头插法
    }
    return S;  // 返回头指针
}

void difference(SLinkList space, int *S)
{
    // 依次输入集合A和B的元素，在一维数组space(备用链表)中建立表示集合(A-B)U(B-A)的静态链表(头指针为 S)
    // 假设备用空间足够大, space[0].cur为备用链表space头指针
    InitList(space);  // 初始化备用链表
    *S = Malloc_SL(space);  // 初始化数据链表
    int r = *S;  // r 永远指向 S 的最后一个结点, 尾指针
    int m, n;
    printf("请依次输入集合A、B元素的个数: ");
    scanf("%d %d", &m, &n);
    // 输入集合 A
    printf("请输入集合A: ");
    for (int j = 0; j < m; ++j)
    {
        int i = Malloc_SL(space);
        scanf("%d", &space[i].data);  // 注意: 前面定义 ElemType 为 int，这里才是%d, 若为其他，要做相应更改
        space[r].cur = i; // 插入表尾
        // 更新尾指针
        r = i;
    }
    space[r].cur = 0;  // 尾指针置空
    // 输入集合B
    printf("请输入集合B: ");
    for (int j = 0; j < n; ++j)
    {
        int i = Malloc_SL(space);
        ElemType b;
        scanf("%d", &b);
        int k = space[*S].cur;  // k 指向当前 A 链表的第一个节点，用于遍历
        int p = *S;  // p 为 k 的前驱, 便于删除A中与B重复的元素
        // 遍历整个 A ，查找是否有重复的元素
        while (k != 0 && space[k].data != b)
        {
            p = k;  // 移动 k 的前驱 p 到 当前k
            k = space[k].cur;  // ++k
        }
        if (!k)  // k == 0, 即未找到重复元素 -- 插入 b 到表尾 r 之后，且 r 的位置不变
        {
            int i = Malloc_SL(space);
            space[i].data = b;
            space[i].cur = space[r].cur;
            space[r].cur = i;
        }
        else  // 找到重复元素 -- 删除存在的重复元素
        {
            space[p].cur = space[k].cur;
            Free_SL(space, k);
            if (k == r)  // 如果删除的是表尾
            {
                r = p;  // 修改尾指针 r, 让尾指针指向 k 的前驱 p
            }
        }
    }
}

int main() {
    SLinkList space;
    int S;

    // 1. 初始化测试
    InitList(space);
    printf("--- 静态链表初始化完成 ---\n");

    // 2. 插入功能测试 (正常与边界)
    // 假设 S 为数据链表头结点
    S = Malloc_SL(space); 
    space[S].cur = 0;
    
    ElemType v1 = 10, v2 = 20, v3 = 30;
    ListInsert(space, S, 1, &v1); // 插入 10
    ListInsert(space, S, 2, &v2); // 插入 20
    printf("插入10, 20后，头结点索引为%d, 第一个元素为%d\n", S, space[space[S].cur].data);

    // 3. 报错测试：插入位置越界
    if (ListInsert(space, S, 5, &v3) == ERROR) {
        printf("报错测试成功：位置5越界插入失败\n");
    }

    // 4. 查找功能测试
    ElemType target = 20;
    int pos = LocateElem(space, S, &target);
    printf("查找元素20: %s (下标:%d)\n", pos ? "找到" : "未找到", pos);

    // 5. 删除功能测试
    ElemType deleted_val;
    if (ListDelete(space, S, 1, &deleted_val) == OK) {
        printf("删除第一个元素成功，值为: %d\n", deleted_val);
    }

    // 6. 报错测试：删除不存在的位置
    if (ListDelete(space, S, 10, &deleted_val) == ERROR) {
        printf("报错测试成功：位置10不存在，删除失败\n");
    }

    // 7. 集合差测试 (A-B)U(B-A)
    printf("\n--- 开始测试集合差运算 ---\n");
    // 注意：运行此函数需要手动输入数据
    // 输入示例：
    // 3 2 (A有3个元素，B有2个)
    // 1 2 3 (A的元素)
    // 2 4 (B的元素)
    // 预期结果应该是链表中剩下 1, 3, 4
    int S_diff;
    difference(space, &S_diff);
    
    printf("集合差运算结果链表: ");
    int p = space[S_diff].cur;
    while(p) {
        printf("%d ", space[p].data);
        p = space[p].cur;
    }
    printf("\n");
    system("pause");
    return 0;
}