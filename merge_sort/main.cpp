#include <iostream>
#include <vector>
#include <functional>

/*
归并排序 (Merge Sort)
分治思想：将数组一分为二，递归排序后合并两个有序子数组。

合并过程（两个有序数组合为一个有序数组）：
  左: [1, 3, 5]   右: [2, 4, 6]
  双指针从头比较，较小的先放入结果：
  → [1] → [1,2] → [1,2,3] → [1,2,3,4] → [1,2,3,4,5] → [1,2,3,4,5,6]

递推公式：T(n) = 2T(n/2) + O(n)
  每层合并总共 O(n)，共 logn 层 → O(nlogn)

时间复杂度：O(nlogn)  最好/最坏/平均均如此
空间复杂度：O(n)
稳定排序
*/

template<typename T, typename Compare = std::less<T>>
void merge_sort(std::vector<T>& arr, Compare cmp = Compare()) {
    int n = arr.size();
    if (n <= 1) return;
    std::vector<T> tmp(n);
    std::function<void(int, int)> sort = [&](int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        sort(l, m);
        sort(m + 1, r);
        int i = l, j = m + 1, k = 0;
        while (i <= m && j <= r)
            tmp[k++] = !cmp(arr[j], arr[i]) ? arr[i++] : arr[j++];
        while (i <= m) tmp[k++] = arr[i++];
        while (j <= r) tmp[k++] = arr[j++];
        for (int i = 0; i < k; i++) arr[l + i] = tmp[i];
    };
    sort(0, n - 1);
}

int main() {
    // 默认升序
    std::vector<int> a = {5, 3, 1, 4, 2};
    merge_sort(a);
    std::cout << "asc:  ";
    for (int x : a) std::cout << x << ' ';
    std::cout << '\n';  // 输出: 1 2 3 4 5

    // 自定义降序
    merge_sort(a, std::greater<int>());
    std::cout << "desc: ";
    for (int x : a) std::cout << x << ' ';
    std::cout << '\n';  // 输出: 5 4 3 2 1

    // 稳定性验证：相等元素保持原始顺序
    std::vector<std::pair<int,char>> b = {{3,'a'}, {1,'b'}, {3,'c'}, {1,'d'}};
    merge_sort(b, [](auto& x, auto& y) { return x.first < y.first; });
    std::cout << "stable: ";
    for (auto& [v, c] : b) std::cout << v << c << ' ';
    std::cout << '\n';  // 输出: 1b 1d 3a 3c

    return 0;
}

/*
=== STL sort 用法 ===

#include <algorithm>

std::vector<int> v = {5, 3, 1, 4, 2};

// 升序（默认）
std::sort(v.begin(), v.end());

// 降序
std::sort(v.begin(), v.end(), std::greater<int>());

// 自定义比较器
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

// 稳定排序（保持相等元素的相对顺序，归并排序实现）
std::stable_sort(v.begin(), v.end());

// 注意：std::sort 使用 IntroSort（快排+堆排+插入排序），平均 O(nlogn)
//       std::stable_sort 使用归并排序，O(nlogn) 但需要 O(n) 额外空间
*/
