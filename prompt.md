1. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错

2. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

```c
#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 255
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int Status;
typedef int Bool;

/* 严蔚敏教材版：0号单元存长度 */
typedef unsigned char SString[MAXSTRLEN + 1];

/*==================== 辅助输出 ====================*/
void PrintString(SString S) {
    int i;
    for (i = 1; i <= S[0]; i++) {
        printf("%c", S[i]);
    }
    printf(" (length=%d)\n", S[0]);
}

/*==================== 1. StrAssign ====================*/
/* 生成一个值等于 chars 的串 T */
Status StrAssign(SString T, const char *chars) {
    int i, len = (int)strlen(chars);
    if (len > MAXSTRLEN) return ERROR;

    T[0] = len;
    for (i = 1; i <= len; i++) {
        T[i] = chars[i - 1];
    }
    return OK;
}

/*==================== 2. StrCopy ====================*/
/* 由串 S 复制得串 T */
Status StrCopy(SString T, SString S) {
    int i;
    for (i = 0; i <= S[0]; i++) {
        T[i] = S[i];
    }
    return OK;
}

/*==================== 3. StrEmpty ====================*/
/* 若 S 为空串，则返回 TRUE，否则返回 FALSE */
Bool StrEmpty(SString S) {
    return S[0] == 0 ? TRUE : FALSE;
}

/*==================== 4. StrCompare ====================*/
/* 若 S > T 返回值 >0；若 S=T 返回 0；若 S<T 返回值 <0 */
int StrCompare(SString S, SString T) {
    int i;
    for (i = 1; i <= S[0] && i <= T[0]; i++) {
        if (S[i] != T[i]) {
            return S[i] - T[i];
        }
    }
    return S[0] - T[0];
}

/*==================== 5. StrLength ====================*/
/* 返回串长 */
int StrLength(SString S) {
    return S[0];
}

/*==================== 6. ClearString ====================*/
/* 将 S 清为空串 */
Status ClearString(SString S) {
    S[0] = 0;
    return OK;
}

/*==================== 7. Concat ====================*/
/* 用 T 返回由 S1 和 S2 联接而成的新串 */
Status Concat(SString T, SString S1, SString S2) {
    int i;

    if (S1[0] + S2[0] <= MAXSTRLEN) {
        /* 未截断 */
        T[0] = S1[0] + S2[0];
        for (i = 1; i <= S1[0]; i++) {
            T[i] = S1[i];
        }
        for (i = 1; i <= S2[0]; i++) {
            T[S1[0] + i] = S2[i];
        }
        return OK;
    } else if (S1[0] < MAXSTRLEN) {
        /* 截断 S2 */
        T[0] = MAXSTRLEN;
        for (i = 1; i <= S1[0]; i++) {
            T[i] = S1[i];
        }
        for (i = 1; i <= MAXSTRLEN - S1[0]; i++) {
            T[S1[0] + i] = S2[i];
        }
        return ERROR;
    } else {
        /* 只保留 S1 前 MAXSTRLEN 个字符 */
        T[0] = MAXSTRLEN;
        for (i = 1; i <= MAXSTRLEN; i++) {
            T[i] = S1[i];
        }
        return ERROR;
    }
}

/*==================== 8. SubString ====================*/
/* 用 Sub 返回串 S 的第 pos 个字符起长度为 len 的子串 */
Status SubString(SString Sub, SString S, int pos, int len) {
    int i;
    if (pos < 1 || pos > S[0] || len < 0 || len > S[0] - pos + 1) {
        return ERROR;
    }

    Sub[0] = len;
    for (i = 1; i <= len; i++) {
        Sub[i] = S[pos + i - 1];
    }
    return OK;
}

/*==================== 9. Index ====================*/
/* 返回 T 在 S 中第 pos 个字符之后第一次出现的位置，否则返回 0 */
int Index(SString S, SString T, int pos) {
    int i, j;

    if (pos < 1 || pos > S[0] || T[0] == 0) return 0;

    for (i = pos; i <= S[0] - T[0] + 1; i++) {
        j = 1;
        while (j <= T[0] && S[i + j - 1] == T[j]) {
            j++;
        }
        if (j > T[0]) {
            return i;
        }
    }
    return 0;
}

/*==================== 10. StrInsert ====================*/
/* 在串 S 的第 pos 个字符之前插入串 T */
Status StrInsert(SString S, int pos, SString T) {
    int i;
    if (pos < 1 || pos > S[0] + 1) return ERROR;
    if (S[0] + T[0] > MAXSTRLEN) return ERROR;

    /* 后移 */
    for (i = S[0]; i >= pos; i--) {
        S[i + T[0]] = S[i];
    }

    /* 插入 */
    for (i = 1; i <= T[0]; i++) {
        S[pos + i - 1] = T[i];
    }

    S[0] += T[0];
    return OK;
}

/*==================== 11. StrDelete ====================*/
/* 从串 S 中删除第 pos 个字符起长度为 len 的子串 */
Status StrDelete(SString S, int pos, int len) {
    int i;
    if (pos < 1 || pos > S[0] || len < 0 || pos + len - 1 > S[0]) {
        return ERROR;
    }

    for (i = pos + len; i <= S[0]; i++) {
        S[i - len] = S[i];
    }

    S[0] -= len;
    return OK;
}

/*==================== 12. Replace ====================*/
/* 用 V 替换主串 S 中所有与 T 相等的不重叠子串 */
Status Replace(SString S, SString T, SString V) {
    int pos = 1;
    int index;

    if (T[0] == 0) return ERROR;

    while ((index = Index(S, T, pos)) != 0) {
        if (StrDelete(S, index, T[0]) == ERROR) return ERROR;
        if (StrInsert(S, index, V) == ERROR) return ERROR;
        pos = index + V[0];   /* 保证不重叠 */
    }
    return OK;
}

/*==================== 13. DestroyString ====================*/
/* 定长顺序串无动态内存，本质就是置空 */
Status DestroyString(SString S) {
    S[0] = 0;
    return OK;
}

/*==================== 测试主函数 ====================*/
int main() {
    SString S, T, V, Sub, Copy, Cat;

    printf("===== 1. StrAssign =====\n");
    StrAssign(S, "hello");
    StrAssign(T, "world");
    PrintString(S);
    PrintString(T);

    printf("\n===== 2. StrCopy =====\n");
    StrCopy(Copy, S);
    PrintString(Copy);

    printf("\n===== 3. StrEmpty =====\n");
    printf("S is empty? %s\n", StrEmpty(S) ? "TRUE" : "FALSE");
    ClearString(Copy);
    printf("Copy is empty? %s\n", StrEmpty(Copy) ? "TRUE" : "FALSE");

    printf("\n===== 4. StrCompare =====\n");
    printf("Compare(S,T) = %d\n", StrCompare(S, T));

    printf("\n===== 5. StrLength =====\n");
    printf("Length of S = %d\n", StrLength(S));

    printf("\n===== 6. ClearString =====\n");
    StrCopy(Copy, S);
    ClearString(Copy);
    PrintString(Copy);

    printf("\n===== 7. Concat =====\n");
    Concat(Cat, S, T);
    PrintString(Cat);

    printf("\n===== 8. SubString =====\n");
    SubString(Sub, Cat, 2, 4);
    PrintString(Sub);

    printf("\n===== 9. Index =====\n");
    StrAssign(S, "ababcabcacbab");
    StrAssign(T, "abcac");
    PrintString(S);
    PrintString(T);
    printf("Index(S,T,1) = %d\n", Index(S, T, 1));

    printf("\n===== 10. StrInsert =====\n");
    StrAssign(S, "HelloWorld");
    StrAssign(T, " ");
    StrInsert(S, 6, T);
    PrintString(S);

    printf("\n===== 11. StrDelete =====\n");
    StrDelete(S, 6, 1);
    PrintString(S);

    printf("\n===== 12. Replace =====\n");
    StrAssign(S, "abc123abc456abc");
    StrAssign(T, "abc");
    StrAssign(V, "XYZ");
    Replace(S, T, V);
    PrintString(S);

    printf("\n===== 13. DestroyString =====\n");
    DestroyString(S);
    PrintString(S);

    return 0;
}
```

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class SString {
private:
    static const int MAXSTRLEN = 255;
    unsigned char ch[MAXSTRLEN + 1]; // ch[0] 存长度

public:
    SString() {
        ch[0] = 0;
    }

    /* 1. StrAssign */
    bool StrAssign(const char* chars) {
        int len = (int)strlen(chars);
        if (len > MAXSTRLEN) return false;

        ch[0] = len;
        for (int i = 1; i <= len; i++) {
            ch[i] = chars[i - 1];
        }
        return true;
    }

    /* 2. StrCopy */
    bool StrCopy(const SString& S) {
        for (int i = 0; i <= S.ch[0]; i++) {
            ch[i] = S.ch[i];
        }
        return true;
    }

    /* 3. StrEmpty */
    bool StrEmpty() const {
        return ch[0] == 0;
    }

    /* 4. StrCompare */
    int StrCompare(const SString& T) const {
        for (int i = 1; i <= ch[0] && i <= T.ch[0]; i++) {
            if (ch[i] != T.ch[i]) {
                return ch[i] - T.ch[i];
            }
        }
        return ch[0] - T.ch[0];
    }

    /* 5. StrLength */
    int StrLength() const {
        return ch[0];
    }

    /* 6. ClearString */
    bool ClearString() {
        ch[0] = 0;
        return true;
    }

    /* 7. Concat */
    bool Concat(const SString& S1, const SString& S2) {
        int i;
        if (S1.ch[0] + S2.ch[0] <= MAXSTRLEN) {
            ch[0] = S1.ch[0] + S2.ch[0];
            for (i = 1; i <= S1.ch[0]; i++) ch[i] = S1.ch[i];
            for (i = 1; i <= S2.ch[0]; i++) ch[S1.ch[0] + i] = S2.ch[i];
            return true;
        } else if (S1.ch[0] < MAXSTRLEN) {
            ch[0] = MAXSTRLEN;
            for (i = 1; i <= S1.ch[0]; i++) ch[i] = S1.ch[i];
            for (i = 1; i <= MAXSTRLEN - S1.ch[0]; i++) ch[S1.ch[0] + i] = S2.ch[i];
            return false;
        } else {
            ch[0] = MAXSTRLEN;
            for (i = 1; i <= MAXSTRLEN; i++) ch[i] = S1.ch[i];
            return false;
        }
    }

    /* 8. SubString */
    bool SubString(const SString& S, int pos, int len) {
        if (pos < 1 || pos > S.ch[0] || len < 0 || len > S.ch[0] - pos + 1) {
            return false;
        }

        ch[0] = len;
        for (int i = 1; i <= len; i++) {
            ch[i] = S.ch[pos + i - 1];
        }
        return true;
    }

    /* 9. Index */
    int Index(const SString& T, int pos) const {
        if (pos < 1 || pos > ch[0] || T.ch[0] == 0) return 0;

        for (int i = pos; i <= ch[0] - T.ch[0] + 1; i++) {
            int j = 1;
            while (j <= T.ch[0] && ch[i + j - 1] == T.ch[j]) {
                j++;
            }
            if (j > T.ch[0]) return i;
        }
        return 0;
    }

    /* 10. StrInsert */
    bool StrInsert(int pos, const SString& T) {
        if (pos < 1 || pos > ch[0] + 1) return false;
        if (ch[0] + T.ch[0] > MAXSTRLEN) return false;

        for (int i = ch[0]; i >= pos; i--) {
            ch[i + T.ch[0]] = ch[i];
        }
        for (int i = 1; i <= T.ch[0]; i++) {
            ch[pos + i - 1] = T.ch[i];
        }

        ch[0] += T.ch[0];
        return true;
    }

    /* 11. StrDelete */
    bool StrDelete(int pos, int len) {
        if (pos < 1 || pos > ch[0] || len < 0 || pos + len - 1 > ch[0]) {
            return false;
        }

        for (int i = pos + len; i <= ch[0]; i++) {
            ch[i - len] = ch[i];
        }
        ch[0] -= len;
        return true;
    }

    /* 12. Replace */
    bool Replace(const SString& T, const SString& V) {
        if (T.ch[0] == 0) return false;

        int pos = 1, index;
        while ((index = Index(T, pos)) != 0) {
            if (!StrDelete(index, T.ch[0])) return false;
            if (!StrInsert(index, V)) return false;
            pos = index + V.ch[0];
        }
        return true;
    }

    /* 13. DestroyString */
    bool DestroyString() {
        ch[0] = 0;
        return true;
    }

    /* 输出 */
    void Print() const {
        for (int i = 1; i <= ch[0]; i++) {
            cout << (char)ch[i];
        }
        cout << " (length=" << (int)ch[0] << ")" << endl;
    }
};

int main() {
    SString S, T, V, Sub, Copy, Cat;

    cout << "===== 1. StrAssign =====" << endl;
    S.StrAssign("hello");
    T.StrAssign("world");
    S.Print();
    T.Print();

    cout << "\n===== 2. StrCopy =====" << endl;
    Copy.StrCopy(S);
    Copy.Print();

    cout << "\n===== 3. StrEmpty =====" << endl;
    cout << "S is empty? " << (S.StrEmpty() ? "TRUE" : "FALSE") << endl;
    Copy.ClearString();
    cout << "Copy is empty? " << (Copy.StrEmpty() ? "TRUE" : "FALSE") << endl;

    cout << "\n===== 4. StrCompare =====" << endl;
    cout << "Compare(S,T) = " << S.StrCompare(T) << endl;

    cout << "\n===== 5. StrLength =====" << endl;
    cout << "Length of S = " << S.StrLength() << endl;

    cout << "\n===== 6. ClearString =====" << endl;
    Copy.StrCopy(S);
    Copy.ClearString();
    Copy.Print();

    cout << "\n===== 7. Concat =====" << endl;
    Cat.Concat(S, T);
    Cat.Print();

    cout << "\n===== 8. SubString =====" << endl;
    Sub.SubString(Cat, 2, 4);
    Sub.Print();

    cout << "\n===== 9. Index =====" << endl;
    S.StrAssign("ababcabcacbab");
    T.StrAssign("abcac");
    S.Print();
    T.Print();
    cout << "Index(S,T,1) = " << S.Index(T, 1) << endl;

    cout << "\n===== 10. StrInsert =====" << endl;
    S.StrAssign("HelloWorld");
    T.StrAssign(" ");
    S.StrInsert(6, T);
    S.Print();

    cout << "\n===== 11. StrDelete =====" << endl;
    S.StrDelete(6, 1);
    S.Print();

    cout << "\n===== 12. Replace =====" << endl;
    S.StrAssign("abc123abc456abc");
    T.StrAssign("abc");
    V.StrAssign("XYZ");
    S.Replace(T, V);
    S.Print();

    cout << "\n===== 13. DestroyString =====" << endl;
    S.DestroyString();
    S.Print();

    return 0;
}
```