#include <iostream>
#include <string>
#include <vector>

/*
字符串哈希 (String Hash)
将字符串映射为整数，支持 O(1) 比较任意子串是否相等。

=== 核心思想 ===

多项式哈希函数：
  hash(s) = (s[0]*A^0 + s[1]*A^1 + s[2]*A^2 + ...) % M

前缀哈希数组 h[]：
  h[0] = 0
  h[i] = hash(s[0..i))  即前 i 个字符的哈希值

提取子串 s[l..r) 的哈希值：
  raw_hash = h[r] - h[l]                     // 差值包含 A^l 因子
  normalized = raw_hash * A^(-l) % M          // 除掉 A^l，归一化到位置 0
  这样不同位置的相同子串得到相同哈希值，可以直接比较。

A^(-1) 通过费马小定理求逆元：A^(-1) = A^(M-2) % M

=== 碰撞 ===

单哈希有碰撞概率（约 1/M），可用双哈希降低（两个不同 M 都匹配才认为相等）。
为防 Hack，可随机化 A 和 M。

时间复杂度：预处理 O(n)，单次子串比较 O(1)
空间复杂度：O(n)
*/

class StrHash {
    static constexpr int N = 1e6 + 5;
    static constexpr long long M = 1e9 + 7;
    static constexpr long long A = 31;
    static std::vector<long long> _inv_pw;

public:
    static long long qpow(long long a, long long b) {
        long long res = 1;
        while (b) {
            if (b & 1) res = res * a % M;
            a = a * a % M;
            b >>= 1;
        }
        return res;
    }

    static void init() {
        if (_inv_pw.size() > 1) return;
        _inv_pw.resize(N);
        _inv_pw[0] = 1;
        long long inv_a = qpow(A, M - 2);
        for (int i = 1; i < N; i++)
            _inv_pw[i] = _inv_pw[i - 1] * inv_a % M;
    }

    static std::vector<long long> build(const std::string& str) {
        int n = str.length();
        std::vector<long long> h(n + 1, 0);
        long long hash = 0, base = 1;
        for (int i = 0; i < n; i++) {
            hash = (hash + str[i] * base) % M;
            base = base * A % M;
            h[i + 1] = hash;
        }
        return h;
    }

    // 获取子串 [l, r) 的哈希值（归一化到位置 0）
    static long long get(const std::vector<long long>& h, int l, int r) {
        return (h[r] - h[l] + M) % M * _inv_pw[l] % M;
    }

    // 比较两个子串 [l1, r1) 和 [l2, r2) 是否相等
    static bool equal(const std::vector<long long>& h, int l1, int r1, int l2, int r2) {
        if (r1 - l1 != r2 - l2) return false;
        return get(h, l1, r1) == get(h, l2, r2);
    }

    // 在 text 中查找 pattern 的所有出现位置
    static std::vector<int> search(const std::vector<long long>& text_h,
                                   const std::vector<long long>& pat_h) {
        std::vector<int> res;
        int n = text_h.size() - 1, m = pat_h.size() - 1;
        long long pat_hash = get(pat_h, 0, m);
        for (int i = 0; i + m <= n; i++)
            if (get(text_h, i, i + m) == pat_hash)
                res.push_back(i);
        return res;
    }
};
std::vector<long long> StrHash::_inv_pw;

int main() {
    StrHash::init();

    // 子串比较
    auto h = StrHash::build("abcabc");
    std::cout << "equal [0,3) [3,6): " << StrHash::equal(h, 0, 3, 3, 6) << '\n';  // 输出: 1
    std::cout << "equal [0,3) [1,4): " << StrHash::equal(h, 0, 3, 1, 4) << '\n';  // 输出: 0

    // 字符串匹配
    auto th = StrHash::build("ababab");
    auto ph = StrHash::build("ab");
    auto pos = StrHash::search(th, ph);
    std::cout << "search: ";
    for (int p : pos) std::cout << p << ' ';
    std::cout << '\n';  // 输出: 0 2 4

    return 0;
}
