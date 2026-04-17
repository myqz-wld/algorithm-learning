#include <iostream>
#include <vector>
#include <functional>

/*
最近公共祖先 LCA (Lowest Common Ancestor)

=== 朴素法 ===

将两个节点提升到相同深度，然后同步向上走直到相遇。
预处理：O(N)  查询：O(N)

=== 倍增法 (Binary Lifting) ===

预计算 up[i][j] = 节点 i 的第 2^j 个祖先。
递推：up[i][j] = up[up[i][j-1]][j-1]
  即：走 2^j 步 = 先走 2^(j-1) 步，再走 2^(j-1) 步

查询 LCA(u, v)：
1. 将深的节点提升到和浅的相同深度（二进制拆分跳跃）
2. 若此时 u == v，返回（v 是 u 的祖先）
3. 否则 u, v 同步倍增跳跃，找到 LCA 的直接子节点，再上跳一步

预处理：O(NlogN)  查询：O(logN)

示例树：
       0
      / \
     1   2
    / \   \
   3   4   5
*/

// 朴素法：逐步上跳
int lca_naive(const std::vector<int>& parents, int u, int v) {
    auto get_depth = [&](int x) {
        int d = 0;
        while (parents[x] != -1) { x = parents[x]; d++; }
        return d;
    };
    int du = get_depth(u), dv = get_depth(v);
    while (du > dv) { u = parents[u]; du--; }
    while (dv > du) { v = parents[v]; dv--; }
    while (u != v) { u = parents[u]; v = parents[v]; }
    return u;
}

// 倍增法
class LCA {
    int n, LOG;
    std::vector<int> depth;
    std::vector<std::vector<int>> up;

public:
    LCA(const std::vector<int>& parents) : n(parents.size()) {
        LOG = 1;
        while ((1 << LOG) < n) LOG++;
        LOG++;

        std::vector<std::vector<int>> children(n);
        int root = -1;
        for (int i = 0; i < n; i++) {
            if (parents[i] == -1) root = i;
            else children[parents[i]].push_back(i);
        }

        depth.assign(n, 0);
        up.assign(n, std::vector<int>(LOG));
        for (int i = 0; i < n; i++)
            up[i][0] = (parents[i] == -1) ? i : parents[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 0; i < n; i++)
                up[i][j] = up[up[i][j - 1]][j - 1];

        std::function<void(int)> dfs = [&](int u) {
            for (int v : children[u]) {
                depth[v] = depth[u] + 1;
                dfs(v);
            }
        };
        dfs(root);
    }

    int query(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u, v);
        int diff = depth[u] - depth[v];
        for (int j = 0; j < LOG; j++)
            if ((diff >> j) & 1) u = up[u][j];
        if (u == v) return u;
        for (int j = LOG - 1; j >= 0; j--)
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        return up[u][0];
    }
};

int main() {
    //        0
    //       / \
    //      1   2
    //     / \   \
    //    3   4   5
    std::vector<int> parents = {-1, 0, 0, 1, 1, 2};

    // 朴素法
    std::cout << "naive:\n";
    std::cout << "  LCA(3,4) = " << lca_naive(parents, 3, 4) << '\n';  // 输出: 1
    std::cout << "  LCA(3,5) = " << lca_naive(parents, 3, 5) << '\n';  // 输出: 0
    std::cout << "  LCA(1,4) = " << lca_naive(parents, 1, 4) << '\n';  // 输出: 1

    // 倍增法
    LCA lca(parents);
    std::cout << "binary lifting:\n";
    std::cout << "  LCA(3,4) = " << lca.query(3, 4) << '\n';  // 输出: 1
    std::cout << "  LCA(3,5) = " << lca.query(3, 5) << '\n';  // 输出: 0
    std::cout << "  LCA(1,4) = " << lca.query(1, 4) << '\n';  // 输出: 1
    std::cout << "  LCA(0,5) = " << lca.query(0, 5) << '\n';  // 输出: 0

    return 0;
}
