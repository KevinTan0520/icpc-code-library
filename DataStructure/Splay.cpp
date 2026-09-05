#include <bits/stdc++.h>
using namespace std;

struct Splay {
    struct Node {
        int son[2], fa, value, count, size;
        Node() {
            son[0] = son[1] = 0;
            fa = value = count = size = 0;
        }
    };

    int root;
    vector<Node> tr;

    void init() {
        root = 0;
        tr.assign(1, Node());
        return;
    }

    int new_node(int value, int fa) {
        Node node;
        node.fa = fa;
        node.value = value;
        node.count = node.size = 1;
        tr.push_back(node);
        return tr.size() - 1;
    }

    void pushup(int u) {
        tr[u].size = tr[tr[u].son[0]].size
            + tr[tr[u].son[1]].size + tr[u].count;
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

    int find(int value) {
        int u = root, last = 0;
        while (u != 0 && tr[u].value != value) {
            last = u;
            u = tr[u].son[value > tr[u].value];
        }
        if (u != 0) splay(u);
        else if (last != 0) splay(last);
        return u;
    }

    void insert(int value) {
        if (root == 0) {
            root = new_node(value, 0);
            return;
        }
        int u = root, fa = 0;
        while (u != 0 && tr[u].value != value) {
            fa = u;
            u = tr[u].son[value > tr[u].value];
        }
        if (u != 0) {
            tr[u].count++;
            pushup(u);
            splay(u);
            return;
        }
        u = new_node(value, fa);
        tr[fa].son[value > tr[fa].value] = u;
        splay(u);
        return;
    }

    void erase(int value) {
        int u = find(value);
        if (u == 0) return;
        if (tr[u].count > 1) {
            tr[u].count--;
            pushup(u);
            return;
        }
        int left = tr[u].son[0], right = tr[u].son[1];
        if (left == 0 || right == 0) {
            root = left + right;
            if (root != 0) tr[root].fa = 0;
            return;
        }
        root = left;
        tr[left].fa = 0;
        int largest = left;
        while (tr[largest].son[1] != 0) largest = tr[largest].son[1];
        splay(largest);
        tr[root].son[1] = right;
        tr[right].fa = root;
        pushup(root);
        return;
    }

    int rank(int value) {
        int u = root, last = 0, res = 1;
        while (u != 0) {
            last = u;
            if (value <= tr[u].value) {
                if (value == tr[u].value) {
                    res += tr[tr[u].son[0]].size;
                    splay(u);
                    return res;
                }
                u = tr[u].son[0];
            } else {
                res += tr[tr[u].son[0]].size + tr[u].count;
                u = tr[u].son[1];
            }
        }
        if (last != 0) splay(last);
        return res;
    }

    int get_kth(int x) {
        assert(1 <= x && x <= tr[root].size);
        int u = root;
        while (u != 0) {
            int left_size = tr[tr[u].son[0]].size;
            if (x <= left_size) {
                u = tr[u].son[0];
            } else if (x <= left_size + tr[u].count) {
                splay(u);
                return tr[u].value;
            } else {
                x -= left_size + tr[u].count;
                u = tr[u].son[1];
            }
        }
        assert(false);
        return 0;
    }

    int predecessor(int value) {
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
        splay(res);
        return tr[res].value;
    }

    int successor(int value) {
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
        splay(res);
        return tr[res].value;
    }
};

int main() {
    return 0;
}
