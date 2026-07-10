#include <bits/stdc++.h>
using namespace std;

int lowbit(int x) {
    return x & -x;
}

struct BinaryIndexedTree {
    int size;
    vector<int> tr;
    void init(int n) {
        size = n;
        tr.assign(n + 1, 0);
        return;
    }
    void point_add(int x, int v) {
        while (x <= size) {
            tr[x] += v;
            x += lowbit(x);
        }
        return;
    }
    void build(const vector<int> &a) {
        for (int i = 1; i <= size; i++) {
            point_add(i, a[i]);
        }
        return;
    }
    int prefix_query(int r) {
        int res = 0;
        while (r) {
            res += tr[r];
            r -= lowbit(r);
        }
        return res;
    }
    int range_query(int l, int r) {
        return prefix_query(r) - prefix_query(l - 1);
    }
};

int main() {
    return 0;
}