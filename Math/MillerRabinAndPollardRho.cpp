#include <bits/stdc++.h>
using namespace std;

long long quick_pow(long long a, long long x, long long mod) {
    if (x == 1) return a % mod;
    long long res = quick_pow(a, x / 2, mod);
    if (x % 2) return res * res % mod * a % mod;
    else return res * res % mod;
}

bool miller_rabin(long long n)  { // return 1 if prime, 0 otherwise
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

long long f(long long t, long long c, long long x) {
    return ((__int128) t * t + c) % x;
}

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

long long pollard_rho(long long x) { // return x if failed, else return a factor of x (not 1 or x)
    long long t = 0, c = rand() % (x - 1) + 1, s = t, val = 1, step = 0, goal = 1;
    for (goal = 1;; goal <<= 1, s = t, val = 1) {
        for (step = 1; step <= goal; step++) {
            t = f(t, c, x);
            val = val * abs(t - s) % x;
            if (!val) return x;
            if (step % 127 == 0) {
                long long d = gcd(val, x);
                if (d > 1) return d;
            }
        }
        long long d = gcd(val, x);
        if (d > 1) return d;
    }
}

void fac(long long x, long long &ans) { // update ans to x's greatest prime factor
    if (x <= ans || x < 2) return;
    if (miller_rabin(x)) {
        ans = max(x, ans);
    }
    else {
        long long p = x;
        while (p >= x) p = pollard_rho(x);
        while (x % p == 0) x /= p;
        fac(x, ans), fac(p, ans);
    }
}

int main() {
    return 0;
}