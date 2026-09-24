#include <bits/stdc++.h>
#define int long long
#define pii pair<int,int>

using namespace std;

const int N = 5e3 + 5;
const int M = 5e4 + 5;
const int inf = 1e9;

int n, m, s, t, tot = 1, head[N], dis[N], cur[N], pre[N];
struct Edge{
	int nxt, to, r, cost;
}e[M << 1];

void add(int u, int v, int w, int c)
{
	e[++tot] = {head[u], v, w, c}; head[u] = tot;
	e[++tot] = {head[v], u, 0, -c}; head[v] = tot;
}

bool inq[N];
queue <int> q;
bool spfa()
{
	for(int i = 1; i <= n; i ++ )
		dis[i] = inf, inq[i] = 0;
	
	q.push(s), dis[s] = 0, inq[s] = 0; 
	
	while(!q.empty())
	{
		int u = q.front(); q.pop();
		inq[u] = 0;
		for(int i = head[u]; i; i = e[i].nxt)
		{
			int v = e[i].to;
			if(e[i].r > 0 and dis[v] > dis[u] + e[i].cost)
			{
				dis[v] = dis[u] + e[i].cost, pre[v] = i;
				if(!inq[v]) q.push(v), inq[v] = 1;
			}
		}
	}
	
	return dis[t] < inf;
}

bool vis[N];

pii dfs(int u, int sum)
{
	vis[u] = 1;
	
	if(u == t or sum == 0) return {sum, 0};
	
	int maxflow = 0, mincost = 0;
	for(int i = cur[u]; i; i = e[i].nxt)
	{
		cur[u] = i;
		int v = e[i].to;
		if(!vis[v] and e[i].r > 0 and dis[u] + e[i].cost == dis[v])
		{
			pii delta = dfs(v, min(e[i].r, sum));
			sum -= delta.first,
			maxflow += delta.first, 
			mincost	+= e[i].cost * delta.first + delta.second,
			e[i].r -= delta.first, 
			e[i ^ 1].r += delta.first;
		}
	}
	
	return {maxflow, mincost};
}

signed main()
{
	cin >> n >> m >> s >> t;
	
	for(int i=1; i<=m; i++)
	{
		int u, v, w, c;
		cin >> u >> v >> w >> c;
		add(u, v, w, c);
	}
	
	int maxflow = 0, mincost = 0;
	
	while(spfa())
	{
		for(int i=1; i<=n; i++) vis[i] = 0, cur[i] = head[i];
		pii res = dfs(s, inf);
		maxflow += res.first;
		mincost += res.second;
	}

	cout << maxflow << " " << mincost;
}