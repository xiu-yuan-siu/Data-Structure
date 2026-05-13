#include <iostream>
#include <stdexcept>
using namespace std;

class Polynomial
{
private:
    struct term  // 项
    {
        float coef;
        int expn;
    };
    struct LNode
    {
        term data;
        LNode *next;
    };
    LNode *head;

    // 内部辅助函数：在特定位置后插入新项(深拷贝 和 有参构造用)
    void insertAfter(LNode *pre, float c, int e)
    {
        LNode *s = new LNode{{c, e}, pre->next};
        pre->next = s;
    }
public:
    // 默认构造 初始化
    Polynomial()
    {
        head = new LNode{{0, 0}, nullptr};  // 每个 {} 都是一个列表初始化
    }
    // 有参构造, 通过数组快速创建多项式, 辅助构造多项式，避免输入麻烦
    Polynomial(float coefs[], int expns[], int n) : Polynomial ()
    {
        for (int i = 0; i < n; ++i)
        {
            insertSorted(coefs[i], expns[i]);
        }
    }
    // 拷贝构造函数(深拷贝，防止多个对象指向同一片内存空间)
    Polynomial(const Polynomial &other) : Polynomial()
    {
        LNode *p = head;
        for (LNode *q = other.head->next; q != nullptr; q = q->next)
        {
            insertAfter(p, q->data.coef, q->data.expn);
            p = p->next;
        }
    }
    // 赋值运算符重载(深拷贝)
    Polynomial& operator=(const Polynomial &other)
    {
        if (this != &other)
        {
            clear();
            LNode *p = head;
            for (LNode *q = other.head->next; q != nullptr; q = q->next)
            {
                insertAfter(p, q->data.coef, q->data.expn);
                p = p->next;
            }
        }
        return *this;
    }
    // 析构函数 销毁
    ~Polynomial()
    {
        clear();
        delete head;
    }
    void clear()
    {
        LNode *p = head->next;
        while (p)
        {
            LNode *temp = p;
            p = p->next;
            delete temp;
        }
        head->next = nullptr;
    }
    // 指数递增插入
    void insertSorted(float c, int e)
    {
        if (c == 0)
        {
            return;
        }
        LNode *pre = head;  // 插入位置的前驱
        while (pre->next && pre->next->data.expn < e)  // 找前驱
        {
            pre = pre->next;
        }
        if (pre->next && pre->next->data.expn == e)  // 指数相同
        {
            pre->next->data.coef += c;
            if (pre->next->data.coef == 0)  // 指数和为 0，删除结点
            {
                LNode *temp = pre->next;
                pre->next = temp->next;
                delete temp;
            }
        }
        else  // 指数不同, 插入 pre 后面
        {
            insertAfter(pre, c, e);
        }
    }
    void creat(int m)   
    {
        clear();
        for (int i = 1; i <= m; ++i)
        {
            float c;
            int e;
            cout << "请依次输入第 " << i << " 项的系数和指数: ";
            cin >> c >> e;
            insertSorted(c, e);
        }
    }
    // += 运算符重载  C++ 习惯性能进行 p3 = p1 + p2 的运算，就能进行 p1 += p2 的运算(原地操作)
    Polynomial& operator+=(const Polynomial &other)  // 原地操作 返回 Polynomial&
    {
        LNode *ha = head;
        LNode *qa = ha->next;
        LNode *qb = other.head->next;
        while (qa && qb)
        {
            if (qa->data.expn < qb->data.expn)  // coef: qa < qb, 加入 qa 这里不需要 ++qb
            {
                ha = qa;
                qa = qa->next;
            }
            else if (qa->data.expn == qb->data.expn)  // coef: qa = qb
            {
                qa->data.coef += qb->data.coef;
                if (qa->data.coef == 0)  // 系数和为 0 删除结点
                {
                    ha->next = qa->next;
                    delete qa;
                    qa = ha->next;
                }
                else  // 系数之和不为 0，把和结点 qa 插入到 ha 后面
                {
                    ha = qa;
                    qa = qa->next;
                }
                qb = qb->next;
            }
            else  // coef: qa > qb , 把qb插入到 ha 后面
            {
                insertAfter(ha, qb->data.coef, qb->data.expn);
                ha = ha->next;
                qb = qb->next;
            }
        }
        // 将 qb 剩余段加入 ha
        while (qb)
        {
            insertAfter(ha, qb->data.coef, qb->data.expn);
            ha = ha->next;
            qb = qb->next;
        }
        return *this;
    }
    // +
    Polynomial operator+(const Polynomial &other) const 
    {
        Polynomial res = *this;  // 拷贝构造，实际为副本(深拷贝)
        res += other;
        return res;
    }
    // -
    Polynomial operator-(const Polynomial &other) const 
    {
        Polynomial res = *this;
        Polynomial temp = other;
        LNode *p = temp.head->next;
        while (p)
        {
            p->data.coef *= -1;
            p = p->next;
        }
        res += temp;
        return res;
    }
    // *
    Polynomial operator*(const Polynomial &other) const
    {
        Polynomial res;
        for (LNode *qa = head->next; qa != nullptr; qa = qa->next)
        {
            Polynomial temp;  // 临时多项式 存单项乘多项式的结果
            LNode *cur = temp.head;  // 单项乘单项结果 每次插入在 cur 后面
            for (LNode *qb = other.head->next; qb != nullptr; qb = qb->next)
            {
                temp.insertAfter(cur, qa->data.coef * qb->data.coef, qa->data.expn + qb->data.expn);
                cur = cur->next;
            }
            res += temp;
        }
        return res;
    }
    // 友元函数：重载输入流，支持 cout << Polynomial;
    friend ostream& operator<<(ostream &os, const Polynomial &p)  // 里面的 os 相当于 cout
    {
        LNode *q = p.head->next;
        if (!q)  // 多项式为空
        {
            os << "0";
        }
        bool first = true;  // 控制第一项前面不打印符号
        while (q)
        {
            if (!first && q->data.coef > 0)  // 不是第一项 且 系数>0
            {
                os << " + ";
            }
            if (q->data.coef < 0)  // 系数 < 0, 不打印 + ，优化系数的负号，第一项不优化
            {
                os << (first ? "-" : " - ");
            }
            float abs_c = (q->data.coef > 0) ? q->data.coef : -q->data.coef;
            if (abs_c != 1 || q->data.expn == 0)  // 针对 系数为不为 1或-1(系数为这两个就不打印他们的绝对值 1) 或 指数为 0 的情况 
            {
                os << abs_c;  // "-" 已经替换了
            }
            if (q->data.expn != 0)
            {
                os << "x";
                if (q->data.expn != 1)
                {
                    os << "^" << q->data.expn;
                }
            }
            q = q->next;
            first = false;
        }
        return os;
    }
};

int main() {
    // 测试 1: 通过数组创建 (CreatePolynByArray 功能)
    float c1[] = {3, 4, 7}; int e1[] = {1, 3, 5};
    float c2[] = {2, -4, 5}; int e2[] = {2, 3, 6};
    
    Polynomial p1(c1, e1, 3); // 3x^1 + 4x^3 + 7x^5
    Polynomial p2(c2, e2, 3); // 2x^2 - 4x^3 + 5x^6

    cout << "P1: " << p1 << endl;
    cout << "P2: " << p2 << endl;

    // 测试 2: 运算符加法 (4x^3 和 -4x^3 应该抵消)
    Polynomial pSum = p1 + p2;
    cout << "P1 + P2: " << pSum << " (4x^3被抵消)" << endl;

    // 测试 3: 运算符减法
    Polynomial pSub = p1 - p2;
    cout << "P1 - P2: " << pSub << endl;

    // 测试 4: 运算符乘法 (1+x)(1-x) = 1-x^2
    float c3[] = {1, 1}; int e3[] = {0, 1};
    float c4[] = {1, -1}; int e4[] = {0, 1};
    Polynomial p3(c3, e3, 2);
    Polynomial p4(c4, e4, 2);
    cout << "(" << p3 << ") * (" << p4 << ") = " << (p3 * p4) << endl;

    // 测试 5: 边界 - 结果为 0
    Polynomial pZero = p3 - p3;
    cout << "P3 - P3: " << pZero << endl;
    // system("pause");
    return 0;
}