#include <bits/stdc++.h>
using namespace std;

long long quick_pow(long long a, long long x, long long mod) {
    if (x == 1) return a % mod;
    long long res = quick_pow(a, x / 2, mod);
    if (x % 2) return res * res % mod * a % mod;
    else return res * res % mod;
}

bool miller_rabin(long long n)  {
    if (n < 3 || n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    long long u = n - 1, t = 0;
    int test_time = 8;
    while (u % 2 == 0) u /= 2, ++t;
    for (int i = 0; i < test_time; i++) {
        long long a = rand() % (n - 3) + 2, v = quick_pow(a, u, n);
        if (v == 1) continue;
        long long s;
        for (s = 0; s < t; s++) {
            if (v == n - 1) break;
            v = (long long) v * v % n;
        }
        if (s == t) return 0;
    }
    return 1;
}

int main() {
    return 0;
}