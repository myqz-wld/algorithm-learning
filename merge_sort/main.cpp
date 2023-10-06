#include <iostream>
#include <vector>
#include <functional>

/*
归并排序
主要思路是将大数组拆分为两个小数组，对两个小数组排序后（可以递归地调用归并排序），再对大数组进行排序。
假设大数组的元素数量为n，在两个小数组有序的基础上，对大数组进行排序的时间复杂度为O(n)，所以整体的时间复杂度是：
T(n) = 2 * T(n / 2) + O(n)
一直递推这个公式，可以得到：
T(n) = O(nlog(n))
空间复杂度为O(n)
*/
template<class T>
void merge_sort(std::vector<T>& ts) {
    int n = ts.size();
    // 临时空间
    std::vector<T> temp(n);
    // 对[left, right]区域内的元素进行排序
    std::function<void(int, int)> help = [&](int left, int right) -> void {
        // 如果只有一个元素，不需要排序
        if (left == right) {
            return;
        }
        int middle = left + (right - left) / 2;
        // 对[left, middle]排序
        help(left, middle);
        // 对[middle + 1, right]排序
        help(middle + 1, right);
        // 基于两个小数组有序的基础上，对大数组进行排序
        int p = left, q = middle + 1, index = 0;
        while (p <= middle && q <= right) {
            // T视情况重载 < 操作符
            if (ts[p] < ts[q]) {
                temp[index++] = ts[p++];
            } else {
                temp[index++] = ts[q++];
            }
        }
        while (p <= middle) {
            temp[index++] = ts[p++];
        }
        while (q <= right) {
            temp[index++] = ts[q++];
        }
        // 将临时空间里的元素复制给原数组
        for (int i = 0; i < index; i++) {
            ts[i + left] = temp[i];
        }
    };
    help(0, n - 1);
}

// 需要传入自定义比较函数
template<class T, typename F>
void merge_sort(std::vector<T>& ts, F compare) {
    int n = ts.size();
    // 临时空间
    std::vector<T> temp(n);
    // 对[left, right]区域内的元素进行排序
    std::function<void(int, int)> help = [&](int left, int right) -> void {
        // 如果只有一个元素，不需要排序
        if (left == right) {
            return;
        }
        int middle = left + (right - left) / 2;
        // 对[left, middle]排序
        help(left, middle);
        // 对[middle + 1, right]排序
        help(middle + 1, right);
        // 基于两个小数组有序的基础上，对大数组进行排序
        int p = left, q = middle + 1, index = 0;
        while (p <= middle && q <= right) {
            // 使用自定义比较函数
            if (compare(ts[p], ts[q])) {
                temp[index++] = ts[p++];
            } else {
                temp[index++] = ts[q++];
            }
        }
        while (p <= middle) {
            temp[index++] = ts[p++];
        }
        while (q <= right) {
            temp[index++] = ts[q++];
        }
        // 将临时空间里的元素复制给原数组
        for (int i = 0; i < index; i++) {
            ts[i + left] = temp[i];
        }
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
    merge_sort(nums);
    for (int i = 0; i < n; i++) {
        std::cout << nums[i] << ' ';
    }
    std::cout << '\n';
    merge_sort(nums, [](int a, int b) {
        return a > b;
    });
    for (int i = 0; i < n; i++) {
        std::cout << nums[i] << ' ';
    }
    std::cout << '\n';
    return 0;
}
