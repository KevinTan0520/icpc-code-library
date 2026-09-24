#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
const int N = 1e4 + 10;
const int M = 5e4 + 10;
const int INF = 0x3f3f3f3f;
int n, m, s, t, u[M], v[M], l[M], r[M], bal[N], ss, tt, ef, idx, id[N], rf;
struct edge { int to, w, nex; } e[M];
int head[N], tot = -1;
void adde(int u, int v, int w) {
	e[++ tot] = {v, w, head[u]};
	head[u] = tot;
	e[++ tot] = {u, 0, head[v]};
	head[v] = tot;
}
int dis[N];
queue<int> q;
bool bfs(int s) {
	memset(dis, 0, sizeof dis);
	dis[s] = 1;
	q.push(s);
	while(!q.empty()) {
		int u = q.front();
		q.pop();
		for(int i = head[u]; i != -1; i = e[i].nex) {
			int v = e[i].to;
			if(e[i].w && !dis[v]) {
				dis[v] = dis[u] + 1;
				q.push(v);
			}
		}
	}
	return dis[t] != 0;
}
int cur[N];
int dfs(int u, int flow) {
	if(u == t) return flow;
	int delta = flow;
	for(int i = cur[u]; i != -1; i = e[i].nex) {
		cur[u] = i;
		int v = e[i].to;
		if(dis[v] == dis[u] + 1 && e[i].w) {
			int f = dfs(v, min(delta, e[i].w));
			delta -= f;
			e[i].w -= f;
			e[i ^ 1].w += f;
			if(delta == 0) break;
		}
	}
	return flow - delta;
}
int mf;
void dinic() {
	mf = 0;
	while(bfs(s)) {
		memcpy(cur, head, sizeof cur);
		mf += dfs(s, INF);
	}
}
int main() {
	ios :: sync_with_stdio(0);
	cin.tie(0); cout.tie(0);
	memset(head, -1, sizeof head);
	cin >> n >> m >> ss >> tt;
	for(int i = 1; i <= m; i++) {
		cin >> u[i] >> v[i] >> l[i] >> r[i];
		adde(u[i], v[i], r[i] - l[i]);//建第一次用来判可行的图
		bal[v[i]] += l[i];
		bal[u[i]] -= l[i];//更新节点流量差
	}
	s = n + 1;
	t = n + 2;//建立新的超级源点和超级汇点
	adde(tt, ss, INF);//无源汇转有源汇
	idx = tot;//记录一下这一条边的id，之后要删掉
	for(int i = 1; i <= n; i++) {
		if(bal[i] > 0) {
			adde(s, i, bal[i]);
			ef += bal[i];//应该流的流量
		}
		if(bal[i] < 0) adde(i, t, -bal[i]);
	}
	dinic();
	if(mf < ef) cout << "N\n";//没有满流，形不成可行流
	else {
		rf = e[idx].w;//记录之前可行流流量
		e[idx].w = e[idx ^ 1].w = 0;//删掉之前的边
		s = ss;
		t = tt;//更新源点和汇点为题目要求的源汇
		dinic();//再跑一遍最大流
		cout << rf + mf << endl;//流量为两次流量之和
	}
	return 0;
}