#include <iostream>
#include <vector>
#include <algorithm>

/*
基数排序 (Radix Sort)
从低位到高位逐位排序，每位使用计数排序（稳定排序）保证正确性。
仅适用于非负整数，负数需特殊处理。

以十进制排序 [170, 45, 75, 90, 802, 24, 2, 66] 为例：
  个位排序: [170, 90, 802, 2, 24, 45, 75, 66]
  十位排序: [802, 2, 24, 45, 66, 170, 75, 90]
  百位排序: [2, 24, 45, 66, 75, 90, 170, 802]

每位排序使用计数排序，关键点：
- 倒序遍历原数组保证稳定性（相同数位的元素保持上一轮的相对顺序）
- 计数数组做前缀和，得到每个数位值的排名上界

时间复杂度：O(n * log_k(m))  n=数组大小, m=最大值, k=进制
空间复杂度：O(n + k)
稳定排序
*/

template<int K = 10>
void radix_sort(std::vector<int>& nums) {
    if (nums.empty()) return;
    int n = nums.size();
    int max_val = *std::max_element(nums.begin(), nums.end());
    std::vector<int> buf(n);
    std::vector<int> cnt(K);

    for (int base = 1; base <= max_val; base *= K) {
        std::fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; i++)
            cnt[(nums[i] / base) % K]++;
        for (int i = 1; i < K; i++)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) {
            int d = (nums[i] / base) % K;
            buf[--cnt[d]] = nums[i];
        }
        nums = buf;
    }
}

int main() {
    // 十进制基数排序
    std::vector<int> a = {170, 45, 75, 90, 802, 24, 2, 66};
    radix_sort(a);
    std::cout << "base10: ";
    for (int x : a) std::cout << x << ' ';
    std::cout << '\n';  // 输出: 2 24 45 66 75 90 170 802

    // 二进制基数排序
    std::vector<int> b = {5, 3, 8, 1, 7, 2};
    radix_sort<2>(b);
    std::cout << "base2:  ";
    for (int x : b) std::cout << x << ' ';
    std::cout << '\n';  // 输出: 1 2 3 5 7 8

    return 0;
}
