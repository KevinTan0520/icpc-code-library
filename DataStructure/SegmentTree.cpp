#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    int size;
    vector<long long> tr, add;
    void init(int n) {
        size = n;
        tr.assign(4 * size + 1, 0);
        add.assign(4 * size + 1, 0);
    }
    void pushup(int id) {
        tr[id] = tr[id * 2] + tr[id * 2 + 1];
        return;
    }
    void pushdown(int l, int r, int id) {
        if (add[id] == 0) return;
        int mid = (l + r) / 2;
        add[id * 2] += add[id];
        add[id * 2 + 1] += add[id];
        tr[id * 2] += add[id] * (mid - l + 1);
        tr[id * 2 + 1] += add[id] * (r - mid);
        add[id] = 0;
        return;
    }
    void build(const vector<long long> &a, int l, int r, int id) {
        if (l == r) {
            tr[id] = a[l];
            return;
        }
        int mid = (l + r) / 2;
        build(a, l, mid, id * 2);
        build(a, mid + 1, r, id * 2 + 1);
        pushup(id);
        return;
    }
    long long range_query(int ql, int qr, int l, int r, int id) {
        if (ql <= l && qr >= r) {
            return tr[id];
        }
        int mid = (l + r) / 2;
        long long res = 0;
        pushdown(l, r, id);
        if (ql <= mid) {
            res += range_query(ql, qr, l, mid, id * 2);
        }
        if (qr >= mid + 1) {
            res += range_query(ql, qr, mid + 1, r, id * 2 + 1);
        }
        return res;
    }
    void range_add(int ql, int qr, long long qv, int l, int r, int id) {
        if (ql <= l && qr >= r) {
            add[id] += qv;
            tr[id] += qv * (r - l + 1);
            return;
        }
        int mid = (l + r) / 2;
        pushdown(l, r, id);
        if (ql <= mid) {
            range_add(ql, qr, qv, l, mid, id * 2);
        }
        if (qr >= mid + 1) {
            range_add(ql, qr, qv, mid + 1, r, id * 2 + 1);
        }
        pushup(id);
    }
};

int main() {
    return 0;
}