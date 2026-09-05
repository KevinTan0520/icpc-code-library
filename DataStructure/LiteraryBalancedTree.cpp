#include <bits/stdc++.h>
using namespace std;

// 文艺平衡树：按序列位置维护 Splay，区间翻转均摊 O(log n)。
// 序列下标从 1 开始，内部在首尾各加入一个哨兵节点。
struct LiteraryBalancedTree {
    struct Node {
        int son[2], fa, value, size;
        bool reversed;
        Node() {
            son[0] = son[1] = 0;
            fa = value = size = 0;
            reversed = false;
        }
    };

    int root, n;
    vector<Node> tr;

    int new_node(int value, int fa) {
        Node node;
        node.fa = fa;
        node.value = value;
        node.size = 1;
        tr.push_back(node);
        return static_cast<int>(tr.size()) - 1;
    }

    void pushup(int u) {
        tr[u].size = tr[tr[u].son[0]].size
            + tr[tr[u].son[1]].size + 1;
        return;
    }

    void apply_reverse(int u) {
        if (u == 0) return;
        swap(tr[u].son[0], tr[u].son[1]);
        tr[u].reversed = !tr[u].reversed;
        return;
    }

    void pushdown(int u) {
        if (!tr[u].reversed) return;
        apply_reverse(tr[u].son[0]);
        apply_reverse(tr[u].son[1]);
        tr[u].reversed = false;
        return;
    }

    int build(const vector<int> &a, int l, int r, int fa) {
        if (l > r) return 0;
        int mid = (l + r) / 2;
        int u = new_node(a[mid], fa);
        int left = build(a, l, mid - 1, u);
        tr[u].son[0] = left;
        int right = build(a, mid + 1, r, u);
        tr[u].son[1] = right;
        pushup(u);
        return u;
    }

    // a[1..n] 为初始序列，a[0] 不使用。
    void init(const vector<int> &a) {
        assert(!a.empty());
        n = static_cast<int>(a.size()) - 1;
        vector<int> sequence(n + 2, 0);
        for (int i = 1; i <= n; i++) sequence[i] = a[i];
        tr.assign(1, Node());
        root = build(sequence, 0, n + 1, 0);
        return;
    }

    // 初始化为排列 1,2,...,size。
    void init(int size) {
        assert(size >= 0);
        vector<int> a(size + 1);
        iota(a.begin() + 1, a.end(), 1);
        init(a);
        return;
    }

    void rotate(int u) {
        int f = tr[u].fa, g = tr[f].fa;
        int direction = u == tr[f].son[1];
        tr[f].son[direction] = tr[u].son[direction ^ 1];
        if (tr[u].son[direction ^ 1] != 0) {
            tr[tr[u].son[direction ^ 1]].fa = f;
        }
        tr[u].son[direction ^ 1] = f;
        tr[f].fa = u;
        tr[u].fa = g;
        if (g != 0) tr[g].son[f == tr[g].son[1]] = u;
        pushup(f);
        pushup(u);
        return;
    }

    void splay(int u, int target = 0) {
        vector<int> path;
        for (int v = u; v != target; v = tr[v].fa) path.push_back(v);
        if (target != 0) path.push_back(target);
        for (auto it = path.rbegin(); it != path.rend(); ++it) pushdown(*it);
        while (tr[u].fa != target) {
            int f = tr[u].fa, g = tr[f].fa;
            if (g != target) {
                if ((u == tr[f].son[1]) == (f == tr[g].son[1])) rotate(f);
                else rotate(u);
            }
            rotate(u);
        }
        if (target == 0) root = u;
        return;
    }

    // 返回整棵树中第 k 个节点（包括两个哨兵）。
    int kth_node(int k) {
        assert(1 <= k && k <= tr[root].size);
        int u = root;
        while (u != 0) {
            pushdown(u);
            int left_size = tr[tr[u].son[0]].size;
            if (k <= left_size) {
                u = tr[u].son[0];
            } else if (k == left_size + 1) {
                return u;
            } else {
                k -= left_size + 1;
                u = tr[u].son[1];
            }
        }
        assert(false);
        return 0;
    }

    void range_reverse(int l, int r) {
        assert(1 <= l && l <= r && r <= n);
        int left = kth_node(l);
        splay(left);
        int right = kth_node(r + 2);
        splay(right, left);
        apply_reverse(tr[right].son[0]);
        pushup(right);
        pushup(left);
        return;
    }

    int kth(int k) {
        assert(1 <= k && k <= n);
        int u = kth_node(k + 1);
        splay(u);
        return tr[u].value;
    }

    // 返回 1-based 数组，res[0] 为占位元素，实际序列为 res[1..n]。
    vector<int> to_vector() {
        vector<int> res;
        res.reserve(n + 2);
        vector<int> stack;
        int u = root;
        while (u != 0 || !stack.empty()) {
            while (u != 0) {
                pushdown(u);
                stack.push_back(u);
                u = tr[u].son[0];
            }
            u = stack.back();
            stack.pop_back();
            res.push_back(tr[u].value);
            u = tr[u].son[1];
        }
        return vector<int>(res.begin(), res.end() - 1);
    }
};

int main() {
    return 0;
}
