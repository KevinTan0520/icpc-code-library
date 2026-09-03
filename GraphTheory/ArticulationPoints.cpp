#include <bits/stdc++.h>
using namespace std;

// 无向图割点：删除该点及其关联边后，原连通块数量增加。
// 用法：init(n) -> add_edge(u, v) -> find_points()。
struct ArticulationPoints {
    int n, timer, edge_count;
    vector<vector<pair<int, int>>> adj;
    vector<int> dfn, low;
    vector<bool> is_cut;    // is_cut[u] 表示 u 是否为割点

    void init(int n_) {
        n = n_;
        edge_count = 0;
        adj.assign(n + 1, {});
    }

    void add_edge(int u, int v) {
        adj[u].push_back({v, edge_count});
        adj[v].push_back({u, edge_count});
        edge_count++;
    }

    void tarjan(int u, int parent_edge) {
        dfn[u] = low[u] = ++timer;
        int child_count = 0;
        for (const auto &item : adj[u]) {
            int v = item.first, id = item.second;
            if (id == parent_edge) continue;
            if (!dfn[v]) {
                child_count++;
                tarjan(v, id);
                low[u] = min(low[u], low[v]);
                // v 的子树不能绕过 u 到达 u 的祖先，则非根节点 u 是割点。
                if (parent_edge != -1 && low[v] >= dfn[u]) {
                    is_cut[u] = true;
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        // DFS 根有至少两个搜索树儿子时才是割点。
        if (parent_edge == -1 && child_count > 1) is_cut[u] = true;
    }

    // 返回按点编号升序排列的全部割点，同时可直接查询 is_cut。
    vector<int> find_points() {
        timer = 0;
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        is_cut.assign(n + 1, false);
        for (int u = 1; u <= n; u++) {
            if (!dfn[u]) tarjan(u, -1);
        }

        vector<int> points;
        for (int u = 1; u <= n; u++) {
            if (is_cut[u]) points.push_back(u);
        }
        return points;
    }
};

int main() {
    return 0;
}
