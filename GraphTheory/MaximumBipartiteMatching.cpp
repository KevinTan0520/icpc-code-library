#include <bits/stdc++.h>
using namespace std;

int n, m, s, t, r[105][105], ans, d[105], cur[105];
queue <int> q;
bool bfs()
{
	memset(d, 0, sizeof(d)); 
	
	d[s] = 1, q.push(s); 
	
	while(!q.empty())
	{
		int u = q.front(); q.pop();
		for(int i=1; i<=n+2; i++)
			if(d[i] == 0 and r[u][i] > 0)
				d[i] = d[u] + 1, q.push(i);
	}
	
	return d[t];
}

int dfs(int u, int sum)
{
	if(sum == 0 or u == t) return sum;
	
	int flow = 0;
	
	for(int v=cur[u]; v<=n+2; v++)
	{
		cur[u] = v;
		if(d[v] == d[u] + 1 and r[u][v] > 0)
		{
			int d = dfs(v, min(sum, r[u][v]));
			r[u][v] -= d, r[v][u] += d;
			flow += d, sum -= d;
		}
	}
	
	return flow;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	
	cin >> m >> n;
	
	s = n+1, t = n+2;
	
	while(1)
	{
		int u, v;
		cin >> u >> v;
		
		if(u == -1 and v == -1) break;
		
		r[u][v] = 1;
	}
	
	
	for(int i=1; i<=m; i++) r[s][i] = 1;
	for(int i=m+1; i<=n; i++) r[i][t] = 1;
	
	while(bfs()) 
	{
		for(int i=1; i<=n+2; i++) cur[i] = 1;
		ans += dfs(s, 1e9);
	}

	cout << ans << "\n";
	
	for(int u=1; u<=m; u++)
	{
		for(int v=m+1; v<=n; v++)
		{
			if(r[v][u]) cout << u << " " << v << "\n";
		}
	}	
	
	return 0;
}