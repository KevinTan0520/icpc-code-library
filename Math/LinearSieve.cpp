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

int main() {
    return 0;
}