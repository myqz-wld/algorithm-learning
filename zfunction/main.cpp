#include <iostream>
#include <vector>
#include <string>

/*
Z 函数 (Z-Function)
z[i] = 从位置 i 开始的后缀与整个字符串的最长公共前缀长度。
z[0] 定义为 0（或字符串长度，取决于定义，这里用 0）。

=== 核心思想 ===

维护一个"Z-box" [l, r]，表示已知的最右匹配区间（s[l..r] == s[0..r-l]）。

对于每个位置 i：
1. 若 i <= r：利用已知信息，z[i] >= min(z[i-l], r-i+1)
   因为 s[l..r] == s[0..r-l]，所以 s[i..r] == s[i-l..r-l]
2. 然后暴力扩展 z[i]
3. 若 i + z[i] - 1 > r：更新 Z-box

以 "aabxaab" 为例：
  i=0:                          z[0] = 0 (定义)
  i=1: 'a' vs 'a' → 匹配       z[1] = 1, 更新 Z-box [1,1]
       'b' vs 'a' → 停
  i=2: 'b' vs 'a' → 停          z[2] = 0
  i=3: 'x' vs 'a' → 停          z[3] = 0
  i=4: i > r, 暴力匹配 "aab"    z[4] = 3, 更新 Z-box [4,6]
  i=5: i <= r, z[i-l]=z[1]=1    z[5] = 1
  i=6: i <= r, z[i-l]=z[2]=0    z[6] = 0
  结果: [0, 1, 0, 0, 3, 1, 0]

=== 字符串匹配 ===

构造 pattern + "$" + text，计算 Z 数组。
z[i] == len(pattern) 的位置即为匹配位置。

时间复杂度：O(n)
空间复杂度：O(n)
*/

class ZFunction {
public:
    static std::vector<int> compute(const std::string& s) {
        int n = s.size();
        std::vector<int> z(n, 0);
        for (int i = 1, l = 0, r = 0; i < n; i++) {
            if (r >= i)
                z[i] = std::min(z[i - l], r - i + 1);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
                z[i]++;
            if (i + z[i] - 1 > r)
                l = i, r = i + z[i] - 1;
        }
        return z;
    }

    // 在 text 中查找 pattern 的所有出现位置
    static std::vector<int> search(const std::string& pattern, const std::string& text) {
        std::string concat = pattern + "$" + text;
        auto z = compute(concat);
        int m = pattern.size();
        std::vector<int> res;
        for (int i = m + 1; i < (int)concat.size(); i++)
            if (z[i] == m)
                res.push_back(i - m - 1);
        return res;
    }
};

int main() {
    // Z 数组计算
    auto z = ZFunction::compute("aabxaab");
    std::cout << "z-array: ";
    for (int v : z) std::cout << v << ' ';
    std::cout << '\n';  // 输出: 0 1 0 0 3 1 0

    // 字符串匹配
    auto pos = ZFunction::search("ab", "ababab");
    std::cout << "search:  ";
    for (int p : pos) std::cout << p << ' ';
    std::cout << '\n';  // 输出: 0 2 4

    // 周期检测：字符串 s 的最短周期 = n - z[i]（若 n % (n-z[i]) == 0 且 z[i] + i == n）
    std::string s = "abcabcabc";
    auto z2 = ZFunction::compute(s);
    int n = s.size();
    int period = n;
    for (int i = 1; i < n; i++) {
        if (z2[i] + i == n && n % i == 0) {
            period = i;
            break;
        }
    }
    std::cout << "period of \"" << s << "\": " << period << '\n';  // 输出: 3

    return 0;
}
