#include <iostream>
#include <vector>
#include <queue>

/*
堆（Heap）
完全二叉树结构，满足堆性质：
- 最大堆：父节点 >= 子节点
- 最小堆：父节点 <= 子节点

数组存储，下标从0开始：
- 父节点：(i - 1) / 2
- 左子节点：2 * i + 1
- 右子节点：2 * i + 2

时间复杂度：
- push: O(logN)
- pop:  O(logN)
- top:  O(1)
*/

template<typename T, typename Compare = std::less<T>>
class Heap {
    std::vector<T> data;
    Compare cmp;

    void sift_up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (cmp(data[p], data[i])) {
                std::swap(data[p], data[i]);
                i = p;
            } else {
                break;
            }
        }
    }

    void sift_down(int i) {
        int n = data.size();
        while (2 * i + 1 < n) {
            int child = 2 * i + 1;
            if (child + 1 < n && cmp(data[child], data[child + 1])) {
                child++;
            }
            if (cmp(data[i], data[child])) {
                std::swap(data[i], data[child]);
                i = child;
            } else {
                break;
            }
        }
    }

public:
    Heap() = default;

    // 从数组建堆 O(n)
    Heap(const std::vector<T>& arr) : data(arr) {
        heapify();
    }

    // 建堆：从最后一个非叶子节点向前 sift_down
    void heapify() {
        for (int i = (int)data.size() / 2 - 1; i >= 0; i--) {
            sift_down(i);
        }
    }

    void push(const T& val) {
        data.push_back(val);
        sift_up(data.size() - 1);
    }

    void pop() {
        if (data.empty()) return;
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) sift_down(0);
    }

    const T& top() const { return data[0]; }
    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
};

// 类型别名
template<typename T>
using MaxHeap = Heap<T, std::less<T>>;

template<typename T>
using MinHeap = Heap<T, std::greater<T>>;

int main() {
    // 最大堆测试
    MaxHeap<int> maxH;
    maxH.push(3);
    maxH.push(1);
    maxH.push(4);
    maxH.push(1);
    maxH.push(5);
    std::cout << "MaxHeap: ";
    while (!maxH.empty()) {
        std::cout << maxH.top() << ' ';
        maxH.pop();
    }
    std::cout << '\n';  // 输出: 5 4 3 1 1

    // 最小堆测试
    MinHeap<int> minH;
    minH.push(3);
    minH.push(1);
    minH.push(4);
    minH.push(1);
    minH.push(5);
    std::cout << "MinHeap: ";
    while (!minH.empty()) {
        std::cout << minH.top() << ' ';
        minH.pop();
    }
    std::cout << '\n';  // 输出: 1 1 3 4 5

    // 建堆测试 O(n)
    std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
    MaxHeap<int> heapFromArr(arr);
    std::cout << "Heapify: ";
    while (!heapFromArr.empty()) {
        std::cout << heapFromArr.top() << ' ';
        heapFromArr.pop();
    }
    std::cout << '\n';  // 输出: 9 6 5 4 3 2 1 1

    return 0;
}

/*
=== STL priority_queue 用法 ===

#include <queue>

// 最大堆（默认）
std::priority_queue<int> maxPQ;

// 最小堆
std::priority_queue<int, std::vector<int>, std::greater<int>> minPQ;

// 常用操作
pq.push(x);     // 插入元素
pq.pop();       // 弹出堆顶
pq.top();       // 获取堆顶
pq.size();      // 元素个数
pq.empty();     // 是否为空

// 自定义比较器（例：按pair.second的最小堆）
auto cmp = [](auto& a, auto& b) { return a.second > b.second; };
std::priority_queue<std::pair<int,int>,
                    std::vector<std::pair<int,int>>,
                    decltype(cmp)> pq(cmp);

=== 懒删除（Lazy Deletion）===

priority_queue 只能 pop 堆顶，无法删除任意元素。
懒删除：标记要删除的元素，取堆顶时跳过已删除的。

// 示例：支持懒删除的最大堆
std::priority_queue<int> pq;
std::unordered_map<int, int> del_cnt;  // 记录每个值的删除次数

// 插入
pq.push(x);

// 懒删除（只标记，不真正删除）
del_cnt[x]++;

// 获取真正的堆顶（跳过已删除元素）
while (!pq.empty() && del_cnt[pq.top()] > 0) {
    del_cnt[pq.top()]--;
    pq.pop();
}
int top_val = pq.top();

// 封装版本
template<typename T, typename Compare = std::less<T>>
class LazyHeap {
    std::priority_queue<T, std::vector<T>, Compare> pq;
    std::unordered_map<T, int> del_cnt;

    void purge() {
        while (!pq.empty() && del_cnt[pq.top()] > 0) {
            del_cnt[pq.top()]--;
            pq.pop();
        }
    }

public:
    void push(const T& x) { pq.push(x); }

    void erase(const T& x) { del_cnt[x]++; }  // 懒删除

    T top() { purge(); return pq.top(); }

    void pop() { purge(); pq.pop(); }

    bool empty() { purge(); return pq.empty(); }
};
*/
