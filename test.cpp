#include <iostream>
#include <vector>

using namespace std;

class Polynomial {
private:
    struct term {
        float coef;
        int expn;
    };
    struct LNode {
        term data;
        LNode* next;
    };
    LNode* head;

    // 内部辅助：在特定位置后插入新项（深拷贝或新造项用）
    void insertAfter(LNode* pre, float c, int e) {
        if (c == 0) return;
        LNode* s = new LNode{{c, e}, pre->next};
        pre->next = s;
    }

public:
    // 默认构造
    Polynomial() {
        head = new LNode{{0, 0}, nullptr};
    }

    // 辅助构造：通过数组快速创建 (对应之前的 CreatePolynByArray)
    Polynomial(float coefs[], int expns[], int n) : Polynomial() {
        for (int i = 0; i < n; ++i) {
            insertSorted(coefs[i], expns[i]);
        }
    }

    // 拷贝构造函数（实现深拷贝，防止多个对象指向同一链表）
    Polynomial(const Polynomial& other) : Polynomial() {
        LNode* dest = head;
        for (LNode* src = other.head->next; src; src = src->next) {
            insertAfter(dest, src->data.coef, src->data.expn);
            dest = dest->next;
        }
    }

    // 析构函数
    ~Polynomial() {
        clear();
        delete head;
    }

    void clear() {
        LNode* p = head->next;
        while (p) {
            LNode* temp = p;
            p = p->next;
            delete temp;
        }
        head->next = nullptr;
    }

    // 保持指数递增的插入逻辑
    void insertSorted(float c, int e) {
        if (c == 0) return;
        LNode* pre = head;
        while (pre->next && pre->next->data.expn < e) {
            pre = pre->next;
        }
        if (pre->next && pre->next->data.expn == e) {
            pre->next->data.coef += c;
            if (pre->next->data.coef == 0) {
                LNode* temp = pre->next;
                pre->next = temp->next;
                delete temp;
            }
        } else {
            insertAfter(pre, c, e);
        }
    }

    void create(int m) {
        clear();
        for (int i = 1; i <= m; ++i) {
            float c; int e;
            cout << "请输入第 " << i << " 项的系数和指数: ";
            cin >> c >> e;
            insertSorted(c, e);
        }
    }

    // --- 运算符重载 ---

    // 赋值运算符 (Deep Copy)
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            clear();
            LNode* dest = head;
            for (LNode* src = other.head->next; src; src = src->next) {
                insertAfter(dest, src->data.coef, src->data.expn);
                dest = dest->next;
            }
        }
        return *this;
    }

    // 加法赋值 (p1 += p2)
    Polynomial& operator+=(const Polynomial& other) {
        LNode* ha = head;
        LNode* qa = ha->next;
        LNode* qb = other.head->next;

        while (qa && qb) {
            if (qa->data.expn < qb->data.expn) {
                ha = qa;
                qa = qa->next;
            } else if (qa->data.expn == qb->data.expn) {
                qa->data.coef += qb->data.coef;
                if (qa->data.coef == 0) {
                    ha->next = qa->next;
                    delete qa;
                    qa = ha->next;
                } else {
                    ha = qa;
                    qa = qa->next;
                }
                qb = qb->next;
            } else {
                insertAfter(ha, qb->data.coef, qb->data.expn);
                ha = ha->next;
                qb = qb->next;
            }
        }
        while (qb) {
            insertAfter(ha, qb->data.coef, qb->data.expn);
            ha = ha->next;
            qb = qb->next;
        }
        return *this;
    }

    // 加法 (p1 + p2)
    Polynomial operator+(const Polynomial& other) const {
        Polynomial res = *this; // 利用拷贝构造
        res += other;
        return res;
    }

    // 减法 (p1 - p2)
    Polynomial operator-(const Polynomial& other) const {
        Polynomial res = *this;
        Polynomial temp = other;
        LNode* p = temp.head->next;
        while (p) { p->data.coef *= -1; p = p->next; }
        res += temp;
        return res;
    }

    // 乘法 (p1 * p2)
    Polynomial operator*(const Polynomial& other) const {
        Polynomial res;
        for (LNode* qa = head->next; qa; qa = qa->next) {
            Polynomial line; // 存放单项乘以多项式的临时结果
            LNode* curr = line.head;
            for (LNode* qb = other.head->next; qb; qb = qb->next) {
                line.insertAfter(curr, qa->data.coef * qb->data.coef, qa->data.expn + qb->data.expn);
                curr = curr->next;
            }
            res += line;
        }
        return res;
    }

    // 友元函数：重载输出流，支持 cout << p1;
    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        LNode* q = p.head->next;
        if (!q) { os << "0"; return os; }
        bool first = true;
        while (q) {
            float c = q->data.coef;
            int e = q->data.expn;
            if (!first && c > 0) os << " + ";
            if (c < 0) os << (first ? "-" : " - ");
            
            float abs_c = (c > 0) ? c : -c;
            if (abs_c != 1 || e == 0) os << abs_c;
            if (e != 0) {
                os << "x";
                if (e != 1) os << "^" << e;
            }
            q = q->next;
            first = false;
        }
        return os;
    }
};