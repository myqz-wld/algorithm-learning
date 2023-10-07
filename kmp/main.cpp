#include <iostream>
#include <vector>

/*
KMP
主要思路是通过已经匹配的局部信息来加速整体的匹配
假设在字符串 ababaccc 中匹配字符串 abac，在第一次匹配过程中：
0123456
ababacc
abac
在下标3处发现不一致（b != c），暴力的做法是将 abac 字符串右移1位后，再次从头进行匹配，
但我们已知 aba 是匹配的，所以可以直接右移2位（右移1位一定匹配不上），这个其实就是通过字符串 aba 的最大公共前后缀来加速匹配。
对于字符串 abac 来说，知道字符串 aba 的最大公共前后缀，也能加速寻找自身的最大公共前后缀，不是从头开始寻找：
abac
 abac
而是，可以直接进行如下尝试：
abac
  abac

假设字符串长度为m，目标字符串长度为n
时间复杂度O(m + n)
空间复杂度O(n)
*/


std::vector<int> get_next(std::string str) {
    int n = str.length();
    // next[i]表示[0:i]的最大公共前后缀长度
    // 初始全部默认为0
    std::vector<int> next(n, 0);
    // 下面这部分的时间复杂度为O(n)
    // 可以理解为每次计算完下标后，就将该下标入栈，一旦后续求最大公共长度时，越过了该下标，就将该下标出栈
    // 每个元素只会入栈出栈一次，所以时间复杂度为O(n)
    // 从下标1开始迭代，按照定义，下标0的最大公共前后缀长度就是0
    for (int i = 1; i < n; i++) {
        int index = i;
        do {
            // index - 1 下标的最大公共前后缀长度，也是我们尝试进行匹配的位置
            index = next[index - 1];
            if (str[index] == str[i]) {
                // 匹配成功就更新，并跳出迭代
                next[i] = index + 1;
                break;
            }
            // 匹配不成功就继续迭代尝试，直到最大公共前后缀长度为0
        } while (index != 0);
    }
    return next;
}

std::vector<int> kmp(std::string str, std::string target) {
    std::vector<int> next = get_next(target);
    std::vector<int> result;
    int p = 0, q = 0;
    while (p < str.length()) {
        if (str[p] == target[q]) {
            p++;
            q++;
        } else {
            if (q == 0) {
                // 如果在下标0就已经匹配不上，移动p指针
                p++;
            } else {
                // 根据已匹配的最大公共前后缀进行移动
                q = next[q - 1];
            }
        }

        if (q == target.length()) {
            // 匹配成功，加入到结果集合中
            result.push_back(p - q);
            // 根据已匹配的最大公共前后缀移动
            q = next[q - 1];
        }
    }
    return result;
}

int main(int argc, char const *argv[]) {
    std::string str, target;
    std::cin >> str >> target;
    std::vector<int> indexs = kmp(str, target);
    for (int index : indexs) {
        std::cout << index << ' ';
    }
    std::cout << '\n';
    return 0;
}
