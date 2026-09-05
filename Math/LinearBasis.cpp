#include <bits/stdc++.h>
using namespace std;

const int MAX_BIT = 62;

struct LinearBasis {
    int rank;
    vector<long long> p;

    void init() {
        rank = 0;
        p.assign(MAX_BIT + 1, 0);
        return;
    }

    bool insert(long long x) {
        for (int i = MAX_BIT; i >= 0; i--) {
            if ((x >> i & 1ll) == 0) continue;
            if (p[i] == 0) {
                p[i] = x;
                rank++;
                return true;
            }
            x ^= p[i]; 
        }
        return false;
    }

    bool contain(long long x) {
        for (int i = MAX_BIT; i >= 0; i--) {
            if ((x >> i & 1ll) == 0) continue;
            if (p[i] == 0) {
                return false;
            }
            x ^= p[i];
        }
        return true;
    }

    // 返回 x 与线性基中若干向量异或后能得到的最大值
    // x = 0 时，就是原集合的最大子集异或和
    long long get_max(long long x = 0) {
        for (int i = MAX_BIT; i >= 0; i--) {
            if ((x ^ p[i]) > x) x ^= p[i];
        }
        return x;
    }

    void merge(const LinearBasis &other) {
        for (int i = MAX_BIT; i >= 0; i--) {
            if (other.p[i] != 0) {
                insert(other.p[i]);
            }
        }
        return;
    }
};

int main() {
    return 0;
}