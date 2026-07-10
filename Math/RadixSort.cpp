#include <bits/stdc++.h>
using namespace std;

void radix_sort(vector<int> &a, int n) {
    vector<int> cnt(10, 0), tmp(n + 1, 0), p10;
    int maxx = 0, maxk = 0;
    for (int i = 1; i <= n; i++) maxx = max(maxx, a[i]);
    while (maxx > 0) {
        maxk++;
        maxx /= 10;
    }
    p10.push_back(1);
    for (int i = 1; i < maxk; i++) p10.push_back(p10[i - 1] * 10);
    for (int k = 0; k < maxk; k++) {
        cnt.assign(10, 0);
        for (int i = 1; i <= n; i++) {
            cnt[a[i] / p10[k] % 10]++;
        }
        for (int i = 1; i <= 9; i++) {
            cnt[i] = cnt[i - 1] + cnt[i];
        }
        for (int i = n; i >= 1; i--) {
            tmp[cnt[a[i] / p10[k] % 10]] = a[i];
            cnt[a[i] / p10[k] % 10]--;
        }
        a = tmp;
    }
    return;
}

int main() {
    // int n;
    // cin >> n;
    // vector<int> a(n + 1, 0);
    // for (int i = 1; i <= n; i++) cin >> a[i];
    // radix_sort(a, n);
    // for (int i = 1; i <= n; i++) cout << a[i] << ' ';
    return 0;
}