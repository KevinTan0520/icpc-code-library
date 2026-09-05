#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
const int MOD = 998244353;
const int PRIMITIVE_ROOT = 3;

int quick_pow(int x, int exp) {
    int res = 1;
    while (exp > 0) {
        if (exp & 1) res = static_cast<i64>(res) * x % MOD;
        x = static_cast<i64>(x) * x % MOD;
        exp >>= 1;
    }
    return res;
}

void ntt(vector<int> &a, int len, bool inverse) {
    assert(len > 0 && (len & (len - 1)) == 0 && (MOD - 1) % len == 0);
    assert(static_cast<int>(a.size()) > len);

    for (int i = 1, j = 0; i < len; i++) {
        int bit = len >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) swap(a[i + 1], a[j + 1]);
    }

    for (int block = 2; block <= len; block <<= 1) {
        int root = quick_pow(PRIMITIVE_ROOT, (MOD - 1) / block);
        if (inverse) root = quick_pow(root, MOD - 2);
        for (int i = 0; i < len; i += block) {
            int w = 1;
            for (int j = 0; j < block / 2; j++) {
                int u = a[i + j + 1];
                int v = static_cast<i64>(a[i + j + block / 2 + 1]) * w % MOD;
                a[i + j + 1] = u + v;
                if (a[i + j + 1] >= MOD) a[i + j + 1] -= MOD;
                a[i + j + block / 2 + 1] = u - v;
                if (a[i + j + block / 2 + 1] < 0) a[i + j + block / 2 + 1] += MOD;
                w = static_cast<i64>(w) * root % MOD;
            }
        }
    }

    if (inverse) {
        int inv_len = quick_pow(len, MOD - 2);
        for (int i = 1; i <= len; i++) a[i] = static_cast<i64>(a[i]) * inv_len % MOD;
    }
}

// a[1..len_a] 和 b[1..len_b] 为有效系数，返回值也从下标 1 开始。
// MOD = 998244353 时变换长度至多为 2^23。
vector<int> convolution(const vector<int> &a, int len_a,
                        const vector<int> &b, int len_b) {
    assert(len_a >= 0 && len_b >= 0);
    assert(static_cast<int>(a.size()) > len_a && static_cast<int>(b.size()) > len_b);
    if (len_a == 0 || len_b == 0) return vector<int>(1);
    int result_size = len_a + len_b - 1;
    int transform_len = 1;
    while (transform_len < result_size) transform_len <<= 1;
    assert((MOD - 1) % transform_len == 0);

    vector<int> fa(transform_len + 1), fb(transform_len + 1);
    for (int i = 1; i <= len_a; i++) {
        fa[i] = a[i] % MOD;
        if (fa[i] < 0) fa[i] += MOD;
    }
    for (int i = 1; i <= len_b; i++) {
        fb[i] = b[i] % MOD;
        if (fb[i] < 0) fb[i] += MOD;
    }

    ntt(fa, transform_len, false);
    ntt(fb, transform_len, false);
    for (int i = 1; i <= transform_len; i++) {
        fa[i] = static_cast<i64>(fa[i]) * fb[i] % MOD;
    }
    ntt(fa, transform_len, true);
    fa.resize(result_size + 1);
    return fa;
}

int main() {
    return 0;
}
