#include <bits/stdc++.h>
using namespace std;

// 可持久化 FHQ Treap：每次修改产生新版本，支持重复元素。
// 版本 0 为空树；insert/erase 返回新版本号，查询操作不会产生新版本。
struct PersistentFHQTreap {
    struct Node {
        int son[2], value, count, size;
        unsigned priority;
        Node() {
            son[0] = son[1] = 0;
            value = count = size = 0;
            priority = 0;
        }
    };

    vector<int> root;
    vector<Node> tr;
    mt19937 rng;

    // 传入操作数可预留节点池，避免大数据下 vector 扩容产生内存峰值。
    void init(int expected_operations = 0) {
        assert(expected_operations >= 0);
        root.clear();
        tr.clear();
        if (expected_operations > 0) {
            root.reserve(static_cast<size_t>(expected_operations) + 1);
            tr.reserve(static_cast<size_t>(expected_operations) * 40 + 1);
        }
        root.push_back(0);
        tr.push_back(Node());
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
        return;
    }

    int new_node(int value) {
        Node node;
        node.value = value;
        node.count = node.size = 1;
        node.priority = rng();
        tr.push_back(node);
        return static_cast<int>(tr.size()) - 1;
    }

    int clone_node(int u) {
        tr.push_back(tr[u]);
        return static_cast<int>(tr.size()) - 1;
    }

    void pushup(int u) {
        tr[u].size = tr[tr[u].son[0]].size
            + tr[tr[u].son[1]].size + tr[u].count;
        return;
    }

    // 将树分成键值小于 value 和键值不小于 value 的两部分。
    pair<int, int> split(int u, int value) {
        if (u == 0) return {0, 0};
        if (tr[u].value < value) {
            int right = tr[u].son[1];
            int x = clone_node(u);
            pair<int, int> parts = split(right, value);
            tr[x].son[1] = parts.first;
            pushup(x);
            return {x, parts.second};
        }
        int left = tr[u].son[0];
        int y = clone_node(u);
        pair<int, int> parts = split(left, value);
        tr[y].son[0] = parts.second;
        pushup(y);
        return {parts.first, y};
    }

    int merge(int x, int y) {
        if (x == 0 || y == 0) return x + y;
        if (tr[x].priority > tr[y].priority) {
            int right = tr[x].son[1];
            int u = clone_node(x);
            int merged = merge(right, y);
            tr[u].son[1] = merged;
            pushup(u);
            return u;
        }
        int left = tr[y].son[0];
        int u = clone_node(y);
        int merged = merge(x, left);
        tr[u].son[0] = merged;
        pushup(u);
        return u;
    }

    bool contains(int u, int value) const {
        while (u != 0 && tr[u].value != value) {
            u = tr[u].son[value > tr[u].value];
        }
        return u != 0;
    }

    int change_count(int u, int value, int delta) {
        int v = clone_node(u);
        if (tr[v].value == value) {
            tr[v].count += delta;
        } else {
            int direction = value > tr[v].value;
            int child = tr[v].son[direction];
            int changed = change_count(child, value, delta);
            tr[v].son[direction] = changed;
        }
        pushup(v);
        return v;
    }

    int insert_node(int u, int node) {
        if (u == 0) return node;
        if (tr[node].priority > tr[u].priority) {
            pair<int, int> parts = split(u, tr[node].value);
            tr[node].son[0] = parts.first;
            tr[node].son[1] = parts.second;
            pushup(node);
            return node;
        }
        int v = clone_node(u);
        int direction = tr[node].value > tr[v].value;
        int child = tr[v].son[direction];
        int inserted = insert_node(child, node);
        tr[v].son[direction] = inserted;
        pushup(v);
        return v;
    }

    int erase_node(int u, int value) {
        if (tr[u].value == value) {
            return merge(tr[u].son[0], tr[u].son[1]);
        }
        int v = clone_node(u);
        int direction = value > tr[v].value;
        int child = tr[v].son[direction];
        int erased = erase_node(child, value);
        tr[v].son[direction] = erased;
        pushup(v);
        return v;
    }

    int copy_version(int version) {
        assert(0 <= version && version < static_cast<int>(root.size()));
        root.push_back(root[version]);
        return static_cast<int>(root.size()) - 1;
    }

    int insert(int version, int value) {
        assert(0 <= version && version < static_cast<int>(root.size()));
        int new_root;
        if (contains(root[version], value)) {
            new_root = change_count(root[version], value, 1);
        } else {
            new_root = insert_node(root[version], new_node(value));
        }
        root.push_back(new_root);
        return static_cast<int>(root.size()) - 1;
    }

    int erase(int version, int value) {
        assert(0 <= version && version < static_cast<int>(root.size()));
        int new_root = root[version];
        if (contains(new_root, value)) {
            int u = new_root;
            while (tr[u].value != value) {
                u = tr[u].son[value > tr[u].value];
            }
            if (tr[u].count > 1) {
                new_root = change_count(new_root, value, -1);
            } else {
                new_root = erase_node(new_root, value);
            }
        }
        root.push_back(new_root);
        return static_cast<int>(root.size()) - 1;
    }

    int size(int version) const {
        assert(0 <= version && version < static_cast<int>(root.size()));
        return tr[root[version]].size;
    }

    int rank(int version, int value) const {
        assert(0 <= version && version < static_cast<int>(root.size()));
        int u = root[version], res = 1;
        while (u != 0) {
            if (value <= tr[u].value) {
                u = tr[u].son[0];
            } else {
                res += tr[tr[u].son[0]].size + tr[u].count;
                u = tr[u].son[1];
            }
        }
        return res;
    }

    int kth(int version, int k) const {
        assert(0 <= version && version < static_cast<int>(root.size()));
        assert(1 <= k && k <= tr[root[version]].size);
        int u = root[version];
        while (u != 0) {
            int left_size = tr[tr[u].son[0]].size;
            if (k <= left_size) {
                u = tr[u].son[0];
            } else if (k <= left_size + tr[u].count) {
                return tr[u].value;
            } else {
                k -= left_size + tr[u].count;
                u = tr[u].son[1];
            }
        }
        assert(false);
        return 0;
    }

    int predecessor(int version, int value) const {
        assert(0 <= version && version < static_cast<int>(root.size()));
        int u = root[version], res = 0;
        while (u != 0) {
            if (tr[u].value < value) {
                res = u;
                u = tr[u].son[1];
            } else {
                u = tr[u].son[0];
            }
        }
        // 与 P3835 约定一致：不存在前驱时返回 -2^31+1。
        return res == 0 ? numeric_limits<int>::min() + 1 : tr[res].value;
    }

    int successor(int version, int value) const {
        assert(0 <= version && version < static_cast<int>(root.size()));
        int u = root[version], res = 0;
        while (u != 0) {
            if (tr[u].value > value) {
                res = u;
                u = tr[u].son[0];
            } else {
                u = tr[u].son[1];
            }
        }
        // 与 P3835 约定一致：不存在后继时返回 2^31-1。
        return res == 0 ? numeric_limits<int>::max() : tr[res].value;
    }
};

void solve() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    PersistentFHQTreap tree;
    tree.init(n);
    for (int i = 1; i <= n; i++) {
        int v, opt, x;
        cin >> v >> opt >> x;

        int current_version;
        if (opt == 1) {
            current_version = tree.insert(v, x);
        } else if (opt == 2) {
            current_version = tree.erase(v, x);
        } else {
            // 查询不改变树，但仍然必须生成版本 i。
            current_version = tree.copy_version(v);
            if (opt == 3) {
                cout << tree.rank(current_version, x) << '\n';
            } else if (opt == 4) {
                cout << tree.kth(current_version, x) << '\n';
            } else if (opt == 5) {
                cout << tree.predecessor(current_version, x) << '\n';
            } else {
                cout << tree.successor(current_version, x) << '\n';
            }
        }
        // 每次操作恰好生成一个新版本，因此 current_version == i。
    }
}

int main() {
    solve();
    return 0;
}