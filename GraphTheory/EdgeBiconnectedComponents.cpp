#include <bits/stdc++.h>
using namespace std;

// 无向图边双连通分量：删去所有桥后，每个连通块即为一个边双。
// 用法：init(n) -> add_edge(u, v) -> find_components()。
struct EdgeBiconnectedComponents {
    struct Edge { int u, v; };

    int n, timer;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adj;
    vector<int> dfn, low;
    vector<bool> is_bridge;             // 按 add_edge 的顺序标记每条边是否为桥
    vector<vector<int>> components;     // components[编号]：该边双内的所有点
    vector<int> component_id_of;        // component_id_of[u]：点 u 所属边双

    void init(int n_) {
        n = n_;
        adj.assign(n + 1, {});
        edges.clear();
    }

    void add_edge(int u, int v) {
        int id = static_cast<int>(edges.size());
        edges.push_back({u, v});
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
    }

    void tarjan(int u, int parent_edge) {
        dfn[u] = low[u] = ++timer;
        for (const auto &item : adj[u]) {
            int v = item.first, id = item.second;
            if (id == parent_edge) continue;
            if (!dfn[v]) {
                tarjan(v, id);
                low[u] = min(low[u], low[v]);
                // v 的子树无法绕回 u 或 u 的祖先，故 (u,v) 是桥。
                if (low[v] > dfn[u]) is_bridge[id] = true;
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void collect(int u, int component_id) {
        components[component_id].push_back(u);
        for (const auto &item : adj[u]) {
            int v = item.first, id = item.second;
            if (is_bridge[id] || component_id_of[v] != 0) continue;
            component_id_of[v] = component_id;
            collect(v, component_id);
        }
    }

    // 返回所有边双；components 与 component_id_of 的编号均从 1 开始。
    vector<vector<int>> find_components() {
        timer = 0;
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        is_bridge.assign(edges.size(), false);
        for (int u = 1; u <= n; u++) {
            if (!dfn[u]) tarjan(u, -1);
        }

        component_id_of.assign(n + 1, 0);
        components.assign(1, {});
        for (int u = 1; u <= n; u++) {
            if (component_id_of[u] != 0) continue;
            components.push_back({});
            int id = static_cast<int>(components.size()) - 1;
            component_id_of[u] = id;
            collect(u, id);
        }
        return components;
    }
};

int main() {
    return 0;
}
