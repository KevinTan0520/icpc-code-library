#include <iostream>
#include <vector>
#define int long long

using namespace std;

const int N = 5e5 + 5;
const int M = 1e6 + 5;
const int mod = 1e9 + 7;

int n, m, pw[N + M];

struct Edge{
	int to, nxt;
}e1[M << 1], e2[M << 1];

int tot1 = 1, head1[N];
void add1(int u, int v)
{
	e1[++tot1] = {v, head1[u]}; head1[u] = tot1;
}

int tot2 = 1, head2[N];
void add2(int u, int v)
{
	e2[++tot2] = {v, head2[u]}; head2[u] = tot2;
}

int low[N], dfn[N], idx;
bool bridge[M << 1];
void Tarjan(int u, int from)
{
	low[u] = dfn[u] = ++idx;
	for(int i=head1[u]; i; i=e1[i].nxt)
	{
		if((i ^ 1) == from) continue;
		int v = e1[i].to;
		
		if(!dfn[v]) // tree edge
		{
			Tarjan(v, i);
			low[u] = min(low[u], low[v]);
			if(low[v] >= dfn[v])
				bridge[i] = bridge[i ^ 1] = 1;
		}
		else low[u] = min(low[u], dfn[v]); // back edge
	}
}

int cnt, belong[N], V[N], E[N];
void dfs0(int u)
{
	belong[u] = cnt, V[cnt] ++ ;
	for(int i = head1[u]; i; i = e1[i].nxt)
	{
		int v = e1[i].to;
		if(belong[v] or bridge[i]) continue;
		dfs0(v);
	}
}

int ans, siz[N], f[N], g[N];
void dfs(int u, int from)
{
	f[u] = pw[E[u]] * (pw[V[u]] - 1) % mod,
	g[u] = pw[E[u]], siz[u] = E[u];
	for(int i = head2[u]; i; i=e2[i].nxt)
	{
		if((i ^ 1) == from) continue;
		int v = e2[i].to;
		dfs(v, i);
		siz[u] += siz[v] + 1;
		f[u] = f[u] * 2 * g[v] % mod + (f[u] + g[u]) * f[v] % mod;			f[u] %= mod;
		g[u] *= 2 * g[v];					 								g[u] %= mod;
	}
	if(u == 1) ans += f[u], ans %= mod;
	else ans += f[u] * pw[m - siz[u] - 1] % mod,	ans %= mod;
}

signed main()
{
	cin >> n >> m;
	
	pw[0] = 1; for(int i=1; i<=m; i++) pw[i] = (pw[i-1] << 1) % mod;
	
	for(int i=1; i<=m; i++)
	{
		int u, v;
		cin >> u >> v;
		add1(u, v); add1(v, u);
	}
	
	Tarjan(1, 0);
	
	for(int i=1; i<=n; i++)
	{
		if(!belong[i]) ++ cnt, dfs0(i);
	}
	
	for(int i=2; i<=tot1; i++)
	{
		int u = e1[i].to, v = e1[i ^ 1].to;
		if(belong[u] == belong[v]) E[belong[u]] ++ ;
		else add2(belong[u], belong[v]);
	}
	
	for(int i=1; i<=cnt; i++) E[i] >>= 1;
	
	dfs(1, 0);
	
	cout << ans;
}
