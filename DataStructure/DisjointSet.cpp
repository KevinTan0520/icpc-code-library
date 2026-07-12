#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    int size;
    vector<int> fa;
    void init(int n) {
        size = n;
        fa.assign(size + 1, 0);
        for (int i = 1; i <= n; i++) fa[i] = i;
    }
    int find(int x) {
        if (fa[x] == x) return x;
        return fa[x] = find(fa[x]);
    }
    void combine(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        fa[x] = y;
        return;
    }
};

int main() {
    return 0;
}