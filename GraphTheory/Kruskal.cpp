#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1e5 + 10;

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

struct Edge {int u, v, w; };
vector<Edge> e[MAXN], tot;

bool cmp(Edge a, Edge b) {
    return a.w < b.w;
}

int kruskal(int n, int m) {
    int sum = 0, cnt = 0;
    DisjointSet ds;
    ds.init(n);
    sort(tot.begin() + 1, tot.begin() + m + 1, cmp);
    for (int i = 1; i <= m; i++) {
        if (ds.find(tot[i].u) == ds.find(tot[i].v)) {
            continue;
        }
        sum += tot[i].w;
        ds.combine(tot[i].u, tot[i].v);
        cnt++;
    }
    if (cnt != n - 1) return -1;
    return sum;
}

int main() {
    return 0;
}