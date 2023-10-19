#include <iostream>
#include <vector>
#include <functional>

/*
快速排序
主要思路是选定一个数字，把小于该数字的数字放在该数字的左边，把大于该数字的数字放在该数字的右边
然后在该数字的左边和右边再各选定一个数字，进行递归

假设数组大小为n
时间复杂度平均O(nlogn)，最差O(n^2)
空间复杂度O(1)
*/
template<class T>
void quick_sort(std::vector<T>& ts) {
    // 数组大小
    int n = ts.size();
    std::function<void(int, int)> help = [&](int left, int right) {
        // 如果少于等于一个元素，不用进行排序
        if (left >= right) {
            return;
        }
        // 这里直接选定left为目标数字
        // 目标数字的选择直接影响到快速排序的效率，有更多好的选择方法，这边采取最简单的实现
        int pivot = left;
        int l = left + 1, r = right;
        while (l <= r) {
            // 将l指向大于等于目标数字的数字（不存在的话，l的值会是right + 1）
            while (l <= r && ts[l] < ts[pivot]) {
                l++;
            } 
            // 将r指向小于目标数字的数字（不存在的话，r的值会是left）
            while (l <= r && ts[r] >= ts[pivot]) {
                r--;
            }
            // 上面保证了l != r
            if (l < r) {
                // 如果l < r，直接对两个数字进行交换
                std::swap(ts[l], ts[r]);
                // 移动l和r
                l++, r--;
            } else {
                // 如果l > r，说明分组完毕，交换l - 1和pivot
                std::swap(ts[pivot], ts[l - 1]);
            }
        }
        // 递归[left, l - 2]和[l, right]
        help(left, l - 2);
        help(l, right);
    };
    help(0, n - 1);
}

template<class T, typename F>
void quick_sort(std::vector<T>& ts, F compare) {
    // 数组大小
    int n = ts.size();
    std::function<void(int, int)> help = [&](int left, int right) {
        // 如果少于等于一个元素，不用进行排序
        if (left >= right) {
            return;
        }
        // 这里直接选定left为目标数字
        // 目标数字的选择直接影响到快速排序的效率，有更多好的选择方法，这边采取最简单的实现
        int pivot = left;
        int l = left + 1, r = right;
        while (l <= r) {
            // 将l指向大于等于目标数字的数字（不存在的话，l的值会是right + 1）
            // 使用自定义比较函数
            while (l <= r && compare(ts[l], ts[pivot])) {
                l++;
            } 
            // 将r指向小于目标数字的数字（不存在的话，r的值会是left）
            // 使用自定义比较函数
            while (l <= r && !compare(ts[r], ts[pivot])) {
                r--;
            }
            // 上面保证了l != r
            if (l < r) {
                // 如果l < r，直接对两个数字进行交换
                std::swap(ts[l], ts[r]);
                // 移动l和r
                l++, r--;
            } else {
                // 如果l > r，说明分组完毕，交换l - 1和pivot
                std::swap(ts[pivot], ts[l - 1]);
            }
        }
        // 递归[left, l - 2]和[l, right]
        help(left, l - 2);
        help(l, right);
    };
    help(0, n - 1);   
}


int main(int argc, char const *argv[]) {
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        std::cin >> nums[i];
    }
    quick_sort(nums);
    for (int i = 0; i < n; i++) {
        std::cout << nums[i] << ' ';
    }
    std::cout << '\n';;
    quick_sort(nums, [](int a, int b) {
        return a > b;
    });
    for (int i = 0; i < n; i++) {
        std::cout << nums[i] << ' ';
    }
    std::cout << '\n';;
    return 0;
}
