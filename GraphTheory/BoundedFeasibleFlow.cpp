#include<bits/stdc++.h>
using namespace std;
const int N = 1010,M = 10010;
typedef long long LL;
struct edge
{
	LL x,cap,rev;
};
vector<edge> e[N];
int n,m,s,t;
int d[N],it[N];
void add(int a,int b,int c)
{
	e[a].push_back({b,c,e[b].size()});
	e[b].push_back({a,0,e[a].size() - 1});
}
void bfs()
{
	memset(d,-1,sizeof d);
	queue<int> q;
	d[s] = 0;
	q.push(s);
	while(!q.empty())
	{
		int u = q.front();
		q.pop();
		for(auto t : e[u])
			if(t.cap > 0 && d[t.x] < 0) d[t.x] = d[u] + 1,q.push(t.x);
	}
}
LL dfs(int u,LL f)
{
	if(u == t) return f;
	for(int &i = it[u]; i < e[u].size(); i ++)
	{
		edge &t = e[u][i]; 
		if(d[u] < d[t.x] && t.cap > 0)
		{
			LL d = dfs(t.x,min(f,t.cap));
			if(d > 0)
			{
				t.cap -= d;
				e[t.x][t.rev].cap += d;
				return d;
			}
		}
	}
	return 0;
}
LL dinic() 
{
	LL flow = 0;
	while(1)
	{
		bfs();
		if(d[t] < 0) break;
		memset(it,0,sizeof it);
		LL d = dfs(s,1e18);
		while(d > 0)
		{
			flow += d;
			d = dfs(s,1e18);
		}
	}
	return flow;
}
int f[N];
pair<pair<int,int>,int> idx[M];
int main()
{
	LL sum = 0;
	cin>>n>>m;
	s = 0,t = n + 1;
	for(int i = 1; i <= m; i ++)
	{
		int x,y,z,w;
		cin>>x>>y>>z>>w;
		f[x] -= z,f[y] += z;
		add(x,y,w - z);
		idx[i] = {{y,e[y].size() - 1},z};
	}
	for(int i = 1; i <= n; i ++)
		if(f[i] > 0) add(s,i,f[i]),sum += f[i];
		else if(f[i] < 0) add(i,t,-f[i]);
	if(dinic() < sum)
	{
		puts("No");
		return 0;
	}
	puts("Yes");
	for(int i = 1; i <= m; i ++) cout<<e[idx[i].first.first][idx[i].first.second].cap + idx[i].second<<'\n';
	return 0;
}