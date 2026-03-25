1. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错

2. 检查看看代码有没有错误，有的话告诉我该改哪个函数的哪个地方，改完后写一份测试案例测试所有函数功能和所有报错（写一个成员函数打印一个main函数测试）

3. 双向链表的实现，C和C++各写出来，函数包括初始化，销毁，清空，获取元素（传位置），返回位序，获取前驱，获取后继，插入，删除，合并，打印(用于测试)可适当补充或增减上述所说的函数，按照书里面的要求来, 并写上C和C++的测试案例，测试所有功能和边界错误的处理

### 算法复杂度分析

| 算法 | 时间复杂度 | 空间复杂度 | 核心数据结构 | 对应页码 |
| :--- | :--- | :--- | :--- | :--- |
| **EvaluateExpression** | $O(n)$ | $O(n)$ | 双栈（OPTR & OPND） | 第 63 页 [1] |

---

### 1. C 语言实现
本实现参考书中第 46-47 页的顺序栈定义 [2, 3] 及第 63 页的算法逻辑 [1]。

```c
#include <stdio.h>
#include <stdlib.h>

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

// 栈结构定义 [第46页]
typedef struct {
    int *base;
    int *top;
    int stacksize;
} SqStack;

void InitStack(SqStack *S) {
    S->base = (int *)malloc(STACK_INIT_SIZE * sizeof(int));
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
}

void Push(SqStack *S, int e) {
    if (S->top - S->base >= S->stacksize) {
        S->base = (int *)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(int));
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *(S->top++) = e;
}

void Pop(SqStack *S, int *e) {
    if (S->top != S->base) *e = *(--S->top);
}

int GetTop(SqStack S) {
    if (S.top != S.base) return *(S.top - 1);
    return 0;
}

// 算符优先级处理 [第63页 表3.1]
char Precede(char t1, char t2) {
    static const char table[4] = {
        {'>','>','<','<','<','>','>'}, {'>','>','<','<','<','>','>'},
        {'>','>','>','>','<','>','>'}, {'>','>','>','>','<','>','>'},
        {'<','<','<','<','<','=',' '}, {'>','>','>','>',' ','>','>'},
        {'<','<','<','<','<',' ','='}
    };
    char ops[] = "+-*/()#";
    int i, j;
    for(int k=0; k<7; k++) {
        if(ops[k] == t1) i = k;
        if(ops[k] == t2) j = k;
    }
    return table[i][j];
}

int Operate(int a, char theta, int b) {
    if(theta == '+') return a + b;
    if(theta == '-') return a - b;
    if(theta == '*') return a * b;
    return a / b;
}

int In(char c) {
    char ops[] = "+-*/()#";
    for(int i=0; i<7; i++) if(c == ops[i]) return 1;
    return 0;
}

// 表达式求值算法 [算法3.4, 第63页]
int EvaluateExpression() {
    SqStack OPTR, OPND;
    InitStack(&OPTR); Push(&OPTR, '#');
    InitStack(&OPND);
    char c = getchar();
    int a, b, theta, x;
    while (c != '#' || GetTop(OPTR) != '#') {
        if (!In(c)) { Push(&OPND, c - '0'); c = getchar(); }
        else {
            switch (Precede(GetTop(OPTR), c)) {
                case '<': Push(&OPTR, c); c = getchar(); break;
                case '=': Pop(&OPTR, &x); c = getchar(); break;
                case '>': 
                    Pop(&OPTR, &theta); Pop(&OPND, &b); Pop(&OPND, &a);
                    Push(&OPND, Operate(a, (char)theta, b)); break;
            }
        }
    }
    return GetTop(OPND);
}

int main() {
    printf("请输入表达式 (案例: 3*(7-2)#): ");
    printf("结果: %d\n", EvaluateExpression());
    return 0;
}
```

---

### 2. C++ 语言实现
采用标准 STL 容器，保持书本逻辑命名。

```cpp
#include <iostream>
#include <stack>
#include <string>

using namespace std;

// 算符优先级 [第63页]
char Precede(char theta1, char theta2) {
    string ops = "+-*/()#";
    char table[4] = {
        {'>','>','<','<','<','>','>'}, {'>','>','<','<','<','>','>'},
        {'>','>','>','>','<','>','>'}, {'>','>','>','>','<','>','>'},
        {'<','<','<','<','<','=',' '}, {'>','>','>','>',' ','>','>'},
        {'<','<','<','<','<',' ','='}
    };
    return table[ops.find(theta1)][ops.find(theta2)];
}

int Operate(int a, char theta, int b) {
    switch(theta) {
        case '+': return a + b; case '-': return a - b;
        case '*': return a * b; case '/': return a / b;
        default: return 0;
    }
}

// 算法 3.4 [第63页]
int EvaluateExpression() {
    stack<char> OPTR; OPTR.push('#');
    stack<int> OPND;
    char c, theta, x;
    int a, b;
    cin >> c;
    while (c != '#' || OPTR.top() != '#') {
        if (isdigit(c)) { 
            OPND.push(c - '0'); cin >> c; 
        } else {
            switch (Precede(OPTR.top(), c)) {
                case '<': OPTR.push(c); cin >> c; break;
                case '=': OPTR.pop(); cin >> c; break;
                case '>': 
                    theta = OPTR.top(); OPTR.pop();
                    b = OPND.top(); OPND.pop();
                    a = OPND.top(); OPND.pop();
                    OPND.push(Operate(a, theta, b)); break;
            }
        }
    }
    return OPND.top();
}

int main() {
    cout << "测试案例 (书中例3-1): 3*(7-2)#" << endl;
    cout << "计算结果: " << EvaluateExpression() << endl; // 应输出 15 [5]
    return 0;
}
```

---

### 3. 概念深度理解

**逻辑结构描述**：
该算法通过**双栈（操作数栈 OPND 和 运算符栈 OPTR）协同**工作 [1]。OPND 存储运算数，OPTR 存储暂未执行的运算符。

**核心逻辑比喻**：
就像**交响乐团的指挥**：操作数是乐手，运算符是乐谱指令。指挥遇到高优先级的强音（如 `*` 或 `(`）会要求立即准备，而遇到低优先级或乐句结束符（如 `+` 或 `#`）则下令前面的乐手先开始演奏（执行运算）。

**工程应用**：
1.  **编译器解析**：编程语言（如 C, Java）的中缀表达式求值底层原理。
2.  **科学计算器**：手持计算器或手机计算器软件。
3.  **Excel 公式引擎**：表格中复杂数学公式的逻辑计算。

**现代差异与优化**：
*   **STL 底层**：`std::stack` 是基于 `std::deque` 的容器适配器，不像书中 `SqStack` 那样保证内存绝对连续，且支持动态缩容 [2]。
*   **现代优化**：编译器通常先将中缀表达式转化为**逆波兰表达式（后缀表达式）**或**抽象语法树（AST）**，这样在运行时只需单栈即可完成计算，且便于进行常量折叠等代码优化。