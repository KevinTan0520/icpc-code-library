#include <bits/stdc++.h>
using namespace std;

// 重链剖分：维护树上路径/子树的区间加与区间和，所有结果对 mod 取模。
// 用法：init(n, root, mod, value) -> add_edge 共 n-1 次 -> build() -> 调用四种操作。
struct HeavyLightDecomposition {
    // DFS 序上的懒标记线段树，通常只通过外层接口调用。
    struct SegmentTree {
        int n;
        long long mod;
        vector<long long> sum, lazy;

        long long normalize(long long value) const {
            value %= mod;
            if (value < 0) value += mod;
            return value;
        }

        void init(int n_, long long mod_) {
            n = n_;
            mod = mod_;
            sum.assign(4 * n + 1, 0);
            lazy.assign(4 * n + 1, 0);
        }

        void pushup(int id) {
            sum[id] = (sum[id * 2] + sum[id * 2 + 1]) % mod;
        }

        void apply(int id, int l, int r, long long value) {
            value = normalize(value);
            sum[id] = (sum[id] + value * (r - l + 1)) % mod;
            lazy[id] = (lazy[id] + value) % mod;
        }

        void pushdown(int id, int l, int r) {
            if (lazy[id] == 0 || l == r) return;
            int mid = (l + r) / 2;
            apply(id * 2, l, mid, lazy[id]);
            apply(id * 2 + 1, mid + 1, r, lazy[id]);
            lazy[id] = 0;
        }

        void build(const vector<long long> &a, int l, int r, int id) {
            if (l == r) {
                sum[id] = normalize(a[l]);
                return;
            }
            int mid = (l + r) / 2;
            build(a, l, mid, id * 2);
            build(a, mid + 1, r, id * 2 + 1);
            pushup(id);
        }

        void range_add(int ql, int qr, long long value, int l, int r, int id) {
            if (ql <= l && r <= qr) {
                apply(id, l, r, value);
                return;
            }
            pushdown(id, l, r);
            int mid = (l + r) / 2;
            if (ql <= mid) range_add(ql, qr, value, l, mid, id * 2);
            if (qr > mid) range_add(ql, qr, value, mid + 1, r, id * 2 + 1);
            pushup(id);
        }

        long long range_sum(int ql, int qr, int l, int r, int id) {
            if (ql <= l && r <= qr) return sum[id];
            pushdown(id, l, r);
            int mid = (l + r) / 2;
            long long result = 0;
            if (ql <= mid) result += range_sum(ql, qr, l, mid, id * 2);
            if (qr > mid) result += range_sum(ql, qr, mid + 1, r, id * 2 + 1);
            return result % mod;
        }
    } seg;

    int n, root, timer;
    long long mod;
    vector<vector<int>> adj;
    vector<long long> value;
    vector<int> parent, depth, size, heavy, top, id;
    // size：子树大小；heavy：重儿子；top：所在重链顶端；id：DFS 序位置。

    // 点编号和 value_ 均从 1 开始；边需要在 build() 前全部加入。
    void init(int n_, int root_, long long mod_, const vector<long long> &value_) {
        n = n_;
        root = root_;
        mod = mod_;
        value = value_;
        adj.assign(n + 1, {});
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 第一遍 DFS：求父亲、深度、子树大小和重儿子。
    void dfs_size(int u, int p) {
        parent[u] = p;
        depth[u] = depth[p] + 1;
        size[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_size(v, u);
            size[u] += size[v];
            if (heavy[u] == 0 || size[v] > size[heavy[u]]) heavy[u] = v;
        }
    }

    // 第二遍 DFS：重儿子优先，使每条重链及每棵子树都对应连续区间。
    void dfs_chain(int u, int chain_top, vector<long long> &ordered_value) {
        top[u] = chain_top;
        id[u] = ++timer;
        ordered_value[timer] = value[u];
        if (heavy[u] != 0) dfs_chain(heavy[u], chain_top, ordered_value);
        for (int v : adj[u]) {
            if (v == parent[u] || v == heavy[u]) continue;
            dfs_chain(v, v, ordered_value);
        }
    }

    // 完成剖分并按 DFS 序建立线段树。
    void build() {
        timer = 0;
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        size.assign(n + 1, 0);
        heavy.assign(n + 1, 0);
        top.assign(n + 1, 0);
        id.assign(n + 1, 0);
        dfs_size(root, 0);

        vector<long long> ordered_value(n + 1);
        dfs_chain(root, root, ordered_value);
        seg.init(n, mod);
        seg.build(ordered_value, 1, n, 1);
    }

    // 路径 u-v 上所有点加 value。
    void path_add(int u, int v, long long value) {
        while (top[u] != top[v]) {
            if (depth[top[u]] < depth[top[v]]) swap(u, v);
            seg.range_add(id[top[u]], id[u], value, 1, n, 1);
            u = parent[top[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        seg.range_add(id[u], id[v], value, 1, n, 1);
    }

    // 查询路径 u-v 上的点权和。
    long long path_sum(int u, int v) {
        long long result = 0;
        while (top[u] != top[v]) {
            if (depth[top[u]] < depth[top[v]]) swap(u, v);
            result += seg.range_sum(id[top[u]], id[u], 1, n, 1);
            result %= mod;
            u = parent[top[u]];
        }
        if (depth[u] > depth[v]) swap(u, v);
        result += seg.range_sum(id[u], id[v], 1, n, 1);
        return result % mod;
    }

    // 以初始 root 为根，u 的整棵子树加 value。
    void subtree_add(int u, long long value) {
        seg.range_add(id[u], id[u] + size[u] - 1, value, 1, n, 1);
    }

    // 查询以初始 root 为根时，u 子树的点权和。
    long long subtree_sum(int u) {
        return seg.range_sum(id[u], id[u] + size[u] - 1, 1, n, 1);
    }
};

int main() {
    return 0;
}
