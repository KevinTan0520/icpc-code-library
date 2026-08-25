//树的中心：作为根节点时出发的最长链最短
/* 性质
树的中心不一定唯一，但最多有 2 个，且这两个中心是相邻的．
树的中心一定位于树的直径上．
树上所有点到其最远点的路径一定交会于树的中心．
当树的中心为根节点时，其到达直径端点的两条链分别为最长链和次长链．
当通过在两棵树间连一条边以合并为一棵树时，连接两棵树的中心可以使新树的直径最小．
树的中心到其他任意节点的距离不超过树直径的一半．
*/

#include <bits/stdc++.h>
using namespace std;

struct Edge { int u, v, w; };

void dfsd(const vector<vector<Edge>> &e, vector<int> &d1, vector<int> &d2, int cur, int fa) {
    for (Edge nxtn : e[cur]) {
        int nxt = nxtn.v, w = nxtn.w;
        if (nxt == fa) continue;
        dfsd(e, d1, d2, nxt, cur);
        if (d1[nxt] + w >= d1[cur]) {
            d2[cur] = d1[cur];
            d1[cur] = d1[nxt] + w;
        }
        else if (d1[nxt] + w >= d2[cur]) {
            d2[cur] = d1[nxt] + w; 
        }
    }
}

void dfsu(const vector<vector<Edge>> &e, const vector<int> &d1, const vector<int> &d2, vector<int> &up, int cur, int fa) {
    for (Edge nxtn : e[cur]) {
        int nxt = nxtn.v, w = nxtn.w;
        if (nxt == fa) continue;
        up[nxt] = up[cur] + w;
        if (d1[nxt] + w != d1[cur]) up[nxt] = max(up[nxt], d1[cur] + w);
        else up[nxt] = max(up[nxt], d2[cur] + w);
        dfsu(e, d1, d2, up, nxt, cur);
    }
}

int get_center(const vector<vector<Edge>> &e, int n, pair<int, int> &cent) {
    vector<int> d1(n + 1, 0), d2(n + 1, 0), up(n + 1, 0);
    dfsd(e, d1, d2, 1, 0);
    dfsu(e, d1, d2, up, 1, 0);
    int minn = 1e9;
    for (int i = 1; i <= n; i++) {
        if (max(d1[i], up[i]) < minn) {
            minn = max(d1[i], up[i]);
            cent.first = i;
            cent.second = 0;
        }
        else if (max(d1[i], up[i]) == minn) {
            cent.second = i;
        }
    }
    return minn;
}

int main() {
    return 0;
}