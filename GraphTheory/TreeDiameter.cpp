//树的直径：树上最长简单路径
#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v, w; };

//BFS，只能处理非负权树
int get_diameter_bfs(const vector<vector<Edge>> &e, int n, pair<int, int> &dia) {
    auto bfs = [&](int src) -> pair<int, int> { // 返回 {最远点, 距离}
        vector<int> dis(n + 1, -1);
        queue<int> q;
        dis[src] = 0;
        q.push(src);
        int far = src, maxd = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dis[u] > maxd) { maxd = dis[u]; far = u; }
            for (const auto& e : e[u]) {
                if (dis[e.v] == -1) {
                    dis[e.v] = dis[u] + e.w;
                    q.push(e.v);
                }
            }
        }
        return {far, maxd};
    };
    auto [a, _] = bfs(1);
    auto [b, len] = bfs(a);
    dia = {a, b};
    return len;
}

//树形DP
int get_diameter_dp(const vector<vector<Edge>> &e, int n, pair<int, int> &dia) {
    vector<int> fa(n + 1, -1), order;
    order.reserve(n);
    stack<int> st;
    st.push(1);
    fa[1] = 0;
    while (!st.empty()) {
        int u = st.top(); st.pop();
        order.push_back(u);
        for (const auto& edge : e[u]) {
            int v = edge.v;
            if (v == fa[u]) continue;
            fa[v] = u;
            st.push(v);
        }
    }
    vector<int> dp(n + 1, 0), down_node(n + 1);
    for (int i = 1; i <= n; i++) down_node[i] = i;
    int len = 0;
    dia = {1, 1};
    for (int idx = n - 1; idx >= 0; idx--) {
        int u = order[idx];
        int max1 = 0, max2 = 0, node1 = u, node2 = u;
        for (const auto& edge : e[u]) {
            int v = edge.v;
            if (fa[v] != u) continue;
            int res = dp[v] + edge.w;
            if (res > max1) {
                max2 = max1;
                node2 = node1;
                max1 = res;
                node1 = down_node[v];
            }
            else if (res > max2) {
                max2 = res;
                node2 = down_node[v];
            }
        }
        dp[u] = max1;
        down_node[u] = node1;
        if (max1 + max2 > len) {
            len = max1 + max2;
            dia = {node1, node2};
        }
    }
    return len;
}

int main() {
    return 0;
}