#include <bits/stdc++.h>
using namespace std;

// 莫比乌斯函数 mu(x) 的定义：
// - mu(1) = 1；
// - x 含有某个质因子的平方时，mu(x) = 0；
// - 否则，若 x 是 k 个不同质数的乘积，则 mu(x) = (-1)^k。
//
// OI 中的常见应用是莫比乌斯反演和容斥。例如把“gcd 恰好为 1”的计数
// 转化为所有公因数倍数的计数；mu(x) 是否为 0 也可用于判断平方自由数。
//
// 线性筛转移：若 p | i，则 i * p 含有 p^2，mu(i * p) = 0；
// 否则加入了一个新的质因子，mu(i * p) = -mu(i)。总复杂度为 O(n)。
void get_mu(vector<int> &mu, int n) {
    vector<int> primes;
    vector<bool> is_composite(n + 1, false);
    mu.assign(n + 1, 0);
    if (n >= 1) mu[1] = 1;

    for (int i = 2; i <= n; i++) {
        if (!is_composite[i]) {
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            if (p > n / i) break;
            is_composite[i * p] = true;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            }
            mu[i * p] = -mu[i];
        }
    }
}

int main() {
    return 0;
}
