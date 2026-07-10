#include <bits/stdc++.h>
using namespace std;

void get_next(const string &s, vector<int> &nxt) {
    int len = s.length();
    nxt.assign(len + 1, 0);
    nxt[0] = -1;
    int i = 2, cn = 0;
    while (i <= len) {
        if (s[i - 1] == s[cn]) {
            nxt[i++] = ++cn;
        } else if (cn != 0) {
            cn = nxt[cn];
        } else {
            nxt[i++] = 0;
        }
    }
    return;
}

int KMP_find_first(const string &s1, const string &s2, const vector<int> &nxt) {
    int len1 = s1.length(), len2 = s2.length(), x = 0, y = 0;
    while (x < len1 && y < len2) {
        if (s1[x] == s2[y]) {
            x++, y++;
        } else if (y == 0) {
            x++;
        } else {
            y = nxt[y];
        }
    }
    if (y == len2) return x - y;
    else return -1;
}

vector<int> KMP_find_all(const string &s1,const string &s2, const vector<int> &nxt) {
    vector<int> res;
    int len1 = s1.length(), len2 = s2.length(), x = 0, y = 0;
    if (len2 == 0) return res;
    while (x < len1) {
        if (s1[x] == s2[y]) {
            x++, y++;
        } else if (y == 0) {
            x++;
        } else {
            y = nxt[y];
        }
        if (y == len2) {
            res.push_back(x - y);
            y = nxt[y];
        }
    }
    return res;
}


int main() {
    return 0;
}
