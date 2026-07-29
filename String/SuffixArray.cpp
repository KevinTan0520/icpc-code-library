#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    vector<int> sa, rk, height;

    void get_sa(const vector<int> &s) {
        int n = s.size();
        sa.resize(n);
        rk.resize(n);
        if (n == 0) return;

        vector<int> value = s;
        sort(value.begin(), value.end());
        value.erase(unique(value.begin(), value.end()), value.end());

        int m = value.size();
        vector<int> x(n), y(n), cnt(max(n, m), 0);
        for (int i = 0; i < n; i++) {
            x[i] = lower_bound(value.begin(), value.end(), s[i]) - value.begin();
            cnt[x[i]]++;
        }
        for (int i = 1; i < m; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) sa[--cnt[x[i]]] = i;

        for (int k = 1; k < n; k <<= 1) {
            int p = 0;
            for (int i = n - k; i < n; i++) y[p++] = i;
            for (int i = 0; i < n; i++) {
                if (sa[i] >= k) y[p++] = sa[i] - k;
            }

            fill(cnt.begin(), cnt.begin() + m, 0);
            for (int i = 0; i < n; i++) cnt[x[y[i]]]++;
            for (int i = 1; i < m; i++) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--) sa[--cnt[x[y[i]]]] = y[i];

            swap(x, y);
            x[sa[0]] = 0;
            p = 1;
            for (int i = 1; i < n; i++) {
                int a = sa[i - 1], b = sa[i];
                bool same = y[a] == y[b];
                same &= (a + k < n ? y[a + k] : -1) ==
                        (b + k < n ? y[b + k] : -1);
                x[b] = same ? p - 1 : p++;
            }
            m = p;
            if (p == n) break;
        }

        for (int i = 0; i < n; i++) rk[sa[i]] = i;
        return;
    }

    void get_height(const vector<int> &s) {
        int n = s.size(), k = 0;
        height.assign(n, 0);
        for (int i = 0; i < n; i++) {
            int r = rk[i];
            if (r == 0) continue;
            int j = sa[r - 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
            height[r] = k;
            if (k != 0) k--;
        }
        return;
    }

    void build(const vector<int> &s) {
        get_sa(s);
        get_height(s);
        return;
    }

    void build(const string &s) {
        vector<int> a(s.size());
        for (int i = 0; i < (int)s.size(); i++) {
            a[i] = (unsigned char)s[i] + 1;
        }
        build(a);
        return;
    }
};

// Prints the occurrence count and all 0-based starting positions.
void suffix_array_find_all(const string &s, const string &t) {
    vector<int> pos;
    if (!t.empty()) {
        SuffixArray suf;
        suf.build(s);
        int n = s.size();
        auto compare = [&](int st) {
            int i = 0;
            while (st + i < n && i < (int)t.size() &&
                   (unsigned char)s[st + i] == (unsigned char)t[i]) i++;
            if (i == (int)t.size()) return 0;
            if (st + i == n) return -1;
            return (unsigned char)s[st + i] < (unsigned char)t[i] ? -1 : 1;
        };

        int l = 0, r = n;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (compare(suf.sa[mid]) < 0) l = mid + 1;
            else r = mid;
        }
        int left = l;
        l = 0, r = n;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (compare(suf.sa[mid]) <= 0) l = mid + 1;
            else r = mid;
        }
        for (int i = left; i < l; i++) pos.push_back(suf.sa[i]);
        sort(pos.begin(), pos.end());
    }

    cout << pos.size() << '\n';
    for (int i = 0; i < (int)pos.size(); i++) {
        if (i != 0) cout << ' ';
        cout << pos[i];
    }
    cout << '\n';
    return;
}

// Prints the length, all positions, and the selected repeated substring.
void longest_repeated_substring(const string &s) {
    SuffixArray suf;
    suf.build(s);
    int n = s.size(), max_len = 0, start = -1;
    for (int i = 1; i < n; i++) {
        int cur_start = min(suf.sa[i - 1], suf.sa[i]);
        if (suf.height[i] > max_len ||
            (suf.height[i] == max_len && suf.height[i] != 0 &&
             cur_start < start)) {
            max_len = suf.height[i];
            start = cur_start;
        }
    }

    vector<int> pos;
    if (max_len != 0) {
        int left = suf.rk[start], right = suf.rk[start];
        while (left > 0 && suf.height[left] >= max_len) left--;
        while (right + 1 < n && suf.height[right + 1] >= max_len) right++;
        for (int i = left; i <= right; i++) pos.push_back(suf.sa[i]);
        sort(pos.begin(), pos.end());
    }

    cout << max_len << '\n';
    for (int i = 0; i < (int)pos.size(); i++) {
        if (i != 0) cout << ' ';
        cout << pos[i];
    }
    cout << '\n';
    if (max_len != 0) cout << s.substr(start, max_len);
    cout << '\n';
    return;
}

// Prints the length and one 0-based starting position in every string.
void longest_common_substring(const vector<string> &strs) {
    int k = strs.size();
    if (k == 0) {
        cout << 0 << "\n\n";
        return;
    }
    if (k == 1) {
        cout << strs[0].size() << "\n0\n";
        return;
    }
    for (const string &s : strs) {
        if (s.empty()) {
            cout << 0 << '\n';
            for (int i = 0; i < k; i++) {
                if (i != 0) cout << ' ';
                cout << 0;
            }
            cout << '\n';
            return;
        }
    }

    vector<int> a, owner, local_pos;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < (int)strs[i].size(); j++) {
            a.push_back(k + 1 + (unsigned char)strs[i][j]);
            owner.push_back(i);
            local_pos.push_back(j);
        }
        a.push_back(i + 1);
        owner.push_back(-1);
        local_pos.push_back(-1);
    }

    SuffixArray suf;
    suf.build(a);
    int n = a.size(), left = 0, kind = 0, best_len = 0;
    int best_left = -1, best_right = -1;
    vector<int> cnt(k, 0);
    deque<int> q;
    for (int right = 0; right < n; right++) {
        if (right != 0) {
            while (!q.empty() && suf.height[q.back()] >= suf.height[right]) {
                q.pop_back();
            }
            q.push_back(right);
        }
        int id = owner[suf.sa[right]];
        if (id != -1 && cnt[id]++ == 0) kind++;

        while (kind == k) {
            int cur = suf.height[q.front()];
            if (cur > best_len) {
                best_len = cur;
                best_left = left;
                best_right = right;
            }
            id = owner[suf.sa[left]];
            if (id != -1 && --cnt[id] == 0) kind--;
            left++;
            while (!q.empty() && q.front() <= left) q.pop_front();
        }
    }

    vector<int> answer(k, 0);
    if (best_len != 0) {
        fill(answer.begin(), answer.end(), -1);
        for (int i = best_left; i <= best_right; i++) {
            int p = suf.sa[i], id = owner[p];
            if (id != -1 && (answer[id] == -1 || local_pos[p] < answer[id])) {
                answer[id] = local_pos[p];
            }
        }
    }

    cout << best_len << '\n';
    for (int i = 0; i < k; i++) {
        if (i != 0) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';
    return;
}

// Prints the length and the earliest 0-based starting position.
void longest_palindromic_substring(const string &s) {
    int n = s.size();
    if (n == 0) {
        cout << "0\n-1\n";
        return;
    }

    vector<int> a;
    a.reserve(2 * n + 1);
    for (unsigned char c : s) a.push_back(c + 2);
    a.push_back(1);
    for (int i = n - 1; i >= 0; i--) a.push_back((unsigned char)s[i] + 2);

    SuffixArray suf;
    suf.build(a);
    int len = a.size();
    vector<int> lg(len + 1, 0);
    for (int i = 2; i <= len; i++) lg[i] = lg[i >> 1] + 1;
    vector<vector<int>> st(lg[len] + 1, vector<int>(len));
    st[0] = suf.height;
    for (int j = 1; j <= lg[len]; j++) {
        for (int i = 0; i + (1 << j) <= len; i++) {
            st[j][i] = min(st[j - 1][i],
                           st[j - 1][i + (1 << (j - 1))]);
        }
    }
    auto lcp = [&](int x, int y) {
        if (x == y) return len - x;
        int l = suf.rk[x], r = suf.rk[y];
        if (l > r) swap(l, r);
        l++;
        int j = lg[r - l + 1];
        return min(st[j][l], st[j][r - (1 << j) + 1]);
    };

    int best_len = 1, best_start = 0;
    for (int i = 0; i < n; i++) {
        int radius = lcp(i, 2 * n - i);
        int cur_len = radius * 2 - 1, cur_start = i - radius + 1;
        if (cur_len > best_len ||
            (cur_len == best_len && cur_start < best_start)) {
            best_len = cur_len;
            best_start = cur_start;
        }
    }
    for (int i = 1; i < n; i++) {
        int radius = lcp(i, 2 * n + 1 - i);
        int cur_len = radius * 2, cur_start = i - radius;
        if (cur_len > best_len ||
            (cur_len == best_len && cur_start < best_start)) {
            best_len = cur_len;
            best_start = cur_start;
        }
    }

    cout << best_len << '\n' << best_start << '\n';
    return;
}

int main() {
    return 0;
}
