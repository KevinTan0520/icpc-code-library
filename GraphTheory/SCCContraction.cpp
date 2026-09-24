#include<bits/stdc++.h>
using namespace std;
const int MAXN=1e4+5;
const int MAXM=1e5+5;
int n,m,a[MAXN],dfn[MAXN],low[MAXN],s[MAXN],idx,top,cnt,scc[MAXN],sum[MAXN];
int f[MAXN];
bool in[MAXN],mem[MAXN];
vector <int> G[MAXN];
vector <int> e[MAXN];
void dfs(int u)
{
	dfn[u]=low[u]=++idx;
	s[++top]=u, in[u]=1;
	for(auto v:G[u])
	{
		if(dfn[v] && in[v]) low[u]=min(low[u],dfn[v]);
		if(!dfn[v])
		{
			dfs(v);
			low[u]=min(low[u],low[v]);
		}
	}
	if(low[u]==dfn[u])
	{
		cnt++;
		while(s[top]!=u) 
		{
			scc[s[top]]=cnt; sum[cnt]+=a[s[top]];
			in[s[top]]=0; top-=1;
		}
		scc[s[top]]=cnt; sum[cnt]+=a[s[top]];
		in[s[top]]=0; top-=1;
	}
}
void dfs2(int u)
{
	if(mem[u]) return ;
	mem[u]=1;
	int res=0; f[u]=sum[u];
	for(auto v:e[u])
	{
		dfs2(v);
		res=max(res,f[v]);
	}
	f[u]+=res;
}
int main()
{
	cin>>n>>m;
	for(int i=1;i<=n;i++) cin>>a[i];
	for(int i=1;i<=m;i++)
	{
		int u,v;
		cin>>u>>v;
		G[u].push_back(v);
	}
	for(int i=1;i<=n;i++)
		if(!dfn[i])
			dfs(i);
			
	for(int u=1;u<=n;u++)	
		for(auto v:G[u])
			if(scc[u]!=scc[v])
				e[scc[u]].push_back(scc[v]);
				
				
	int ans=0;	
	for(int i=1;i<=cnt;i++)
		if(!mem[i])
		{
			dfs2(i);
			ans=max(ans,f[i]);
		}
	
	cout<<ans;
	return 0;
}