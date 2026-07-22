#include <bits/stdc++.h>
using namespace std;

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

int main() {
    return 0;
}
