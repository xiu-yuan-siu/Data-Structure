#include <iostream>
#include <array>  // 使用迭代器
#include <stdexcept>  // 处理标准异常类型 out of range 索引越界
#include <algorithm>  // max(接受两个值), max_element(begin, end)返回迭代器, min_element, is_soted(检查), sort(修改, 本代码未使用)
#include <functional> // greater
using namespace std;

#define TRUE        1   // 真/是
#define FALSE       0   // 假/否
#define OK          1   // 通过/成功
#define ERROR       0   // 错误/失败
#define INFEASIBLE  -1  // 不可行的
#define OVERFLOW    -2  // 堆栈上溢

// 事实上, 状态码对于类中成员函数的返回多此一举, 最好直接使用类中写的 函数重载 (C++风格, 实际不是, 而是实现相同功能的别名函数, 防止无参数时冲突)
typedef int Status;  // 状态码

// ElemType 可以进行自定义, 这里使用泛型编程思想 —— 模板
template <class ElemType>
class Triplet
{
// 属性私有
private:
    array<ElemType, 3> arr;
public:
    // 有参构造
    Triplet(ElemType v1, ElemType v2, ElemType v3)  // 相当于书中 InitTriplet() 函数
    {
        arr = {v1, v2, v3};  // 列表初始化
    }
    // 析构函数
    ~Triplet()  // 相当于书中 DestroyTriplet() 函数
    {
        // 类实例化的对象在之后自动释放，开辟在栈区，不需要手动释放
        cout << "-----------------------------------------------" << endl;
        cout << "Triplet 对象已销毁, 内存自动释放" << endl;  
    }
    // Get 获取第 i 元的值
    Status Get(int i, ElemType &e)  // 这里要用引用修改传过来的参数
    {
        if (i < 1 || i > 3)
        {
            return ERROR;
        }
        e = arr[i - 1];
        return OK;
    }
    // Get的函数重载(实际不是, 而是实现相同功能的别名函数, 防止无参数时冲突), C++ 习惯函数首字母小写, 用大写字母区分
    ElemType& getVal(int i)  // 注意返回值类型 ElemType& 0拷贝开销, 直接修改原值
    {
        if (i < 1 || i > 3)
        {
            throw out_of_range("Index out of range!");
        }
        return arr[i - 1];
    }
    // Put 改变第 i 元的值为 e
    Status Put(int i, ElemType e)
    {
        if (i < 1 || i > 3)
        {
            return ERROR;
        }
        arr[i - 1] = e;
        return OK;
    }
    // Put 函数重载(实际不是, 而是实现相同功能的别名函数, 防止无参数时冲突)
    void setVal(int i, ElemType e)
    {
        if (i < 1 || i > 3)
        {
            throw out_of_range("Index out of range!");
        }
        arr[i - 1] = e;
    }
    // IsAscending 判断是否升序
    Status IsAscending()
    {
        // 升序返回 1, 否则返回 0
        return (arr[0] <= arr[1]) && (arr[1] <= arr[2]);
    }
    // IsAscending 函数重载(实际不是, 而是实现相同功能的别名函数, 防止无参数时冲突)
    bool checkAscending()
    {
        return is_sorted(arr.begin(), arr.end());  // 迭代器 + 算法库, 默认升序
    }
    // IsDescending 判断是否降序
    Status IsDescending()
    {
        // 降序序返回 1, 否则返回 0
        return (arr[0] >= arr[1]) && (arr[1] >= arr[2]);
    }
    // IsDescending 函数重载(实际不是, 而是实现相同功能的别名函数, 防止无参数时冲突)
    bool checkDescending()
    {
        return is_sorted(arr.begin(), arr.end(), greater<ElemType>());  // 迭代器 + 算法库, greater判断是否降序
    }
    // Max 获取最大元素的值
    Status Max(ElemType &e)
    {
        // 嵌套三元运算符 —— 先比较两个, 胜者再比第三个
        e = (arr[0] >= arr[1]) ? (arr[0] >= arr[2] ? arr[0] : arr[2]) : (arr[1] >= arr[2] ? arr[1] : arr[2]);
        return 0;
    }
    // Max 函数重载
    const ElemType& findMax()  // 返回 const ElemType& 防止引用被修改
    {
        return *max_element(arr.begin(), arr.end());
    }
    // Min 获取最大元素的值
    Status Min(ElemType &e)
    {
        // 嵌套三元运算符 —— 先比较两个, 胜者再比第三个
        e = (arr[0] <= arr[1]) ? (arr[0] <= arr[2] ? arr[0] : arr[2]) : (arr[1] <= arr[2] ? arr[1] : arr[2]);
        return 0;
    }
    // Min 函数重载
    const ElemType& findMin()
    {
        return *min_element(arr.begin(), arr.end());
    }
};

// ==========================================
// 测试案例 1: 测试 Status 风格 (兼容教材)
// ==========================================
void testStatusStyle() {
    cout << "=== [测试 1] Status 风格 (兼容教材) ===" << endl;
    cout << "-----------------------------------------------" << endl;
    Triplet<int> t(10, 20, 15);
    Status result;
    int val;

    // 1. 测试 Get
    result = t.Get(2, val);
    cout << "Get(2): " << (result == OK ? "Success" : "Failed") 
         << ", Value: " << val << endl;

    // 2. 测试越界 Get
    result = t.Get(5, val);
    cout << "Get(5): " << (result == ERROR ? "Correctly Caught Error" : "Unexpected OK") << endl;

    // 3. 测试 Put
    t.Put(3, 25); // 修改为 10, 20, 25 (升序)
    cout << "Put(3, 25) executed." << endl;

    // 4. 测试 IsAscending
    result = t.IsAscending();
    cout << "IsAscending: " << (result == OK ? "Yes" : "No") << endl;

    // 5. 测试 Max/Min
    int maxVal, minVal;
    t.Max(maxVal);
    t.Min(minVal);
    cout << "Max: " << maxVal << ", Min: " << minVal << endl;
}

// ==========================================
// 测试案例 2: 测试 C++ 重载/新风格 (现代 C++)
// ==========================================
void testCppStyle() {
    cout << "=== [测试 2] C++ 现代风格 (异常 + 直接返回值) ===" << endl;
    cout << "-----------------------------------------------" << endl;
    Triplet<double> t(3.5, 2.1, 4.9);

    try 
    {
        // 1. 测试 getVal (直接返回值)
        cout << "getVal(1): " << t.getVal(1) << endl;

        // 2. 测试 setVal
        t.setVal(2, 5.5); // 变为 3.5, 5.5, 4.9
        cout << "setVal(2, 5.5) executed." << endl;

        // 3. 测试 checkAscending (返回 bool)
        cout << "checkAscending: " << (t.checkAscending() ? "True" : "False") << endl;

        // 4. 测试 checkDescending
        cout << "checkDescending: " << (t.checkDescending() ? "True" : "False") << endl;

        // 5. 测试 findMax/findMin
        cout << "findMax: " << t.findMax() << endl;
        cout << "findMin: " << t.findMin() << endl;

        // 6. 测试异常捕获
        cout << "Attempting to access index 10..." << endl;
        cout << t.getVal(10) << endl; // 这行不会执行，会跳转 catch
    } 
    catch (const out_of_range& e) 
    {
        cout << ">>> 捕获到异常: " << e.what() << endl;
    }
}

int main() {
    testStatusStyle();
    cout << endl;
    testCppStyle();
    cout << endl;
    cout << "所有测试完成。程序结束，对象自动析构。" << endl;
    cout << endl;
    // system("pause");
    return 0;
}