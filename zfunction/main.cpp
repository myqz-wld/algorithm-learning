#include <iostream>
#include <vector>

/*
Z函数
和马拉车算法的核心思路比较类似

假设字符串长度为n
时间复杂度O(n)
空间复杂度O(n)
*/
std::vector<int> zfunction(std::string str) {
    int n = str.length();
    // result[i]表示以下标i为起点的字符串，和以下标0为起点的字符串的最大公共前缀的长度
    // 初始全部默认为0
    std::vector<int> result(n, 0);
    // right记录所有最大公共前缀中最右侧的端点，left是其对应的左侧端点，初始默认为0
    int left = 0, right = 0;
    // 从下标1开始，按照定义，下标0的最大公共前缀为0
    for (int i = 1; i < n; i++) {
        int index = 0;
        if (right >= i) {
            // 因为[left, right]等价于[0, right - left]，所以能利用 i - left 下标的信息加速
            index = std::min(result[i - left], right - i + 1);
        }
        while (i + index < n && str[index] == str[i + index]) {
            index++;
        }
        result[i] = index;
        if (i + index - 1 > right) {
            // 如果右端点超过了right，对right和left进行更新
            right = i + index - 1;
            left = i;
        }
    }
    return result;
}

int main(int argc, char const *argv[]) {
    std::string s;
    std::cin >> s;
    std::vector<int> z = zfunction(s);
    for (int i = 0; i < s.length(); i++) {
        std::cout << z[i] << ' ';
    }
    std::cout << std::endl;
    return 0;
}
