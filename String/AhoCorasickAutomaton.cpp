#include <bits/stdc++.h>
using namespace std;

struct AhoCorasickAutomaton {
    struct Node {
        int son[26], fail, output, depth;
        bool is_end;
        Node() {
            memset(son, 0, sizeof(son));
            fail = 0;
            output = -1;
            depth = 0;
            is_end = 0;
        }
    };

    vector<Node> trie;
    vector<int> pattern_node, order;

    void init() {
        trie.assign(1, Node());
        pattern_node.clear();
        order.clear();
        return;
    }

    // Returns the 0-based id of the inserted pattern.
    int insert(const string &s) {
        int u = 0, len = s.length();
        for (int i = 0; i < len; i++) {
            int c = s[i] - 'a';
            if (trie[u].son[c] == 0) {
                trie[u].son[c] = trie.size();
                trie.push_back(Node());
                trie.back().depth = trie[u].depth + 1;
            }
            u = trie[u].son[c];
        }
        trie[u].is_end = 1;
        pattern_node.push_back(u);
        return pattern_node.size() - 1;
    }

    void build() {
        queue<int> q;
        order.clear();
        for (int c = 0; c < 26; c++) {
            int v = trie[0].son[c];
            if (v != 0) q.push(v);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);
            int f = trie[u].fail;
            if (trie[f].is_end) trie[u].output = f;
            else trie[u].output = trie[f].output;
            for (int c = 0; c < 26; c++) {
                int v = trie[u].son[c];
                if (v == 0) {
                    trie[u].son[c] = trie[f].son[c];
                    continue;
                }
                trie[v].fail = trie[f].son[c];
                q.push(v);
            }
        }
        return;
    }

    vector<long long> match_count(const string &text) const {
        vector<long long> cnt(trie.size(), 0), res(pattern_node.size(), 0);
        int u = 0, len = text.length(), n = pattern_node.size();
        for (int i = 0; i < len; i++) {
            u = trie[u].son[text[i] - 'a'];
            cnt[u]++;
        }
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int v = order[i];
            cnt[trie[v].fail] += cnt[v];
        }
        for (int i = 0; i < n; i++) {
            res[i] = cnt[pattern_node[i]];
        }
        return res;
    }

    // res[i] contains all 0-based starting positions of pattern i.
    vector<vector<int>> match_all(const string &text) const {
        vector<vector<int>> pos(trie.size()), res(pattern_node.size());
        int u = 0, len = text.length(), n = pattern_node.size();
        for (int i = 0; i < len; i++) {
            u = trie[u].son[text[i] - 'a'];
            int v = trie[u].is_end ? u : trie[u].output;
            while (v != -1) {
                pos[v].push_back(i - trie[v].depth + 1);
                v = trie[v].output;
            }
        }
        for (int i = 0; i < n; i++) {
            res[i] = pos[pattern_node[i]];
        }
        return res;
    }
};

int main() {
    return 0;
}
