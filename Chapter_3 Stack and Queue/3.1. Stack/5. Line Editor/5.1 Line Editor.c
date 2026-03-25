#include <stdio.h>
#include <stdlib.h>
#include "../../../Status.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef char SElemType;  // 注意这里 SElemType 应为 char 做括号匹配

typedef struct
{
    SElemType *top;
    SElemType *base;
    int stacksize;
}SqStack;

Status InitStack(SqStack *S)
{
    S->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S->base)
    {
        exit(OVERFLOW);
    }
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return OK;
}

Status DestroyStack(SqStack *S)
{
    if (S->base)
    {
        free(S->base);
        S->base = S->top = NULL;
        S->stacksize = 0;
    }
    return OK;
}

Status ClearStack(SqStack *S)
{
    S->top = S->base;
    return OK;
}

Status GetTop(SqStack S, SElemType *e)
{
    if (S.base == S.top)
    {
        return ERROR;
    }
    *e = *(S.top - 1);
    return OK;
}

Status Push(SqStack *S, SElemType e)
{
    // 扩容
    if (S->top - S->base >= S->stacksize)
    {
        S->base = (SElemType*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S->base)
        {
            exit(OVERFLOW);
        }
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *(S->top++) = e;
    return OK;
}

Status Pop(SqStack *S, SElemType *e)
{
    if (S->base == S->top)
    {
        return ERROR;
    }
    *e = *(--S->top);
    return OK;
}

Status StackEmpty(SqStack S)
{
    return S.base == S.top;
}

Status LineEdit()
{
    SqStack S;
    InitStack(&S);
    // int getchar(void): 1.返回值为int，因为需要标识EOF(通常为 -1); 2.遇到换行符 \n 也会读取; 3.属于缓冲输入——按下回车才会把数据送入程序
    char ch = getchar();  // 从终端接受一个字符 
    while (ch != EOF)  // EOF 为全文结束符
    {
        while (ch != EOF && ch != '\n')  // 内层循环体内，存在修改变量 ch 的操作——即执行了 ch = getchar();所以这里也要判断 ch != EOF
        {
            if (ch == '#')  // 栈非空时才可退栈(删除栈顶元素)
            {
                if (!StackEmpty(S))  // 若为空栈
                {
                    SElemType temp;
                    Pop(&S, &temp);
                }
            }
            else if (ch == '@')  // 删除整行，即重置为空栈
            {
                ClearStack(&S);
            }
            else  // 有效字符进栈
            {
                // 考虑栈满 -- 扩容(在Push中实现了)
                Push(&S, ch);
            }
            ch = getchar();  // 接收下一个字符
        }
        // 将从栈底到栈顶的栈内字符传送至调用过程的数据区，即向屏幕输出一个字符
        // 顺序栈本质为数组，输出时用指针遍历数组输出(不符合栈后进先出的特性)
        for (SElemType *p = S.base; p < S.top; ++p)
        {
            putchar(*p);
        }
        putchar('\n');
        ClearStack(&S);
        if (ch != EOF)
        {
            ch = getchar();
        }
    }
    DestroyStack(&S);
    return OK;
}

/*
* 测试点	    键盘输入内容	     预期输出（按回车后）	      说明
* 基础输入	    hello	            hello	                正常字符入栈输出
* 退格功能	    whli##ile	        while	                # 删除了前面的 i
* 连续退格	    computer###	        compu	                连续三个 # 删除了 ter
* 空栈退格	    a##b	            b	                    栈空时输入 # 应被忽略，不崩溃
* 全行删除	    wrong line@right	right	                @ 之前的全部被清空
* 混合操作	    abc#d@123#4	        124	                    结合了退格和清行

* 结束程序	Ctrl+Z (Windows)	(程序退出)	             测试 EOF 处理
*/

int main()
{
    printf("--- 行编辑程序已启动 ---\n");
    printf("提示：使用 # 退格，使用 @ 清行，Ctrl+Z(Win)结束\n");
    
    LineEdit();
    
    printf("\n--- 程序运行结束 ---\n");
    return 0;
}