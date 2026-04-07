1. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错

2. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

```c

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