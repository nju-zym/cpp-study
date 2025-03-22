#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

// 并查集类，用于处理连通性问题
class UnionFind {
   private:
    vector<int> parent, rank;

   public:
    // 初始化并查集
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // 查找元素的根节点，并进行路径压缩
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // 合并两个集合
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) {
            return false;
        }

        // 按秩合并
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            if (rank[rootX] == rank[rootY]) {
                rank[rootX]++;
            }
        }
        return true;
    }
}

// 计算 (a^b) % mod，适用于大数幂
ll pow_mod(ll a, ll b, ll mod) {
    if (mod == 1) {
        return 0;
    }
    ll res  = 1;
    a      %= mod;
    while (b > 0) {
        if (b & 1) {
            res = (res * a) % mod;
        }
        a   = (a * a) % mod;
        b >>= 1;
    }
    return res;
}

// 边结构体，包含两个顶点和权重
struct Edge {
    int u, v;
    ll weight;

    Edge(int _u, int _v, ll _w) : u(_u), v(_v), weight(_w) {}

    // 重载小于运算符，用于边的排序
    bool operator<(Edge const& other) const {
        return weight < other.weight;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int l, r, k;
    cin >> l >> r >> k;

    vector<Edge> edges;

    // 构建所有可能的边
    for (int i = l; i <= r; i++) {
        // 操作1: x/2 (如果是偶数)
        if (i % 2 == 0 && i / 2 >= l && i / 2 <= r) {
            ll strength = (pow_mod(i, i / 2, k) + pow_mod(i / 2, i, k)) % k;
            edges.emplace_back(i - l, i / 2 - l, strength);
        }

        // 操作2: x-1
        if (i - 1 >= l) {
            ll strength = (pow_mod(i, i - 1, k) + pow_mod(i - 1, i, k)) % k;
            edges.emplace_back(i - l, (i - 1) - l, strength);
        }

        // 操作3: x+1
        if (i + 1 <= r) {
            ll strength = (pow_mod(i, i + 1, k) + pow_mod(i + 1, i, k)) % k;
            edges.emplace_back(i - l, (i + 1) - l, strength);
        }

        // 操作4: x*2
        if (i * 2 <= r) {
            ll strength = (pow_mod(i, i * 2, k) + pow_mod(i * 2, i, k)) % k;
            edges.emplace_back(i - l, (i * 2) - l, strength);
        }
    }

    // 按权重升序排序边
    sort(edges.begin(), edges.end());

    // 查找最小瓶颈生成树
    UnionFind uf(r - l + 1);
    int components      = r - l + 1;
    ll min_max_strength = 0;

    for (Edge const& edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v);
            components--;
            min_max_strength = max(min_max_strength, edge.weight);

            // 如果我们已经有了一棵生成树，就完成了
            if (components == 1) {
                break;
            }
        }
    }

    // 检查是否有有效的解决方案（连通图）
    if (components == 1) {
        cout << min_max_strength << endl;
    } else {
        cout << "-1" << endl;
    }

    return 0;
}