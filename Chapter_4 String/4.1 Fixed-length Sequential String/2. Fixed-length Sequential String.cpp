#include <iostream>
#include <cstring>
#include <stdexcept>
using namespace std;

constexpr int MAXSTRLEN = 255;  // max string length

class SString  // Sequential String
{
private:
    unsigned char S[MAXSTRLEN + 1];  // S[0] 存长度, String

public:
    // 默认构造
    SString()
    {
        S[0] = 0;
    }
    // 有参构造 = StrAssign()
    SString(const char* chars)
    {
        int len = strlen(chars);
        if (len > MAXSTRLEN)
        {
            throw overflow_error("输入串串长超过规定最大串长!");  // 抛出错误后自动退出程序
        }
        S[0] = len;
        for (int i = 1; i <= len; ++i)
        {
            S[i] = chars[i - 1];
        }
    }
    // 拷贝构造(深拷贝) = StrCopy()
    SString(const SString& other)
    {
        for (int i = 0; i <= other.S[0]; ++i)
        {
            S[i] = other.S[i];
        }
    }
    // 重载 =运算符, 拷贝赋值 -- 支持链式操作
    SString& operator=(const SString& other)
    {
        if (this != &other)  // &other 的 &不能掉(比较的类型保证为SString指针类型)
        {
            for (int i = 0; i <= other.S[0]; ++i)
            {
                S[i] = other.S[i];
            }
        }
        return *this;  // 返回深拷贝后的对象
    }
    // 析构函数(逻辑销毁)
    ~SString()
    {
        S[0] = 0;
    }
    // 清空
    void clear()
    {
        S[0] = 0;
    }
    // 判空
    bool empty() const
    {
        return S[0] == 0;
    }
    // 求串长
    int length() const
    {
        return S[0];
    }
    // 下标访问(类SString不是char*，不支持下标访问，故需要重载)
    // 只读下标访问
    unsigned char operator[](int pos) const  // S[pos]为unsigned char类型
    {
        if (pos < 1 || pos > S[0])
        {
            throw out_of_range("Index out of range!");
        }
        return S[pos];
    }
    // 可写下标访问
    unsigned char& operator[](int pos)  // 不能加 const，可以通过引用 unsigned char& 修改 S[pos]
    {
        if (pos < 1 || pos > S[0])
        {
            throw out_of_range("Index out of range!");
        }
        return S[pos];
    }
    // 串比较(类比较要单独重载 ==, !=, >, <)
    // 串比较函数
    int compare(const SString& other) const
    {
        for (int i = 1; i <= S[0] && i <= other.S[0]; ++i)
        {
            if (S[i] != other.S[i])
            {
                return S[i] - other.S[i];
            }
        }
        return S[0] - other.S[0];
    }
    // 运算符重载
    bool operator==(const SString& other) const
    {
        return compare(other) == 0;
    }
    bool operator!=(const SString& other) const
    {
        return compare(other) != 0;
    }
    bool operator<(const SString& other) const
    {
        return compare(other) < 0;
    }
    bool operator>(const SString& other) const
    {
        return compare(other) > 0;
    }
    // 重载+运算符 -- 串联接(返回新串，并非原地操作(重载+=运算符), 自动处理截断)
    SString operator+(const SString& other) const
    {
        SString result;
        int totalLen = S[0] + other.S[0];

        if (totalLen <= MAXSTRLEN)  // 无截断
        {
            for (int i = 1; i <= S[0]; ++i)
            {
                result.S[i] = S[i];
            }
            for (int i = 1; i <= other.S[0]; ++i)
            {
                result.S[i + S[0]] = other.S[i];
            }
            result.S[0] = totalLen;
        }
        else  // 有截断
        {
            int i = 1;
            for (; i <= S[0]; ++i)
            {
                result.S[i] = S[i];
            }
            for (; i <= MAXSTRLEN; ++i)
            {
                result.S[i] = other.S[i - S[0]];
            }
            result.S[0] = MAXSTRLEN;
        }
        return result;
    }
    // 求子串
    SString substr(int pos, int len) const 
    {
        if (pos < 1 || pos > S[0] || len < 0 || len > S[0] - pos + 1)
        {
            throw invalid_argument("参数 pos 或 len 非法!");
        }
        SString sub;
        for (int i = 1; i <= len; ++i)
        {
            sub.S[i] = S[i + pos - 1];
        }
        sub.S[0] = len;
        return sub;
    }
    // 查找子串T首次出现的位置
    int find(const SString& T, int pos = 1) const // 默认 pos 从第一个元素开始匹配
    {
        if (pos < 1 || pos > S[0] || T.empty())
        {
            throw invalid_argument("参数 pos 非法或参数 T 无效!");
        }
        for (int i = pos; i <= S[0] - T.S[0] + 1; ++i)  // i <= S[0] - T.S[0] + 1 -- 保证 i 后面还有长度为 T.S[0] 的串
        {
            if (substr(i, T.S[0]) == T)
            {
                return i;
            }
        }
        return 0;  // 未在主串 S 中找到子串 T
    }
    // 插入
    void insert(int pos, const SString& T)
    {
        if (pos < 1 || pos > S[0] + 1)
        {
            throw invalid_argument("参数 pos 非法!");
        }
        if (S[0] + T.S[0] > MAXSTRLEN)
        {
            throw overflow_error("插入子串后上溢!");
        }
        // pos~S[0] 后移 T.S[0] 个位置
        for (int i = S[0]; i >= pos; --i)
        {
            S[i + T.S[0]]  = S[i];
        }
        // 插入 T
        for (int i = 1; i <= pos + T.S[0]; ++i)
        {
            S[i + pos - 1] = T.S[i];
        }
        S[0] += T.S[0];
    }
    // 删除
    void erase(int pos, int len)
    {
        if (pos < 1 || pos + len -1 > S[0] || len < 0)
        {
            throw invalid_argument("参数 pos 或 len 非法!");
        }
        // 前移
        for (int i = pos + len; i <= S[0]; ++i)
        {
            S[i - len] = S[i];
        }
        S[0] -= len;
    }
    // 替换
    void replace(const SString& T, const SString& V)
    {
        if (T.empty())
        {
            throw invalid_argument("参数 T 为空，为无效参数!");
        }
        int pos = 1;
        while (true)
        {
            int idx = find(T, pos);
            if (idx == 0)  // 没有找到子串 T
            {
                break;  // 替换完成，退出循环
            }
            // 利用插入和删除完成替换
            // 删除 T
            erase(idx, T.S[0]);  // 自动更新长度
            // 插入 V
            insert(idx, V); // 自动更新长度
            // 更新 pos 防止重叠
            pos = idx + V.S[0];
            if (S[0] > MAXSTRLEN)
            {
                throw overflow_error("替换中发生上溢!");
            }
        }
    }
    // 输出流重载 -- 便于调试
    friend ostream& operator<<(ostream& os, const SString& s)
    {
        for (int i = 1; i <= s.S[0]; ++i)
        {
            os << s.S[i];
        }
        return os << " (len = " << (int)s.S[0] << ")";
    }
};

int main() {
    // 1. 构造与输出
    SString s("hello"), t("world");
    cout << "======================================" << endl;
    cout << "s = " << s << ", t = " << t << "\n";
    cout << "======================================" << endl;
    
    // 2. 运算符重载
    cout << "s == t? :" << (s == t ? "yes" : "no") << "\n";
    cout << "s + t = " << (s + t) << "\n";
    cout << "======================================" << endl;
    
    // 3. 子串与查找
    SString cat = s + t;
    cout << "sub(2,4) of cat = " << cat.substr(2, 4) << "\n";
    cout << "======================================" << endl;
    
    SString text("ababcabcacbab"), pattern("abcac");
    cout << "Find '" << pattern << "' in '" << text << "': pos=" 
              << text.find(pattern) << "\n";
    cout << "======================================" << endl;
    
    // 4. 插入/删除/替换
    SString demo("HelloWorld");
    demo.insert(6, SString(" "));  // Hello + " " + World
    cout << "After insert space: " << demo << "\n";
    cout << "======================================" << endl;
    
    demo.erase(6, 1);  // 删除空格
    cout << "After erase: " << demo << "\n";
    cout << "======================================" << endl;
    
    SString src("abc123abc456"), target("abc"), repl("XYZ");
    src.replace(target, repl);
    cout << "Replace result: " << src << "\n";
    cout << "======================================" << endl;
    
    // 5. 异常处理示例
    try {
        char c = s[100];  // 越界访问
    } catch (const exception& e) {
        cerr << "Caught: " << e.what() << "\n";
    }
    cout << "======================================" << endl;
    
    system("pause");
    return 0;
}