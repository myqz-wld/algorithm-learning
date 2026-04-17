#include <iostream>
#include <vector>
#include <string>

/*
KMP (Knuth-Morris-Pratt)
字符串匹配算法，通过预处理模式串的最长公共前后缀（next 数组）跳过无效比较。

时间复杂度：O(n + m)  n=文本长度, m=模式长度
空间复杂度：O(m)

=== next 数组 ===

next[i] = pattern[0..i] 的最长公共真前后缀长度。

以 "ababac" 为例：
  子串      最长公共真前后缀   next[i]
  a         无                0
  ab        无                0
  aba       a = a             1
  abab      ab = ab           2
  ababa     aba = aba         3
  ababac    无                0

为什么有用？假设匹配到 "ababa" 后失配：
  text:    ...ababaXXXX...
  pattern:    ababac
                   ^ 失配，已匹配 "ababa"，next[4]=3
  已匹配后缀 "aba" 同时是模式串前缀，直接跳到 j=3 继续比较：
  text:    ...ababaXXXX...
  pattern:      ababac
                ^^^已知匹配

=== 构建 next ===

本质是模式串对自身做匹配，双指针 i 和 j：
  pattern: a b a b a c
  i=1: 'b' vs 'a' → 不等, j=0          → next[1]=0
  i=2: 'a' vs 'a' → 相等, j=1          → next[2]=1
  i=3: 'b' vs 'b' → 相等, j=2          → next[3]=2
  i=4: 'a' vs 'a' → 相等, j=3          → next[4]=3
  i=5: 'c' vs 'b' → 不等, 回退 j=next[2]=1
       'c' vs 'b' → 不等, 回退 j=next[0]=0
       'c' vs 'a' → 不等, j=0          → next[5]=0
  回退的本质：当前前后缀匹配不上，退而求其次尝试次长的公共前后缀。

=== 匹配过程 ===

和构建 next 逻辑一致，text 的 i 指针永远不回退，只有 j 在跳。
为什么 O(n+m)：j 每次回退消耗之前 j++ 攒的值，总回退 <= 总前进。
*/

class KMP {
    std::string pattern;
    std::vector<int> next;

    void build() {
        int m = pattern.size();
        next.assign(m, 0);
        for (int i = 1, j = 0; i < m; i++) {
            while (j > 0 && pattern[i] != pattern[j])
                j = next[j - 1];
            if (pattern[i] == pattern[j]) j++;
            next[i] = j;
        }
    }

public:
    KMP(const std::string& p) : pattern(p) { build(); }

    // 返回所有匹配位置
    std::vector<int> search(const std::string& text) const {
        std::vector<int> res;
        int n = text.size(), m = pattern.size();
        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && text[i] != pattern[j])
                j = next[j - 1];
            if (text[i] == pattern[j]) j++;
            if (j == m) {
                res.push_back(i - m + 1);
                j = next[j - 1];
            }
        }
        return res;
    }

    // 返回第一个匹配位置，未找到返回 -1
    int find(const std::string& text) const {
        int n = text.size(), m = pattern.size();
        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && text[i] != pattern[j])
                j = next[j - 1];
            if (text[i] == pattern[j]) j++;
            if (j == m) return i - m + 1;
        }
        return -1;
    }

    const std::vector<int>& get_next() const { return next; }
};

int main() {
    // search: 查找所有匹配位置
    KMP kmp("ab");
    auto pos = kmp.search("ababab");
    std::cout << "search: ";
    for (int p : pos) std::cout << p << ' ';
    std::cout << '\n';  // 输出: 0 2 4

    // find: 查找第一个匹配位置
    KMP kmp2("abc");
    std::cout << "find: " << kmp2.find("xyzabcdef") << '\n';  // 输出: 3
    std::cout << "find: " << kmp2.find("xyz") << '\n';         // 输出: -1

    // next 数组
    KMP kmp3("ababac");
    std::cout << "next: ";
    for (int v : kmp3.get_next()) std::cout << v << ' ';
    std::cout << '\n';  // 输出: 0 0 1 2 3 0

    return 0;
}

/*
=== STL string::find 用法 ===

#include <string>

std::string text = "ababab";
std::string pattern = "ab";

// 查找第一个匹配
size_t pos = text.find(pattern);          // 0
size_t pos2 = text.find(pattern, 2);      // 从下标2开始查找，返回2
// 未找到返回 std::string::npos

// 查找所有匹配
std::vector<int> positions;
size_t p = text.find(pattern);
while (p != std::string::npos) {
    positions.push_back(p);
    p = text.find(pattern, p + 1);
}

// 注意：string::find 最坏 O(n*m)，不如 KMP
*/
