#include <bits/stdc++.h>
using namespace std;

// 有向图强连通分量及缩点。
// 用法：init(n) -> add_edge(u, v) -> find_components()，之后可建缩点图或求最大点权路径。
struct StronglyConnectedComponents {
    int n, timer, component_count;
    vector<vector<int>> adj;
    vector<int> dfn, low, component_id, stk;   // component_id[u]：点 u 所属 SCC
    vector<bool> in_stack;
    vector<vector<int>> components;

    void init(int n_) {
        n = n_;
        adj.assign(n + 1, {});
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void tarjan(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in_stack[u] = true;
        for (int v : adj[u]) {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (dfn[u] != low[u]) return;
        // u 是当前 SCC 的根，将栈顶一直弹到 u。
        component_count++;
        components.push_back({});
        while (true) {
            int v = stk.back();
            stk.pop_back();
            in_stack[v] = false;
            component_id[v] = component_count;
            components.back().push_back(v);
            if (v == u) break;
        }
    }

    // 返回所有 SCC；components 与 component_id 的编号均从 1 开始。
    vector<vector<int>> find_components() {
        timer = component_count = 0;
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        component_id.assign(n + 1, 0);
        in_stack.assign(n + 1, false);
        stk.clear();
        components.assign(1, {});
        for (int u = 1; u <= n; u++) {
            if (!dfn[u]) tarjan(u);
        }
        // Tarjan 先编号汇点分量，因此按编号从大到小即为一种拓扑序。
        return components;
    }

    // 构造缩点 DAG，点编号沿用 component_id，并去除重边。
    vector<vector<int>> condensation_graph() const {
        vector<vector<int>> dag(component_count + 1);
        for (int u = 1; u <= n; u++) {
            for (int v : adj[u]) {
                if (component_id[u] != component_id[v]) {
                    dag[component_id[u]].push_back(component_id[v]);
                }
            }
        }
        for (int id = 1; id <= component_count; id++) {
            sort(dag[id].begin(), dag[id].end());
            dag[id].erase(unique(dag[id].begin(), dag[id].end()), dag[id].end());
        }
        return dag;
    }

    // vertex_weight 从 1 开始；返回缩点 DAG 上的最大点权路径和。
    // 必须先调用 find_components()，同一 SCC 内的点权会自动合并。
    long long maximum_weight_path(const vector<long long> &vertex_weight) const {
        assert(static_cast<int>(vertex_weight.size()) == n + 1);
        vector<long long> component_weight(component_count + 1, 0);
        for (int u = 1; u <= n; u++) {
            component_weight[component_id[u]] += vertex_weight[u];
        }

        vector<vector<int>> dag = condensation_graph();
        vector<long long> dp = component_weight;
        long long result = LLONG_MIN;
        for (int id = component_count; id >= 1; id--) {
            result = max(result, dp[id]);
            for (int v : dag[id]) {
                dp[v] = max(dp[v], dp[id] + component_weight[v]);
            }
        }
        return component_count == 0 ? 0 : result;
    }
};

int main() {
    return 0;
}
