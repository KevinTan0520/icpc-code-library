#include <bits/stdc++.h>
using namespace std;

// 主席树：查询静态数组区间 [l,r] 的第 k 小，建树 O(n log n)，查询 O(log n)。
// 用法：init({0, a1, ..., an}) -> range_kth(l, r, k)，所有下标从 1 开始。
struct PersistentSegmentTree {
    struct Node {
        int left = 0, right = 0, sum = 0;
    };

    int n;
    vector<int> values, root;   // values 为离散值；root[i] 对应前缀 a[1..i]
    vector<Node> tr;

    int update(int previous, int l, int r, int position) {
        int u = static_cast<int>(tr.size());
        // 复制上一版本，只新建从根到 position 的一条链。
        tr.push_back(tr[previous]);
        tr[u].sum++;
        if (l == r) return u;
        int mid = (l + r) / 2;
        if (position <= mid) {
            tr[u].left = update(tr[previous].left, l, mid, position);
        } else {
            tr[u].right = update(tr[previous].right, mid + 1, r, position);
        }
        return u;
    }

    int kth_rank(int left_root, int right_root, int k, int l, int r) const {
        if (l == r) return l;
        // root[r] - root[l-1] 得到区间 [l,r] 内各值的出现次数。
        int left_count = tr[tr[right_root].left].sum - tr[tr[left_root].left].sum;
        int mid = (l + r) / 2;
        if (k <= left_count) {
            return kth_rank(tr[left_root].left, tr[right_root].left, k, l, mid);
        }
        return kth_rank(
            tr[left_root].right, tr[right_root].right,
            k - left_count, mid + 1, r
        );
    }

    // 输入数组 a 从 1 开始，函数内部完成离散化并建立所有前缀版本。
    void init(const vector<int> &a) {
        n = static_cast<int>(a.size()) - 1;
        values.assign(a.begin() + 1, a.end());
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());

        tr.clear();
        tr.push_back({});
        root.assign(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            int position = static_cast<int>(
                lower_bound(values.begin(), values.end(), a[i]) - values.begin()
            );
            root[i] = update(
                root[i - 1], 0, static_cast<int>(values.size()) - 1, position
            );
        }
    }

    // 返回 a[l..r] 中第 k 小的原值，而非离散化后的排名。
    int range_kth(int l, int r, int k) const {
        assert(1 <= l && l <= r && r <= n);
        assert(1 <= k && k <= r - l + 1);
        int rank = kth_rank(
            root[l - 1], root[r], k, 0, static_cast<int>(values.size()) - 1
        );
        return values[rank];
    }
};

int main() {
    return 0;
}
