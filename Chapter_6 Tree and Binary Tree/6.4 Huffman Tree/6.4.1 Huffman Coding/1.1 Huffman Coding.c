#include <stdio.h>
#include <stdlib.h>
#include <string.h>         // strcpy
#include "../../../Status.h"

typedef struct {
    unsigned int weight;
    unsigned int parent, lchild, rchild;
} HTNode, *HuffmanTree;         // 动态分配数组存储赫夫曼树 -- 前 n 个结点为叶子结点，最后一个结点为根结点，中间为分支结点

typedef char **HuffmanCode;     // 动态分配数组存储赫夫曼编码表(二维数组)

// 辅助函数: 选择权值最小的两个结点
void Select(HuffmanTree HT, int n, int *s1, int *s2) {
    int i;
    // min1 为最小权值, min2 为第二小的权值
    unsigned int min1 = 0xFFFFFFFF, min2 = 0xFFFFFFFF;  // 0xFFFFFFFF(16进制)表示 32 位无符号整数能表示的最大值 -- 4,294,967,295(4字节)
    *s1 = *s2 = 0;
    for (int i = 1; i <= n; ++i) {
        if (HT[i].parent == 0) {                            // 保证选择的是结点，不是树
            if (HT[i].weight < min1) {                      // 当前 i 的权值为最小，改变最小和第二小的权值和对应的下标
                min2 = min1;                                // 先更新第二小的权值
                *s2 = *s1;                                  // 更新第二小的权值对应的下标
                min1 = HT[i].weight;                        // 再更新最小权值
                *s1 = i;                                    // 更新最小权值对应的下标
            } else if (HT[i].weight < min2) {               // 当前 i 的权值比最小的大，比第二小的小，只改动第二小的权值及对应的下标
                min2 = HT[i].weight;
                *s2 = i;
            }
        }
    }
}

// 构造赫夫曼树并计算编码 -- 算法 6.12
void HuffmanCoding(HuffmanTree *HT, HuffmanCode *HC, int *w, int n) {   // n 为叶子结点数
    if (n <= 1) return;
    int m = 2 * n - 1;              // 总结点数
    *HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));        // 0 号单元未使用
    HuffmanTree p;
    int i;
    // 初始化赫夫曼树
    for (p = *HT + 1, i = 1; i <= n; ++i, ++p, ++w) {           // 初始化 n 个叶子结点，这里 p 指向第一个单元故 *HT 要 + 1(0 号单元未使用)
        p->weight = *w;
        p->parent = p->lchild = p->rchild = 0;
    }
    for (; i <= m; ++i, ++p) {                                  // 初始化后续分支结点
        p->weight = p->parent = p->lchild = p->rchild = 0;
    }
    // 构建赫夫曼树 -- 建立下标关系
    for (i = n + 1; i <= m; ++i) {                              // 叶子节点的初始化就是构建, parent, lchild, rchild的值就为 0，现在构建分支结点
        int s1, s2;
        Select(*HT, i - 1, &s1, &s2);                           // 从HT[1..i-1]选择parent=0且权值最小的两个结点(也可能是树)
        (*HT)[s1].parent = i;
        (*HT)[s2].parent = i;
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
    // 从叶子结点到根你想求每个字符的赫夫曼编码
    *HC = (HuffmanCode)malloc((n + 1) * sizeof(char*));
    char *cd = (char*)malloc(n * sizeof(char));             // 临时存放编码
    cd[n - 1] = '\0';                                       // 编码结束符，编码位数最多为 (n-1) 位
    for (i = 1; i <= n; ++i) {                              // 从 n 个叶子结点一步一步向上逐个字符求赫夫曼编码
        int start = n - 1;                                  // 编码结束符位置
        for(int c = i, f = (*HT)[i].parent; f != 0; c = f, f = (*HT)[f].parent) {       // c 为当前结点，f 为 c 的双亲
            if ((*HT)[f].lchild == c) {                     // c 为 f 的左孩子，编码为 '0'
                cd[--start] = '0';
            } else {                                        // c 为 f 的右孩子，编码为 '1'
                cd[--start] = '1';
            }
        }
        // 给赫夫曼编码的一维数组分配空间
        (*HC)[i] = (char*)malloc((n - start) * sizeof(char));
        strcpy((*HC)[i], &cd[start]);
    }
    free(cd);
}

/* ============= 无栈非递归遍历赫夫曼树求赫夫曼编码 ============= */
// 1.构造赫夫曼树 -- 算法 6.12 前半部分
void CreateHuffmanTree(HuffmanTree *HT, int *w, int n) {
    if (n <= 1) return;
    int m = 2 * n - 1;
    *HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode));
    HuffmanTree p;
    int i;
    for (i = 1, p = *HT + 1; i <= n; ++i, ++p, ++w) {
        *p = (HTNode){(unsigned int)*w, 0, 0, 0};
    }
    for (; i <= m; ++i, ++p) {
        *p = (HTNode){0, 0, 0, 0};
    }
    for (i = n + 1; i <= m; ++i) {
        int s1, s2;
        Select(*HT, i - 1, &s1, &s2);
        (*HT)[s1].parent = i;
        (*HT)[s2].parent = i;
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
}

// 2.无栈非递归遍历赫夫曼树求赫夫曼编码 -- 算法 6.13
void HuffmanCoding_NoRecursive(HuffmanTree HT, HuffmanCode *HC, int n) {
    // 从根出发，遍历整棵赫夫曼树
    *HC = (HuffmanCode)malloc((n + 1) * sizeof(char*));
    char *cd = (char*)malloc(n * sizeof(char));
    int m = 2 * n - 1;
    int p = m, cdlen = 0;           // p 从根结点出发
    // 遍历赫夫曼树求编码
    for (int i = 1; i <= m; ++i) {
        HT[i].weight = 0;           // 借用 weight 用作结点遍历状态标志
    }
    while (p) {
        if (HT[p].weight == 0) {                // 向左
            HT[p].weight = 1;
            if (HT[p].lchild != 0) {
                p = HT[p].lchild;
                cd[cdlen++] = '0';
            } else if (HT[p].rchild == 0) {     // 登记叶子结点(lchild = rchild = 0)的字符的编码
                (*HC)[p] = (char*)malloc((cdlen + 1) * sizeof(char));
                cd[cdlen] = '\0';               // 编码结束符
                strcpy((*HC)[p], cd);           // (*HC)[p] 为数组名，数组名即指针
                // 叶子结点后不立即cdlen--，留到父结点回溯时统一处理
            }
        } else if (HT[p].weight == 1) {         // 向右
            HT[p].weight = 2;
            if (HT[p].rchild != 0) {
                p = HT[p].rchild;
                cd[cdlen++] = '1';
            }
            // 如果无右孩子（比如只有左子树的结构），直接跳到else分支回溯
        } else {                            // HT[p].weight == 2, 则退回到父节点，编码长度减 1
            HT[p].weight = 0;
            p = HT[p].parent;
            --cdlen;
            // 例如从"101"退回到"10"，准备探索父结点的另一分支
        }
    }
    free(cd);
}

void HuffmanDecoding(HuffmanTree HT, int n, char *code, char *chars) {
    // 根据待编译的二进制字符串code走，每走到一个叶子结点即编译了一个字符通过chars取出，再重新回到根结点，沿着code继续走编译下一个字符
    int p = 2 * n - 1;
    printf("编译结果: ");
    for (int i = 0; code[i] != '\0'; ++i) {
        if (code[i] == '0') {
            p = HT[p].lchild;
        } else {                // code[i] == '1'
            p = HT[p].rchild;
        }
        if (HT[p].lchild == 0 && HT[p].rchild == 0) {       // 到达叶子结点
            printf("%c", chars[p - 1]);                     // code 从 0 开始，chars 从 0 开始，所以 p(1-2n-1) 要减 1
            p = 2 * n - 1;                                  // 回到根结点编译下一个字符，只有从根节点到叶子结点才是一个字符的完整编码
        }
    }
    printf("\n");
}

// ================== 测试案例：严书第三版148页例6-2 ==================
int main() {
    // 例6-2：8个字符及其权值
    int w[] = {5, 29, 7, 8, 14, 23, 3, 11};
    char ch[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}; // 0号不用，从1开始对应
    int n = sizeof(w) / sizeof(w[0]);

    printf("【严书第三版 例6-2】字符权值: ");
    for (int i = 0; i < n; ++i) printf("%c=%d ", ch[i+1], w[i]);
    printf("\nn = %d\n\n", n);

    printf("==== 测试 1: 算法 6.12 (从叶子到根逆向求编码) ====\n");
    HuffmanTree HT1;
    HuffmanCode HC1;
    HuffmanCoding(&HT1, &HC1, w, n);

    printf("字符\t权值\t赫夫曼编码\n");
    for (int i = 1; i <= n; ++i) {
        printf("%c\t%d\t%s\n", ch[i], HT1[i].weight, HC1[i]);
    }

    // 释放内存
    for (int i = 1; i <= n; ++i) free(HC1[i]);
    free(HC1);
    free(HT1);

    printf("\n==== 测试 2: 算法 6.13 (无栈非递归正向遍历求编码) ====\n");
    HuffmanTree HT2;
    HuffmanCode HC2;
    CreateHuffmanTree(&HT2, w, n);

    // 算法6.13会把 HT[i].weight 当作状态标志清零，这里先保存原始权值
    unsigned int *savedWeight = (unsigned int*)malloc((n + 1) * sizeof(unsigned int));
    for (int i = 1; i <= n; ++i) savedWeight[i] = HT2[i].weight;

    HuffmanCoding_NoRecursive(HT2, &HC2, n);

    printf("字符\t权值\t赫夫曼编码\n");
    for (int i = 1; i <= n; ++i) {
        printf("%c\t%u\t%s\n", ch[i], savedWeight[i], HC2[i]);
    }

    // 释放内存
    free(savedWeight);
    for (int i = 1; i <= n; ++i) free(HC2[i]);
    free(HC2);
    free(HT2);

    printf("\n==== 测试 3: 译码测试 (HuffmanDecoding) ====\n");
    // 重新建树用于译码（因为测试2的树被算法6.13修改过weight状态）
    HuffmanTree HT3;
    HuffmanCode HC3;
    HuffmanCoding(&HT3, &HC3, w, n);

    // 先打印编码表供参考
    printf("编码表:\n");
    for (int i = 1; i <= n; ++i) {
        printf("%c: %s\n", ch[i], HC3[i]);
    }

    // 构造测试编码串: "afhe" -> a(0110) + f(1100) + h(101) + e(111)
    // 注意: 不同Select实现可能导致编码不同，这里用实际生成的编码动态拼接
    char testCode[100] = "";
    // 测试译码 "face" 这4个字符: f + a + c + e
    strcat(testCode, HC3[6]);  // 'f' 是第6个字符 (w[5]=23)
    strcat(testCode, HC3[1]);  // 'a' 是第1个字符 (w[0]=5)
    strcat(testCode, HC3[3]);  // 'c' 是第3个字符 (w[2]=7)
    strcat(testCode, HC3[5]);  // 'e' 是第5个字符 (w[4]=14)

    printf("\n待译码二进制串: %s\n", testCode);
    printf("预期译码结果: face\n");
    HuffmanDecoding(HT3, n, testCode, ch);

    // 再测试一个: "bad"
    char testCode2[100] = "";
    strcat(testCode2, HC3[2]);  // 'b'
    strcat(testCode2, HC3[1]);  // 'a'
    strcat(testCode2, HC3[4]);  // 'd'

    printf("\n待译码二进制串: %s\n", testCode2);
    printf("预期译码结果: bad\n");
    HuffmanDecoding(HT3, n, testCode2, ch);

    // 释放内存
    for (int i = 1; i <= n; ++i) free(HC3[i]);
    free(HC3);
    free(HT3);

    system("pause");
    return 0;
}
