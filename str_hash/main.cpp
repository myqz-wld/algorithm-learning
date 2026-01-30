#include <iostream>
#include <string>
#include <vector>

/*
字符串哈希
主要思路是用哈希函数f，来转换字符串
假设有字符串s，有如下转换：
    f(s) -> int
如果f(s1) == f(s2)，我们认为s1字符串和s2字符串相等
一般定义多项式哈希函数:
    f(s) = (s[0] * a^0 + s[1] * a^1 + s[2] * a^2 ……) % m    (a为随机值，m为大质数)
有哈希碰撞的概率（但概率很小），可以用双值哈希来降低碰撞概率
双值哈希的意思是用两个不同的大质数M1、M2，得到两个不同的哈希函数f1、f2，
只有两个哈希函数都匹配上时，才认为字符串相同

为了防止被Hack，可以使用动态的M和A
*/
class string_hash {
    static constexpr int N = 1e6 + 5;
    static constexpr long long M = 1e9 + 7;
    static constexpr long long A = 31;
    static std::vector<long long> _inv_pw;  // 预计算逆元

public:
    // 快速幂
    static long long qpow(long long a, long long b) {
        long long res = 1;
        while (b) {
            if (b & 1) res = res * a % M;
            a = a * a % M;
            b >>= 1;
        }
        return res;
    }

    // 初始化逆元数组（需在使用前调用一次）
    static void init() {
        if (_inv_pw.size() > 1) return;
        _inv_pw.resize(N);
        _inv_pw[0] = 1;
        long long inv_a = qpow(A, M - 2);
        for (int i = 1; i < N; i++) {
            _inv_pw[i] = _inv_pw[i - 1] * inv_a % M;
        }
    }

    // 根据字符串生成哈希数组
    static std::vector<long long> build(const std::string& str) {
        int n = str.length();
        std::vector<long long> hashs(n + 1, 0);
        long long hash = 0, base = 1;
        for (int i = 0; i < n; i++) {
            hash = (hash + str[i] * base) % M;
            base = base * A % M;
            hashs[i + 1] = hash;
        }
        return hashs;
    }

    // 获取子串 [l, r) 的哈希值（标准化到位置0）
    static long long get(const std::vector<long long>& hashs, int l, int r) {
        return (hashs[r] - hashs[l] + M) % M * _inv_pw[l] % M;
    }

    // 比较两个子串 [l1, r1) 和 [l2, r2) 是否相等
    static bool equal(const std::vector<long long>& hashs, int l1, int r1, int l2, int r2) {
        if (r1 - l1 != r2 - l2) return false;
        return get(hashs, l1, r1) == get(hashs, l2, r2);
    }

    // 字符串匹配，返回所有匹配位置
    static std::vector<int> match(const std::vector<long long>& hashs, const std::vector<long long>& target) {
        std::vector<int> result;
        int n = hashs.size() - 1;
        int m = target.size() - 1;
        long long target_hash = target[m];
        for (int i = 0; i + m <= n; i++) {
            if (get(hashs, i, i + m) == target_hash) {
                result.push_back(i);
            }
        }
        return result;
    }
};
std::vector<long long> string_hash::_inv_pw;

int main(int argc, char const *argv[]) {
    string_hash::init();
    std::string str, target;
    std::cin >> str >> target;
    auto hashs = string_hash::build(str);
    auto target_hashs = string_hash::build(target);
    std::vector<int> indexs = string_hash::match(hashs, target_hashs);
    for (int index : indexs) {
        std::cout << index << ' ';
    }
    std::cout << '\n';
    return 0;
}
