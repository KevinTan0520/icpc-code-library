#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v, w; };

class TreeAncestor {
    private: 
    int n, LOG, rt;
    vector<vector<Edge>> adj;
    vector<vector<int>> up;
    vector<int> depth;

    void dfs(int u, int p) {
        up[0][u] = p;
        for (int i = 1; i < LOG; i++) {
            if (up[i-1][u] != -1) 
                up[i][u] = up[i-1][ up[i-1][u] ];
            else 
                up[i][u] = -1;
        }
        for (const auto &edge : adj[u]) {
            int v = (edge.u == u) ? edge.v : edge.u;
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }

    public: 
    TreeAncestor(int n, int rt, const vector<vector<Edge>>& adj) : n(n), rt(rt), adj(adj) {
        LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        up.assign(LOG, vector<int>(n + 1, -1)); 
        depth.assign(n + 1, 0);
        dfs(rt, 0);
    }

    int kth(int u, int k) {
        if (k > depth[u]) return -1;
        for (int i = 0; k > 0; i++) {
            if (k & 1) u = up[i][u];
            k >>= 1;
        }
        return u;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int i = 0; diff > 0; i++) {
            if (diff & 1) u = up[i][u];
            diff >>= 1;
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (up[i][u] != up[i][v]) {
                u = up[i][u];
                v = up[i][v];
            }
        }
        return up[0][u];
    }

    int getDepth(int u) { return depth[u]; }
};

void solve() {
    int n, m, s;
    cin >> n >> m >> s;
    vector<vector<Edge>> e(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        e[u].push_back({u, v, 1});
        e[v].push_back({v, u, 1});
    }
    TreeAncestor ta(n, s, e);
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        cout << ta.lca(u, v) << endl;
    }
    return;
}

int main() {
    solve();
    return 0;
}