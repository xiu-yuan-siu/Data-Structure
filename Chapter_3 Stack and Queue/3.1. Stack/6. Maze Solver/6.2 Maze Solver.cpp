#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct PosType
{
    int x, y;
    bool operator==(const PosType &other) const  // 重载 == 符号，便于书中 curPos == end(结构体间的比较) 的实现
    {
        return x == other.x && y == other.y;
    }
};

struct SElemType
{
    int ord;        // 序号
    PosType seat;   // 坐标
    int di;         // 下一布方向(0:东, 1:南, 2:西, 3:北)
};

class MazeSolver
{
private:
    vector<vector<int>> maze;  // 二维vector，原二维数组(支持下标访问)
    vector<PosType> path;   // 保存最终路径
    // 方向增量数组：东(y++)、南(x++)、西(y--)、北(x--)(注意对应)
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

public:
    // 列表初始化，如果类成员是 const（常量）或引用类型，必须用这种初始化列表，不能在花括号里赋值。
    MazeSolver(vector<vector<int>>m) : maze(m) {};

    // == Pass()
    bool isPassable(PosType &p)
    {
        return maze[p.x][p.y] == 0;
    }

    // == FootPrint() and MarkPrint()
    void mark(PosType &p, int val)  
    {
        maze[p.x][p.y] = val;  // val = 2 已访问, val = 3 死胡同
    }

    bool solve(PosType start, PosType end)
    {
        stack<SElemType> s;         // 直接使用容器 stack 不用实现栈
        PosType curPos = start;     // 当前坐标
        int  curOrd = 1;            // 当前序号(第几步)
        do {
            if (isPassable(curPos))  // == 0
            {
                // 等价于
                // SElemtype e = (curOrd, curPos, 1);
                // s.push(e);
                s.push({curOrd, curPos, 0});   // {} 列表初始化结构体(SElemType类型), 然后push进栈
                mark(curPos, 2);
                if (curPos == end)  // 用重载的结构体 ==运算符 直接比较
                {
                    savePath(s);
                    return true;
                }
                // 未到到终点，更新位置和序号(步数)
                // 利用 方向增量数组 进行坐标位置更新(列表初始化)
                curPos = {curPos.x + dx[0], curPos.y + dy[0]};  // 0:东
                ++curOrd;
            }
            else  // != 0
            {
                while (!s.empty())
                {
                    SElemType &top = s.top();
                    // 顺时针没有找到最后一个方向，将 curPos 更新到 curPos 前一个元素 &top 的另一个方向，然后直接退出循环
                    if (top.di < 3)
                    {
                        ++top.di;
                        curPos = {top.seat.x + dx[top.di], top.seat.y + dy[top.di]};
                        break;
                    }
                    // 顺时针四个方向都试过，将 curPos 前一个元素 &top 标注为死胡同，并进行出栈
                    else
                    {
                        mark(top.seat, 3);  
                        s.pop();
                    }
                }
            }
        } while (!s.empty());
        return false;
    }
    void printMaze() {
        for (const auto& row : maze) {
            for (int cell : row) {
                if (cell == 1) cout << " # ";
                else if (cell == 2) cout << " . ";
                else if (cell == 3) cout << " X ";
                else cout << "   ";
            }
            cout << "\n";
        }
    }
    void savePath(stack<SElemType> s)
    {
        path.clear();
        vector<PosType> temp;

        while (!s.empty())
        {
            temp.push_back(s.top().seat);
            s.pop();
        }

        for (int i = temp.size() - 1; i >= 0; --i)
        {
            path.push_back(temp[i]);
        }
    }
    void printPath()
    {
        cout << "\n确定的路径坐标序列：\n";
        for (int i = 0; i < path.size(); ++i)
        {
            cout << "(" << path[i].x << "," << path[i].y << ")";
            if (i != path.size() - 1)
                cout << " -> ";

            if ((i + 1) % 5 == 0)
                cout << "\n";
        }
        cout << " -> END\n";
    }
};

int main()
{
    vector<vector<int>> mazeData = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    MazeSolver solver(mazeData);

    if (solver.solve({1, 1}, {8, 8}))
    {
        cout << "成功找到路径！\n";
        solver.printPath();   // 打印坐标路径
        solver.printMaze();   // 打印迷宫
    }
    else
    {
        cout << "无解。\n";
    }
    system("pause");
    return 0;
}