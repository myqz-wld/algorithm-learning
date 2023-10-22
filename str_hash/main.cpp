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
*/
constexpr int M = 1e9 + 7;
constexpr int A = 31;
std::vector<int> str_hash(std::string str) {
    int n = str.length();
    std::vector<int> result(n + 1, 0);
    long long hash = 0;
    long long base = 1;
    for (int i = 0; i < n; i++) {
        hash += str[i] * base;
        hash %= M;
        base *= A;
        base %= M;
        result[i + 1] = hash;
    }
    return result;
}

// 用字符串哈希做字符串匹配
std::vector<int> match(std::string str, std::string target) {
    // 搜索字符串的哈希值列表
    std::vector<int> hashs = str_hash(str);
    // 目标字符串哈希值
    int hash = str_hash(target).back();
    std::vector<int> result;
    int n = str.length(), m = target.length();
    long long base = 1;
    for (int i = 0; i < n; i++) {
        // 子字符串的哈希值可以通过计算得出
        if ((hashs[i + m] - hashs[i] + M) % M == (hash * base) % M) {
            result.push_back(i);
        }
        base *= A;
        base %= M;
    }
    return result;
}

int main(int argc, char const *argv[]) {
    std::string str, target;
    std::cin >> str >> target;
    std::vector<int> indexs = match(str, target);
    for (int index : indexs) {
        std::cout << index << ' ';
    }
    std::cout << '\n';
    return 0;
}
