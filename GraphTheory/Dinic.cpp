#include <bits/stdc++.h>
#define int long long

using namespace std;

struct Edge{
	int nxt, to, r;
}e[10005];

int tot = 1, head[205];
void add_edge(int u, int v, int w)
{
	e[++tot] = {head[u], v, w};
	head[u] = tot;
}

int n, m, s, t, ans, d[205], cur[205];

queue <int> q;
bool bfs()
{
	memset(d, 0, sizeof(d)); 
	
	d[s] = 1, q.push(s); 
	
	while(!q.empty())
	{
		int u = q.front(); q.pop();
		for(int i=head[u]; i; i=e[i].nxt)
		{
			int v = e[i].to;
			if(d[v] == 0 and e[i].r > 0)
				d[v] = d[u] + 1, q.push(v);
		}
	}
	
	return d[t];
}

int dfs(int u, int sum)
{
	if(sum == 0 or u == t) return sum;
	
	int flow = 0;
	
	for(int i=cur[u]; i; i=e[i].nxt)
	{
		cur[u] = i;
		int v = e[i].to;
		if(d[v] == d[u] + 1 and e[i].r > 0)
		{
			int d = dfs(v, min(sum, e[i].r));
			e[i].r -= d, e[i^1].r += d;
			flow += d, sum -= d;
		}
	}
	
	return flow;
}

signed main()
{
	cin >> n >> m >> s >> t;
	
	for(int i=1; i<=m; i++)
	{
		int u, v, w;
		cin >> u >> v >> w;
		add_edge(u, v, w);
		add_edge(v, u, 0);
	}
	
	while(bfs()) 
	{
		for(int i=1; i<=n; i++) cur[i] = head[i];
		ans += dfs(s, 1e9);
	}

	cout << ans;
}