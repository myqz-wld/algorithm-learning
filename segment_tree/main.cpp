#include <iostream>

/*
动态开点线段树 (Dynamic Segment Tree)
支持懒标记的通用线段树模板，节点按需创建，适用于值域很大的场景（如 [1, 10^9]）。

=== 核心概念 ===

线段树将区间递归二分，每个节点维护一个子区间的聚合值：
       [1,8]
      /     \
   [1,4]   [5,8]
   /  \    /  \
 [1,2][3,4][5,6][7,8]

区间更新 + 区间查询 → 需要懒标记（Lazy Propagation）：
  更新时不立即递归到叶子，而是在节点上打标记，
  下次访问子节点时再下推（push_down），均摊 O(logN)。

动态开点：
  不预先分配 4N 节点，按需创建，适合值域大但操作少的场景。

=== 模板参数 ===

使用时需提供四个函数：
- apply_lazy:  懒标记应用到节点值  (val, lazy, left, right) -> new_val
- val_to_lazy: 更新值转为懒标记    (val) -> lazy
- merge_lazy:  合并两个懒标记      (old, new) -> merged
- merge_val:   合并两个子节点的值   (left_val, right_val) -> parent_val

要求：apply_lazy(val, default_lazy, l, r) == val（默认懒标记是恒等操作）

时间复杂度：单次操作 O(logN)  N=值域范围
空间复杂度：O(QlogN)         Q=操作次数
*/

template<typename V, typename L>
class SegTree {
    using ApplyLazy = V(*)(V, L, int, int);
    using ValToLazy = L(*)(V);
    using MergeLazy = L(*)(L, L);
    using MergeVal  = V(*)(V, V);

    struct Node {
        int l, r;
        V val;
        L lazy;
        Node *ln = nullptr, *rn = nullptr;
        Node(int l, int r, V v, L lz) : l(l), r(r), val(v), lazy(lz) {}
    };

    Node* root;
    V def_v;
    L def_l;
    ApplyLazy al;
    ValToLazy v2l;
    MergeLazy ml;
    MergeVal  mv;

    void push_down(Node* nd) {
        int mid = (nd->l + nd->r) / 2;
        if (!nd->ln) nd->ln = new Node(nd->l, mid, def_v, def_l);
        if (!nd->rn) nd->rn = new Node(mid + 1, nd->r, def_v, def_l);
        nd->ln->val = al(nd->ln->val, nd->lazy, nd->ln->l, nd->ln->r);
        nd->ln->lazy = ml(nd->ln->lazy, nd->lazy);
        nd->rn->val = al(nd->rn->val, nd->lazy, nd->rn->l, nd->rn->r);
        nd->rn->lazy = ml(nd->rn->lazy, nd->lazy);
        nd->lazy = def_l;
    }

    void update(Node* nd, int l, int r, V val) {
        if (r < nd->l || l > nd->r) return;
        if (l <= nd->l && r >= nd->r) {
            L lz = v2l(val);
            nd->val = al(nd->val, lz, nd->l, nd->r);
            nd->lazy = ml(nd->lazy, lz);
            return;
        }
        push_down(nd);
        update(nd->ln, l, r, val);
        update(nd->rn, l, r, val);
        nd->val = mv(nd->ln->val, nd->rn->val);
    }

    V query(Node* nd, int l, int r) {
        if (!nd || r < nd->l || l > nd->r) return def_v;
        if (l <= nd->l && r >= nd->r) return nd->val;
        push_down(nd);
        return mv(query(nd->ln, l, r), query(nd->rn, l, r));
    }

public:
    SegTree(int l, int r, V def_v, L def_l,
            ApplyLazy al, ValToLazy v2l, MergeLazy ml, MergeVal mv)
        : def_v(def_v), def_l(def_l), al(al), v2l(v2l), ml(ml), mv(mv) {
        root = new Node(l, r, def_v, def_l);
    }

    void update(int l, int r, V val) { update(root, l, r, val); }
    V query(int l, int r) { return query(root, l, r); }
};

int main() {
    // 示例1：区间加 + 区间求和
    SegTree<long long, long long> sum_tree(
        1, 10, 0LL, 0LL,
        [](long long val, long long lazy, int l, int r) -> long long {
            return val + lazy * (r - l + 1);
        },
        [](long long val) -> long long { return val; },
        [](long long a, long long b) -> long long { return a + b; },
        [](long long a, long long b) -> long long { return a + b; }
    );
    sum_tree.update(1, 5, 3);       // [1,5] 每个位置 +3
    sum_tree.update(3, 8, 2);       // [3,8] 每个位置 +2
    std::cout << "sum[1,5]:  " << sum_tree.query(1, 5) << '\n';   // 输出: 21
    std::cout << "sum[3,8]:  " << sum_tree.query(3, 8) << '\n';   // 输出: 21
    std::cout << "sum[1,10]: " << sum_tree.query(1, 10) << '\n';  // 输出: 27

    // 示例2：区间覆盖 + 统计覆盖长度（CountIntervals）
    SegTree<int, bool> ci(
        1, 20, 0, false,
        [](int val, bool lazy, int l, int r) -> int {
            return lazy ? r - l + 1 : val;
        },
        [](int) -> bool { return true; },
        [](bool a, bool b) -> bool { return a | b; },
        [](int a, int b) -> int { return a + b; }
    );
    ci.update(2, 3, 1);
    ci.update(7, 10, 1);
    std::cout << "count: " << ci.query(1, 20) << '\n';  // 输出: 6
    ci.update(5, 8, 1);
    std::cout << "count: " << ci.query(1, 20) << '\n';  // 输出: 8

    return 0;
}
