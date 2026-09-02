#include <bits/stdc++.h>
using namespace std;

void get_prime(vector<int> &primes, vector<bool> &is_prime, int n) {
    primes.clear();
    is_prime.assign(n + 1, 1);
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) primes.push_back(i);
        for (int j = 0; j < primes.size() && primes[j] * i <= n; j++) {
            is_prime[primes[j] * i] = 0;
            if (i % primes[j] == 0) break;
        }
    }
    return;
}

// divisor_count(x)，也常记作 d(x) 或 tau(x)，表示 x 的正约数个数。
// 若 x = p1^a1 * p2^a2 * ... * pk^ak，则
// divisor_count(x) = (a1 + 1) * (a2 + 1) * ... * (ak + 1)。
//
// OI 中可用于批量统计约数、计算约数和式，以及处理“乘积/整除关系有多少种
// 选择”等计数问题。需要对 1..n 全部求值时，线性筛比逐个试除更高效。
//
// min_prime_exp[i] 记录 i 的最小质因子在 i 中的指数 a：
// - p | i 时，指数从 a 增至 a + 1，把约数个数中的因子 (a + 1)
//   替换为 (a + 2)。
// - p 不整除 i 时，加入指数为 1 的新质因子，约数个数乘 2。
// 每个合数只被它的最小质因子筛到一次，总复杂度为 O(n)。
void get_divisor_count(vector<int> &divisor_count, int n) {
    vector<int> primes;
    vector<int> min_prime_exp(n + 1, 0);
    vector<bool> is_composite(n + 1, false);
    divisor_count.assign(n + 1, 0);
    if (n >= 1) divisor_count[1] = 1;

    for (int i = 2; i <= n; i++) {
        if (!is_composite[i]) {
            primes.push_back(i);
            min_prime_exp[i] = 1;
            divisor_count[i] = 2;
        }
        for (int p : primes) {
            if (p > n / i) break;
            is_composite[i * p] = true;
            if (i % p == 0) {
                min_prime_exp[i * p] = min_prime_exp[i] + 1;
                divisor_count[i * p] = divisor_count[i]
                    / (min_prime_exp[i] + 1) * (min_prime_exp[i * p] + 1);
                break;
            }
            min_prime_exp[i * p] = 1;
            divisor_count[i * p] = divisor_count[i] * 2;
        }
    }
}

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