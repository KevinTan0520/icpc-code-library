#include <bits/stdc++.h>
using namespace std;

struct FHQTreap {
    struct Node {
        int son[2], value, count, size;
        unsigned priority;
        Node() {
            son[0] = son[1] = 0;
            value = count = size = 0;
            priority = 0;
        }
    };

    int root;
    vector<Node> tr;
    mt19937 rng;

    void init() {
        root = 0;
        tr.assign(1, Node());
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
        return;
    }

    int new_node(int value) {
        Node node;
        node.value = value;
        node.count = node.size = 1;
        node.priority = rng();
        tr.push_back(node);
        return tr.size() - 1;
    }

    void pushup(int u) {
        tr[u].size = tr[tr[u].son[0]].size
            + tr[tr[u].son[1]].size + tr[u].count;
        return;
    }

    void split(int u, int value, int &x, int &y) {
        if (u == 0) {
            x = y = 0;
            return;
        }
        if (tr[u].value < value) {
            x = u;
            split(tr[u].son[1], value, tr[u].son[1], y);
            pushup(x);
        } else {
            y = u;
            split(tr[u].son[0], value, x, tr[u].son[0]);
            pushup(y);
        }
        return;
    }

    int merge(int x, int y) {
        if (x == 0 || y == 0) return x + y;
        if (tr[x].priority > tr[y].priority) {
            tr[x].son[1] = merge(tr[x].son[1], y);
            pushup(x);
            return x;
        }
        tr[y].son[0] = merge(x, tr[y].son[0]);
        pushup(y);
        return y;
    }

    bool add_count(int u, int value) {
        if (u == 0) return false;
        if (tr[u].value == value) {
            tr[u].count++;
            pushup(u);
            return true;
        }
        int direction = value > tr[u].value;
        bool found = add_count(tr[u].son[direction], value);
        if (found) pushup(u);
        return found;
    }

    void insert(int value) {
        if (add_count(root, value)) return;
        int x, y;
        split(root, value, x, y);
        root = merge(merge(x, new_node(value)), y);
        return;
    }

    void erase(int &u, int value) {
        if (u == 0) return;
        if (tr[u].value == value) {
            if (tr[u].count > 1) {
                tr[u].count--;
                pushup(u);
            } else {
                u = merge(tr[u].son[0], tr[u].son[1]);
            }
            return;
        }
        int direction = value > tr[u].value;
        erase(tr[u].son[direction], value);
        pushup(u);
        return;
    }

    void erase(int value) {
        erase(root, value);
        return;
    }

    int rank(int value) const {
        int u = root, res = 1;
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

    int get_kth(int x) const {
        assert(1 <= x && x <= tr[root].size);
        int u = root;
        while (u != 0) {
            int left_size = tr[tr[u].son[0]].size;
            if (x <= left_size) {
                u = tr[u].son[0];
            } else if (x <= left_size + tr[u].count) {
                return tr[u].value;
            } else {
                x -= left_size + tr[u].count;
                u = tr[u].son[1];
            }
        }
        assert(false);
        return 0;
    }

    int predecessor(int value) const {
        int u = root, res = 0;
        while (u != 0) {
            if (tr[u].value < value) {
                res = u;
                u = tr[u].son[1];
            } else {
                u = tr[u].son[0];
            }
        }
        assert(res != 0);
        return tr[res].value;
    }

    int successor(int value) const {
        int u = root, res = 0;
        while (u != 0) {
            if (tr[u].value > value) {
                res = u;
                u = tr[u].son[0];
            } else {
                u = tr[u].son[1];
            }
        }
        assert(res != 0);
        return tr[res].value;
    }
};

int main() {
    return 0;
}
