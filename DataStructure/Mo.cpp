#include<bits/stdc++.h>
#define int long long 
using namespace std;
const int MAXN = 5e4 + 5;
int sum, n, m, len, c[MAXN], cnt[MAXN], ans1[MAXN], ans2[MAXN];
struct Que{
	int qid, l, r;
}q[MAXN];
bool cmp(Que x, Que y)
{
	if(x.l / len == y.l / len) return x.r < y.r;
	else return x.l < y.l;
}
void add(int x) {
	sum += cnt[c[x]];
	cnt[c[x]] ++ ; 
}
void del(int x) {
	sum -= cnt[c[x]] - 1;
	cnt[c[x]] -- ;
}
int gcd(int a, int b)
{
	if(b == 0) return a;
	return gcd(b, a % b);
}
signed main()
{
	ios::sync_with_stdio(false);
	std::cin.tie(0);
	
	cin >> n >> m;
	
	len = sqrt(n);
	
	for(int i=1; i<=n; i++)
		cin >> c[i];
		
	for(int i=1; i<=m; i++)
		cin >> q[i].l >> q[i].r, q[i].qid = i;
	
	sort(q+1, q+m+1, cmp);
		
	int L = 1, R = 0;
	for(int i=1; i<=m; i++)	
	{
		while(R < q[i].r) add(++R);
		while(L > q[i].l) add(--L);
		while(R > q[i].r) del(R--);
		while(L < q[i].l) del(L++);
		ans1[q[i].qid] = sum;
		ans2[q[i].qid] = (q[i].r - q[i].l) * (q[i].r - q[i].l + 1) / 2;
	}	
	for(int i=1; i<=m; i++)
	{
		if(ans1[i] == 0) cout << "0/1\n";
		else
		{
			int g = gcd(ans1[i], ans2[i]);
			cout << ans1[i] / g << "/" << ans2[i] / g << "\n";
		}
	}
	return 0;
}