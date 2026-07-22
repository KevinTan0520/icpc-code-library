#include <bits/stdc++.h>
using namespace std;

// 欧拉函数 phi(x) 表示 1..x 中与 x 互质的整数个数，并规定 phi(1) = 1。
//
// OI 中的常见应用：
// 1. 用欧拉定理降幂：gcd(a, m) = 1 时，a^phi(m) ≡ 1 (mod m)。
// 2. 统计互质数对、既约分数，以及计算与 gcd 有关的和。
//
// 线性筛转移：
// - p | i 时，i * p 只是在已有质因子 p 上增加一次指数，
//   phi(i * p) = phi(i) * p。
// - p 不整除 i 时，p 是新质因子，phi(i * p) = phi(i) * (p - 1)。
// 每个合数只会被它的最小质因子筛到一次，因此总复杂度为 O(n)。
void get_phi(vector<int> &phi, int n) {
    vector<int> primes;
    vector<bool> is_composite(n + 1, false);
    phi.assign(n + 1, 0);
    if (n >= 1) phi[1] = 1;

    for (int i = 2; i <= n; i++) {
        if (!is_composite[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int p : primes) {
            if (p > n / i) break;
            is_composite[i * p] = true;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                break;
            }
            phi[i * p] = phi[i] * (p - 1);
        }
    }
}

int main() {
    return 0;
}
