#include <bits/stdc++.h>
using namespace std;

int quick_pow(int x, int a, int mod) {
    if (a == 1) return x;
    int res = quick_pow(x, a / 2, mod) % mod;
    if (a % 2) {
        return res * res % mod * x % mod;
    }
    return res * res % mod;
}

int get_single_inv(int x, int mod) {
    return quick_pow(x, mod - 2, mod);
}

vector<int> get_batch_inv(const vector<int> &a, int mod) {
    int n = a.size(), s = 1;
    vector<int> prod(n), res(n);
    for (int i = 0; i < n; i++) {
        prod[i] = s;
        s = s * a[i] % mod;
    }
    s = get_single_inv(s, mod);
    for (int i = n - 1; i >= 0; i--) {
        res[i] = s * prod[i] % mod;
        s = s * a[i] % mod;
    }
    return res;
}
 
vector<int> get_nint_inv(int n, int mod) {
    vector<int> res(n + 1);
    res[1] = 1;
    for (int i = 2; i <= n; i++) {
        res[i] = (mod - mod / i) * res[mod % i] % mod;
    }
    return res;
}

int main() {
    return 0;
}