#include <iostream>
#include <vector>
#include <functional>

/*
最近公共祖先
*/

/*
parents[i]代表i的父节点，parents[i] == -1，代表i是根节点
查询a和b的最近公共祖先
*/
int lca(std::vector<int>& parents, int a, int b) {
    // 节点总数
    int n = parents.size();
    // 建图
    std::vector<std::vector<int>> grid(n);
    int root;
    for (int i = 0; i < n; i++) {
        if (parents[i] == -1) {
            root = i;
            continue;
        }
        grid[parents[i]].push_back(i);
    }

    // dfs遍历查找最近公共祖先
    // 0-子树中不存在a和b 1-子树中存在a 2-子树中存在b 3-子树中存在a和b
    int result = -1;
    std::function<int(int)> dfs = [&](int cur) -> int {
        int flag = 0;
        if (cur == a) {
            flag |= 1;
        }
        if (cur == b) {
            flag |= 2;
        }
        for (int next : grid[cur]) {
            flag |= dfs(next);
        }
        if (flag == 3 && result == -1) {
            // 当flag第一次变为3的时候，代表cur是a和b的最近公共祖先
            result = cur;
        }
        return flag;
    };
    dfs(root);
    return result;
}

/*
主要思路是通过倍增法来加速最近公共祖先的寻找

parents[i]代表i的父节点，parents[i] == -1，代表i是根节点
queries[i]代表查询请求，要查询queries[i][0]和queries[i][1]的最近公共祖先
*/
std::vector<int> lca(std::vector<int>& parents, std::vector<std::vector<int>>& queries) {
    // 节点总数
    int n = parents.size();
    // 建图计算节点深度
    std::vector<std::vector<int>> grid(n);
    int root;
    for (int i = 0; i < n; i++) {
        if (parents[i] == -1) {
            root = i;
            continue;
        }
        grid[parents[i]].push_back(i);
    }
    std::vector<int> depths(n);
    depths[root] = 0;
    std::function<void(int)> dfs = [&](int cur) -> void {
        for (int next : grid[cur]) {
            depths[next] = depths[cur] + 1;
            dfs(next);
        }
    };
    dfs(root);
    // 计算倍增的次数
    int k = 0;
    while (n >= (1 << k)) {
        k++;
    }
    k++;
    // ances[i][j]表示i的2^j级的祖先节点
    // 可以推得：ances[i][j] = ances[ances[i][j - 1]][j - 1]
    std::vector<std::vector<int>> ances(n, std::vector<int>(k));
    for (int i = 0; i < n; i++) {
        // 特殊处理，超过节点深度的都指向根节点
        ances[i][0] = parents[i] == -1 ? i : parents[i]; 
    }
    for (int i = 1; i < k; i++) {
        for (int j = 0; j < n; j++) {
            ances[j][i] = ances[ances[j][i - 1]][i - 1];
        }
    }
    // 定义查找最近公共祖先的函数
    std::function<int(int, int)> find = [&](int x, int y) -> int {
        if (depths[x] < depths[y]) {
            // 让x节点的深度大于等于y节点的深度
            return find(y, x);
        }
        if (depths[x] != depths[y]) {
            // 如果x节点的深度不等于y节点的深度，先找到与y节点同一深度的x节点的祖先
            for (int i = k - 1; i >= 0; i--) {
                if (depths[ances[x][i]] >= depths[y]) {
                    x = ances[x][i];
                }
            }
        }
        if (x != y) {
            // 如果x节点不等于y节点，找x节点和y节点的最近公共祖先
            for (int i = k - 1; i >= 0; i--) {
                if (ances[x][i] != ances[y][i]) {
                    x = ances[x][i];
                    y = ances[y][i];
                }
            }
            x = ances[x][0];
            y = ances[y][0];
        }
        return x;
    };
    // 进行查询
    int m = queries.size();
    std::vector<int> result(m);
    for (int i = 0; i < m; i++) {
        result[i] = find(queries[i][0], queries[i][1]);
    }
    return result;
}


int main(int argc, char const *argv[]) {
    int n;
    std::cin >> n;
    std::vector<int> parents(n);
    for (int i = 0; i < n; i++) {
        std::cin >> parents[i];
    }
    int m;
    std::cin >> m;
    std::vector<std::vector<int>> queries(m, std::vector<int>(2));
    for (int i = 0; i < m; i++) {
        std::cin >> queries[i][0] >> queries[i][1];
    }
    for (int i = 0; i < m; i++) {
        std::cout << lca(parents, queries[i][0], queries[i][1]) << ' ';
    }
    std::cout << '\n';
    std::vector<int> result = lca(parents, queries);
    for (int i = 0; i < m; i++) {
        std::cout << result[i] << ' ';
    }
    std::cout << '\n';
    return 0;
}
