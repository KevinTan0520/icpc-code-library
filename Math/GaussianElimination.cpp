#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

enum class LinearSystemStatus {
    NoSolution,          // 无解
    UniqueSolution,      // 唯一解
    InfiniteSolutions   // 无穷多解
};

// a 的每行为 [m 个系数, 常数项]，方程数和未知数个数可以不同。
// 返回方程组状态；唯一解时 x 为答案，无穷解时自由变量取 0 得到一组特解。
LinearSystemStatus gaussian_elimination(vector<vector<double>> a, vector<double> &x) {
    assert(!a.empty() && a[0].size() >= 2);
    int equation_count = static_cast<int>(a.size());
    int variable_count = static_cast<int>(a[0].size()) - 1;
    vector<int> pivot_row(variable_count, -1);
    int row = 0;

    for (int col = 0; col < variable_count && row < equation_count; col++) {
        // 选绝对值最大的主元，减小浮点误差。
        int pivot = row;
        for (int i = row + 1; i < equation_count; i++) {
            if (abs(a[i][col]) > abs(a[pivot][col])) pivot = i;
        }
        if (abs(a[pivot][col]) < EPS) continue;

        swap(a[pivot], a[row]);
        pivot_row[col] = row;
        double divisor = a[row][col];
        for (int j = col; j <= variable_count; j++) a[row][j] /= divisor;

        // 消去该列的其余行，最终得到行最简形。
        for (int i = 0; i < equation_count; i++) {
            if (i == row || abs(a[i][col]) < EPS) continue;
            double factor = a[i][col];
            for (int j = col; j <= variable_count; j++) {
                a[i][j] -= factor * a[row][j];
            }
        }
        row++;
    }

    // 出现 0 = 非零常数的行时，方程组无解。
    for (int i = 0; i < equation_count; i++) {
        bool all_zero = true;
        for (int j = 0; j < variable_count; j++) {
            if (abs(a[i][j]) >= EPS) all_zero = false;
        }
        if (all_zero && abs(a[i][variable_count]) >= EPS) {
            x.clear();
            return LinearSystemStatus::NoSolution;
        }
    }

    // 没有主元的列对应自由变量，保留为 0。
    x.assign(variable_count, 0);
    bool has_free_variable = false;
    for (int col = 0; col < variable_count; col++) {
        if (pivot_row[col] == -1) {
            has_free_variable = true;
        } else {
            x[col] = a[pivot_row[col]][variable_count];
        }
    }
    return has_free_variable
        ? LinearSystemStatus::InfiniteSolutions
        : LinearSystemStatus::UniqueSolution;
}

void solve() {
    int n;
    cin >> n;
    vector<vector<double>> a(n); // 注意a数组大小必须跟方程组增广矩阵大小严格一致
    vector<double> ans(n);
    for (int i = 0; i < n; i++) {
        a[i].assign(n + 1, 0);
        for (int j = 0; j < n + 1; j++) {
            cin >> a[i][j];
        }
    }
    LinearSystemStatus sta = gaussian_elimination(a, ans);
    if (sta == LinearSystemStatus::NoSolution) {
        cout << "-1\n";
    }
    else if (sta == LinearSystemStatus::InfiniteSolutions) {
        cout << "0\n";
    }
    else {
        for (int i = 0; i < n; i++) {
            printf("x%d=%.3lf\n", i, ans[i]);
        }
    }
    return;
}

int main() {
    solve();
    return 0;
}
