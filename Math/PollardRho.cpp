#include <bits/stdc++.h>
using namespace std;

bool miller_rabin(long long x) {return 0; } // find in other file

long long f(long long t, long long c, long long x) {
    return ((__int128) t * t + c) % x;
}

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

long long pollard_rho(long long x) {
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

void fac(long long x, long long &ans) {
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