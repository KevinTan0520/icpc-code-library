#include <bits/stdc++.h>
using namespace std;

using Complex = complex<double>;
const double PI = acos(-1.0);

void fft(vector<Complex> &a, int len, bool inverse) {
    assert(len > 0 && (len & (len - 1)) == 0);
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
        double angle = 2 * PI / block * (inverse ? -1 : 1);
        Complex root(cos(angle), sin(angle));
        for (int i = 0; i < len; i += block) {
            Complex w(1, 0);
            for (int j = 0; j < block / 2; j++) {
                Complex u = a[i + j + 1];
                Complex v = a[i + j + block / 2 + 1] * w;
                a[i + j + 1] = u + v;
                a[i + j + block / 2 + 1] = u - v;
                w *= root;
            }
        }
    }

    if (inverse) {
        for (int i = 1; i <= len; i++) a[i] /= len;
    }
}

// a[1..len_a] 和 b[1..len_b] 为有效系数，返回值也从下标 1 开始。
// 系数过大时应改用多模数 NTT，避免浮点误差。
vector<long long> convolution(const vector<long long> &a, int len_a,
                              const vector<long long> &b, int len_b) {
    assert(len_a >= 0 && len_b >= 0);
    assert(static_cast<int>(a.size()) > len_a && static_cast<int>(b.size()) > len_b);
    if (len_a == 0 || len_b == 0) return vector<long long>(1);
    int result_size = len_a + len_b - 1;
    int transform_len = 1;
    while (transform_len < result_size) transform_len <<= 1;

    vector<Complex> fa(transform_len + 1), fb(transform_len + 1);
    for (int i = 1; i <= len_a; i++) fa[i] = a[i];
    for (int i = 1; i <= len_b; i++) fb[i] = b[i];

    fft(fa, transform_len, false);
    fft(fb, transform_len, false);
    for (int i = 1; i <= transform_len; i++) fa[i] *= fb[i];
    fft(fa, transform_len, true);

    vector<long long> res(result_size + 1);
    for (int i = 1; i <= result_size; i++) res[i] = llround(fa[i].real());
    return res;
}

int main() {
    return 0;
}
