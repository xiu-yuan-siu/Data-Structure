1. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错

2. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错（写一个成员函数打印一个main函数测试）

3. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

根据《数据结构（C语言版）》第3.2节的内容，栈的应用案例主要包括：**数制转换**、**括号匹配的检验**、**行编辑程序**、**迷宫求解**以及**表达式求值**。

由于代码篇幅较长，我为你实现前三个最典型的案例。代码将基于你在上一次对话中要求的“书中顺序栈实现（`SqStack`）”进行编写。

### 文件名建议
1.  **数制转换**：`NumberConversion.c` / `NumberConversion.cpp`
2.  **括号匹配**：`BracketMatching.c` / `BracketMatching.cpp`
3.  **行编辑程序**：`LineEditor.c` / `LineEditor.cpp`
4.  **迷宫求解**：`MazeSolver.c` / `MazeSolver.cpp`
5.  **表达式求值**：`ExpressionEval.c` / `ExpressionEval.cpp`

---

### 1. 数制转换 (Algorithm 3.1)
**原理**：利用栈“后进先出”的特性，将余数依次入栈，再弹出即可得到从高位到低位的进制转换结果。

#### **C++ 实现 (使用引用)**
```cpp
// 文件名: NumberConversion.cpp
void conversion(int N, int d) {
    SqStack S;
    InitStack(S); // 初始化栈
    while (N) {
        Push(S, N % d); // 余数入栈
        N = N / d;
    }
    while (!StackEmpty(S)) {
        ElemType e;
        Pop(S, e);      // 弹出余数并打印
        printf("%d", e);
    }
    DestroyStack(S);
}
```

#### **C 语言实现 (使用指针)**
```c
// 文件名: NumberConversion.c
void conversion_C(int N, int d) {
    SqStack S;
    InitStack_C(&S); 
    while (N) {
        Push_C(&S, N % d);
        N = N / d;
    }
    while (S.top != S.base) { // 判断栈不为空
        ElemType e;
        Pop_C(&S, &e);
        printf("%d", e);
    }
    DestroyStack_C(&S);
}
```

---

### 2. 括号匹配的检验
**原理**：遇到左括号入栈，遇到右括号则检查栈顶是否匹配并出栈。

#### **C++ 实现**
```cpp
// 文件名: BracketMatching.cpp
Status CheckBrackets(char* exp) {
    SqStack S;
    InitStack(S);
    for (int i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == '(' || exp[i] == '[') Push(S, exp[i]);
        else if (exp[i] == ')' || exp[i] == ']') {
            if (StackEmpty(S)) return ERROR;
            ElemType topChar;
            GetTop(S, topChar);
            if ((exp[i] == ')' && topChar == '(') || (exp[i] == ']' && topChar == '[')) 
                Pop(S, topChar);
            else return ERROR;
        }
    }
    return StackEmpty(S) ? OK : ERROR; // 最终栈空则匹配成功
}
```

#### **C 语言实现**
```c
// 文件名: BracketMatching.c
Status CheckBrackets_C(char* exp) {
    SqStack S;
    InitStack_C(&S);
    for (int i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == '(' || exp[i] == '[') Push_C(&S, exp[i]);
        else if (exp[i] == ')' || exp[i] == ']') {
            if (S.top == S.base) return ERROR;
            ElemType topChar;
            Pop_C(&S, &topChar);
            if ((exp[i] == ')' && topChar != '(') || (exp[i] == ']' && topChar != '['))
                return ERROR;
        }
    }
    return (S.top == S.base) ? OK : ERROR;
}
```

---

### 3. 行编辑程序 (Algorithm 3.2)
**原理**：使用栈作为输入缓冲区。输入 `#` 时弹出栈顶字符（退格），输入 `@` 时清空整个栈（重置行）。

#### **C++ 实现**
```cpp
// 文件名: LineEditor.cpp
void LineEdit() {
    SqStack S;
    InitStack(S);
    char ch = getchar();
    while (ch != EOF) {
        while (ch != EOF && ch != '\n') {
            switch (ch) {
                case '#': Pop(S, ch); break;     // 退格
                case '@': ClearStack(S); break;  // 清空整行
                default:  Push(S, ch); break;    // 有效字符入栈
            }
            ch = getchar();
        }
        // 此处可增加将栈内字符传送至数据区的代码
        ClearStack(S); 
        if (ch != EOF) ch = getchar();
    }
    DestroyStack(S);
}
```

#### **C 语言实现**
```c
// 文件名: LineEditor.c
void LineEdit_C() {
    SqStack S;
    InitStack_C(&S);
    char ch = getchar();
    while (ch != EOF) {
        while (ch != EOF && ch != '\n') {
            if (ch == '#') {
                ElemType e;
                Pop_C(&S, &e);
            } else if (ch == '@') {
                ClearStack_C(&S);
            } else {
                Push_C(&S, ch);
            }
            ch = getchar();
        }
        ClearStack_C(&S);
        if (ch != EOF) ch = getchar();
    }
    DestroyStack_C(&S);
}
```

### 提示：
*   **迷宫求解**（Algorithm 3.3）和**表达式求值**（Algorithm 3.4）的逻辑更为复杂。迷宫求解需要额外定义坐标结构体 `PosType`；表达式求值需要同时操作两个栈：运算符栈 `OPTR` 和操作数栈 `OPND`。
*   在编写 C 语言版本时，请确保你已经包含了我在之前对话中为你提供的 `SqStack` 结构体定义及基础函数。