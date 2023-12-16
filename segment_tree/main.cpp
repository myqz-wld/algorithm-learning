#include <iostream>

/*
线段树模版
*/
template<class V, class L>
class segment_tree_node {
private:
    int left, right;
    V val;
    L lazy;
    segment_tree_node<V, L>* ln = nullptr, * rn = nullptr;

    using apply_lazy = V (*)(V, L, int, int);
    using val_to_lazy = L (*)(V);
    using merge_lazy = L (*)(L, L);
    using merge_val = V (*)(V, V);

public:
    segment_tree_node(int left, int right, V val, L lazy) {
        this->left = left;
        this->right = right;
        this->val = val;
        this->lazy = lazy;
    }

    void lazy_down(V defautl_val, L default_lazy, apply_lazy al, merge_lazy ml) {
        int middle = (left + right) / 2;
        if (!ln) {
            ln = new segment_tree_node<V, L>(left, middle, defautl_val, default_lazy);
        }
        if (!rn) {
            rn = new segment_tree_node<V, L>(middle + 1, right, defautl_val, default_lazy);
        }
        ln->val = al(ln->val, lazy, ln->left, ln->right);
        ln->lazy = ml(ln->lazy, lazy);
        rn->val = al(rn->val, lazy, rn->left, rn->right);
        rn->lazy = ml(rn->lazy, lazy);
        lazy = default_lazy;
    }

    void add(int l, int r, V offset_val, V default_val, L default_lazy, apply_lazy al, val_to_lazy v2l, merge_lazy ml, merge_val mv) {
        if (r < left || l > right) {
            return;
        }
        if (l <= left && r >= right) {
            L offset_lazy = v2l(offset_val);
            val = al(val, offset_lazy, left, right);
            lazy = ml(lazy, offset_lazy);
            return;
        }
        lazy_down(default_val, default_lazy, al, ml);
        int middle = (left + right) / 2;
        if (l <= middle) {
            if (!ln) {
                ln = new segment_tree_node<V, L>(left, middle, default_val, default_lazy);
            }
            ln->add(l, r, offset_val, default_val, default_lazy, al, v2l, ml, mv);
        }
        if (r > middle) {
            if (!rn) {
                rn = new segment_tree_node<V, L>(middle + 1, right, default_val, default_lazy);
            }
            rn->add(l, r, offset_val, default_val, default_lazy, al, v2l, ml, mv);
        }
        val = mv(ln ? ln->val : default_val, rn ? rn->val : default_val);       
    }

    V query(int l, int r, V default_val, L default_lazy, apply_lazy al, val_to_lazy v2l, merge_lazy ml, merge_val mv) {
        if (r < left || l > right) {
            return default_val;
        }
        if (l <= left && r >= right) {
            return val;
        }
        lazy_down(default_val, default_lazy, al, ml);
        int middle = (left + right) / 2;
        V left_val = default_val, right_val = default_val;
        if (l <= middle) {
            if (!ln) {
                ln = new segment_tree_node<V, L>(left, middle, default_val, default_lazy);
            }
            left_val = ln->query(l, r, default_val, default_lazy, al, v2l, ml, mv);
        } 
        if (r > middle) {
            if (!rn) {
                rn = new segment_tree_node<V, L>(middle + 1, right, default_val, default_lazy);
            }
            right_val = rn->query(l, r, default_val, default_lazy, al, v2l, ml, mv);
        }
        return mv(left_val, right_val);
    }
};

template<class V, class L>
class segment_tree {
private:
    // 根节点
    segment_tree_node<V, L>* root;
    // 默认值
    V default_val;
    // 默认懒加载标志
    L default_lazy;
    // 处理函数
    using apply_lazy = V (*)(V, L, int, int);
    using val_to_lazy = L (*)(V);
    using merge_lazy = L (*)(L, L);
    using merge_val = V (*)(V, V);
    apply_lazy al;
    val_to_lazy v2l;
    merge_lazy ml;
    merge_val mv;

public:
    segment_tree(int left, int right, V default_val, L default_lazy, apply_lazy al, val_to_lazy v2l, merge_lazy ml, merge_val mv) {
        this->root = new segment_tree_node<V, L>(left, right, default_val, default_lazy);
        this->default_val = default_val;
        this->default_lazy = default_lazy;
        this->al = al;
        this->v2l = v2l;
        this->ml = ml;
        this->mv = mv;
    }

    void add(int l, int r, V offset_val) {
        this->root->add(l, r, offset_val, default_val, default_lazy, al, v2l, ml, mv);
    }

    V query(int l, int r) {
        return this->root->query(l, r, default_val, default_lazy, al, v2l, ml, mv);
    }
};

class CountIntervals {
private:
    segment_tree<int, bool>* st;
public:
    CountIntervals() {
        auto al = [](int val, bool lazy, int left, int right) -> int {
            if (lazy) {
                return right - left + 1;
            } else {
                return val;
            }
        };
        auto v2l = [](int val) -> bool {
            return true;
        };
        auto ml = [](bool a, bool b) -> bool {
            return a | b;
        };
        auto mv = [](int a, int b) -> int {
            return a + b;
        };
        st = new segment_tree<int, bool>(1, 1e9, 0, false, al, v2l, ml, mv);
    }
    
    void add(int left, int right) {
        st->add(left, right, 1);
    }
    
    int count() {
        return st->query(1, 1e9);
    }
};

int main(int argc, char const *argv[]) {
    CountIntervals ci;
    ci.add(2, 3);
    ci.add(7, 10);
    std::cout << ci.count() << '\n';
    ci.add(5, 8);
    std::cout << ci.count() << '\n';
    return 0;
}
