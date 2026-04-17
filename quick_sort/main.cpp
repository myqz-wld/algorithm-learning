#include <iostream>
#include <vector>
#include <functional>

/*
快速排序 (Quick Sort)
选取 pivot，将小于 pivot 的放左边，大于的放右边，递归处理两半。

Lomuto 分区过程（以首元素 3 为 pivot）：
  [3, 5, 1, 4, 2]
   ^pivot
  用指针 i 维护 "< pivot 区域" 的右边界，j 扫描数组：
  j=1: 5 >= 3 → 跳过
  j=2: 1 < 3  → i++, swap(arr[1],arr[2]) → [3, 1, 5, 4, 2]
  j=3: 4 >= 3 → 跳过
  j=4: 2 < 3  → i++, swap(arr[2],arr[4]) → [3, 1, 2, 4, 5]
  最后 swap(pivot, arr[i]) → [2, 1, 3, 4, 5]
  pivot 归位到下标 2，递归排序左右两半

时间复杂度：平均 O(nlogn)，最差 O(n²)（有序数组 + 固定选首元素）
空间复杂度：O(logn) 递归栈
不稳定排序
*/

template<typename T, typename Compare = std::less<T>>
void quick_sort(std::vector<T>& arr, Compare cmp = Compare()) {
    int n = arr.size();
    if (n <= 1) return;
    std::function<void(int, int)> sort = [&](int lo, int hi) {
        if (lo >= hi) return;
        int i = lo;
        for (int j = lo + 1; j <= hi; j++)
            if (cmp(arr[j], arr[lo]))
                std::swap(arr[++i], arr[j]);
        std::swap(arr[lo], arr[i]);
        sort(lo, i - 1);
        sort(i + 1, hi);
    };
    sort(0, n - 1);
}

int main() {
    // 默认升序
    std::vector<int> a = {5, 3, 1, 4, 2};
    quick_sort(a);
    std::cout << "asc:  ";
    for (int x : a) std::cout << x << ' ';
    std::cout << '\n';  // 输出: 1 2 3 4 5

    // 自定义降序
    quick_sort(a, std::greater<int>());
    std::cout << "desc: ";
    for (int x : a) std::cout << x << ' ';
    std::cout << '\n';  // 输出: 5 4 3 2 1

    // 单元素和空数组
    std::vector<int> b = {42};
    quick_sort(b);
    std::cout << "single: " << b[0] << '\n';  // 输出: 42

    return 0;
}

/*
=== STL sort 用法 ===

#include <algorithm>

// std::sort 内部使用 IntroSort（快排变体），是实践中最常用的排序
std::vector<int> v = {5, 3, 1, 4, 2};
std::sort(v.begin(), v.end());                         // 升序
std::sort(v.begin(), v.end(), std::greater<int>());    // 降序

// nth_element: 快速选择算法，O(n) 找第 k 小元素
std::nth_element(v.begin(), v.begin() + 2, v.end());   // v[2] 是第 3 小的元素

// partial_sort: 部分排序，O(nlogk) 只排前 k 个
std::partial_sort(v.begin(), v.begin() + 3, v.end());  // 前 3 个元素有序
*/
