#include <iostream>
#include <string>
#include <stack>
using namespace std;

// 算法优先级处理
char Precede(char t1, char t2)
{
    const char table[7][7] = {
        {'>','>','<','<','<','>','>'}, 
        {'>','>','<','<','<','>','>'},
        {'>','>','>','>','<','>','>'}, 
        {'>','>','>','>','<','>','>'},
        {'<','<','<','<','<','=',' '}, 
        {'>','>','>','>',' ','>','>'},
        {'<','<','<','<','<',' ','='}
    };
    string op = "+-*/()#";

    int i = op.find(t1);
    int j = op.find(t2);

    return table[i][j];
}

// 判断是否为运算符
bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '#';
}

// 执行运算
int operate(int a, char op, int b)
{
    switch (op)
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

// 算符优先算法
int evaluateExpression()
{
    stack<char> OPTR;  // 运算符栈
    stack<int> OPND;   // 操作数栈

    OPTR.push('#');

    char c = cin.get();

    while (c != '#' || OPTR.top() != '#')
    {
        if (!isOperator(c))  // 不是运算符是操作数, 入栈OPND
        {
            OPND.push(c - '0');  // 重点：c 为字符, 加入操作数栈(int)要把字符转成数字(减去 字符 '0')
            c = cin.get();
        }
        else  // 是运算符
        {
            switch (Precede(OPTR.top(), c))
            {
                case '<':  // 优先级: 栈顶元素 < c, 将 运算符c 入栈 OPTR
                    OPTR.push(c);  // 把字符加入运算符栈不用转换
                    c = cin.get();
                    break;
                case '=':  // 优先级: 栈顶元素 = c(就是 正反括号 '(' 和 ')'), OPTR出栈栈顶元素 -- 脱去正括号 '('
                    OPTR.pop();
                    c = cin.get();
                    break;
                case '>':  // 优先级: 栈顶元素 > c, 脱去一个运算符和两个操作数进行运算，并将结果入栈OPND(操作数)
                    char theta = OPTR.top();  // 提取栈顶元素
                    OPTR.pop();  // 在栈中删去栈顶元素
                    int b = OPND.top();  // 操作数必须先提取 b
                    OPND.pop();
                    int a = OPND.top();
                    OPND.pop();
                    OPND.push(operate(a, theta, b));
                    break;
            }
        }
    }
    return OPND.top();  // 操作数栈顶元素即为最终的计算结果
}

int main()
{
    cout << "请输入表达式(如: 3*(7-2)#): ";
    cout << evaluateExpression() << endl;
    system("pause");
    return 0;
}