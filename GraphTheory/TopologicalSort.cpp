#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 10;

struct Edge {int u, v; };
vector<Edge> e[MAXN];

vector<int> topological_sort(int n) {
    queue<int> q;
    vector<int> deg(n + 1, 0), ans;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < e[i].size(); j++) {
            deg[e[i][j].v]++;
        }
    }
    for (int i = 1; i <= n; i++) {
        if (deg[i] == 0) q.push(i);
    }
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        ans.push_back(cur);
        for (int i = 0; i < e[cur].size(); i++) {
            deg[e[cur][i].v]--;
            if (deg[e[cur][i].v] == 0) q.push(e[cur][i].v);
        }
    }
    return ans;
}

int main() {
    return 0;
}