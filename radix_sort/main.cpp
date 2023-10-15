#include <iostream>
#include <algorithm>
#include <vector>

/*
基数排序
主要思路是从低数位到高数位逐数位进行排序，可以是十进制，也可以是二进制
注意！对负数要特殊处理，这里只涵盖非负数情况

假设数组大小为n，数组中最大数为m，进制为k
时间复杂度O(nlog(k, m))
空间复杂度O(n + k)
*/
template<int k>
void radix_sort(std::vector<int>& nums) {
    // 数组大小
    int n = nums.size();
    // 找数组中的最大值
    int max_val = *std::max_element(nums.begin(), nums.end());
    // 排名，ranks[i]表示排名为i的数字是多少
    std::vector<int> ranks(n);
    // 统计数位上值个数
    std::vector<int> counts(k);
    // 代表数位
    int pivot = 1;
    while (max_val >= pivot) {
        // 初始化统计数据
        for (int i = 0; i < k; i++) {
            counts[i] = 0;
        }
        // 进行统计
        for (int i = 0; i < n; i++) {
            counts[(nums[i] / pivot) % k]++;
        }
        // 累计，得到该数位上的排名结果
        for (int i = 1; i < k; i++) {
            counts[i] += counts[i - 1];
        }
        // 根据排名结果对数组进行排序，排序结果暂时放到ranks数组中
        // 为什么是从下标n - 1开始？
        // 因为经过上一轮的分配，靠后的元素需要拿到更加后面的名次
        for (int i = n - 1; i >= 0; i--) {
            int val = (nums[i] / pivot) % k;
            ranks[counts[val] - 1] = nums[i];
            counts[val]--;
        }
        // 从ranks搬运数据到原数组
        for (int i = 0; i < n; i++) {
            nums[i] = ranks[i];
        }
        // 进位
        pivot *= k;
    }
}

int main(int argc, char const *argv[]) {
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        std::cin >> nums[i];
    }
    radix_sort<10>(nums);
    for (int i = 0; i < n; i++) {
        std::cout << nums[i] << ' ';
    }
    std::cout << '\n';
    return 0;
}
