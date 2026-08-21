#include <bits/stdc++.h>
using namespace std;

struct Edge {int u, v, w; };

bool bellman_ford(const vector<vector<Edge>> &e, vector<int> &dis, int n, int st) {
    dis.assign(n + 1, INT_MAX);
    dis[st] = 0;
    for (int i = 1; i <= n; i++) {
        bool flag = false;
        for (int j = 1; j <= n; j++) {
            for (int k = 0; k < e[j].size(); k++) {
                if (dis[j] != INT_MAX && dis[j] + e[j][k].w < dis[e[j][k].v]) {
                    dis[e[j][k].v] = dis[j] + e[j][k].w;
                    flag = true;
                }
            }
        }
        if (!flag) break;
    }
    //negative cycle
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < e[i].size(); j++) {
            if (dis[i] != INT_MAX && dis[i] + e[i][j].w < dis[e[i][j].v]) {
                return 1; //find negative cycle
            }
        }
    }
    return 0;
}

int main() {
    return 0;
}

/*
解决差分约束问题：
把每个变量 x_i 视为节点，对于约束 x_i - x_j <= c_k ，从 j 向 i 连长度为 c_k 的有向边
dis_0 = 0 ，0 向每个节点连权重为 0 的边 
有负环则无解否则 dis 为一组解
*/