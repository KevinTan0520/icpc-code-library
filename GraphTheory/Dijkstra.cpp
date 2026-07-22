#include <bits/stdc++.h>
using namespace std;

struct Edge {int u, v, w; };
struct Node {
    int id, dis;
    bool operator< (const Node &a) const {
        return dis > a.dis;
    }
};

/*
void print_path(int st, int ed) {
    if (st == ed) {
        cout << st << ' ';
        return;
    }
    print_path(st, pth[ed]);
    cout << ed << ' ';
    return;
}
*/

void dijkstra(const vector<vector<Edge>> &e, vector<int> &dis, int n, int st) { // add vector<int> &pth
    vector<bool> done(n + 1, 0);
    dis.assign(n + 1, INT_MAX);
    dis[st] = 0;
    priority_queue<Node> q;
    q.push({st, dis[st]});
    while (!q.empty()) {
        Node u = q.top();
        q.pop();
        if (done[u.id]) continue;
        done[u.id] = 1;
        for (int i = 0; i < e[u.id].size(); i++) {
            if (done[e[u.id][i].v]) continue;
            if (dis[e[u.id][i].v] > e[u.id][i].w + u.dis) {
                dis[e[u.id][i].v] = e[u.id][i].w + u.dis;
                q.push({e[u.id][i].v, dis[e[u.id][i].v]});
                // pth[e[u.id][i].v] = u.id;
            }
        }
    }
}

int main() {
    return 0;
}