#include <iostream>  // 用于输入输出
#include <map>       // 用于存储图的邻接表
#include <queue>     // 用于BFS遍历
#include <set>       // 用于记录已访问节点
#include <vector>    // 用于存储相邻节点列表

// 寻找访问[l,r]范围内所有整数所需的最小操作次数
// 可用操作: x/2(如果x为偶数), x-1, x+1, x*2
static int solve(int l, int r) {
    // 创建图的邻接表，用map存储每个节点的相邻节点列表
    std::map<int, std::vector<int>> adj;

    // 构建无向图：对范围内每个数字，添加所有可能的边
    for (int i = l; i <= r; i++) {
        // 如果i是偶数，添加 i <-> i/2 的边(前提是i/2在[l,r]范围内)
        if (i % 2 == 0 && i / 2 >= l && i / 2 <= r) {
            adj[i].push_back(i / 2);
            adj[i / 2].push_back(i);
        }

        // 添加 i <-> i-1 的边(前提是i-1在[l,r]范围内)
        if (i - 1 >= l && i - 1 <= r) {
            adj[i].push_back(i - 1);
            adj[i - 1].push_back(i);
        }

        // 添加 i <-> i+1 的边(前提是i+1在[l,r]范围内)
        if (i + 1 >= l && i + 1 <= r) {
            adj[i].push_back(i + 1);
            adj[i + 1].push_back(i);
        }

        // 添加 i <-> i*2 的边(前提是i*2在[l,r]范围内)
        if (i * 2 >= l && i * 2 <= r) {
            adj[i].push_back(i * 2);
            adj[i * 2].push_back(i);
        }
    }

    int best_ops = INT_MAX;

    // 尝试每个可能的起点
    for (int start = l; start <= r; start++) {
        // 运行BFS以找到最小生成树
        std::set<int> visited;
        std::queue<int> q;

        visited.insert(start);
        q.push(start);

        int ops = 0;  // 操作计数器

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int neighbor : adj[node]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                    ops++;  // 计数一次操作
                }
            }
        }

        // 检查是否访问了所有节点
        if (visited.size() == r - l + 1) {
            best_ops = std::min(best_ops, ops);
        }
    }

    return best_ops == INT_MAX ? -1 : best_ops;
}

int main() {
    int t = 0;
    std::cin >> t;

    while ((t--) != 0) {
        int l = 0;
        int r = 0;
        std::cin >> l >> r;
        std::cout << solve(l, r) << '\n';
    }

    return 0;
}