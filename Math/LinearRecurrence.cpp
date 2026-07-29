#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
const i64 MOD = 1000000007;

struct Matrix {
    int rows, cols;
    vector<vector<i64>> a;

    Matrix(int rows = 0, int cols = 0) : rows(rows), cols(cols), a(rows, vector<i64>(cols)) {}

    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; i++) res.a[i][i] = 1;
        return res;
    }

    vector<i64> &operator[](int row) {
        return a[row];
    }

    const vector<i64> &operator[](int row) const {
        return a[row];
    }

    Matrix operator*(const Matrix &other) const {
        assert(cols == other.rows);
        Matrix res(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (a[i][k] == 0) continue;
                for (int j = 0; j < other.cols; j++) {
                    res[i][j] = (res[i][j] + (__int128)a[i][k] * other.a[k][j]) % MOD;
                }
            }
        }
        return res;
    }
};

Matrix quick_pow(Matrix base, long long exp) {
    assert(base.rows == base.cols && exp >= 0);
    Matrix res = Matrix::identity(base.rows);
    while (exp > 0) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

// coefficients = {c_1, ..., c_m} 表示 f_n = c_1 f_{n-1} + ... + c_m f_{n-m}。
// initial = {f_0, ..., f_{m-1}}，返回 f_k mod MOD。
i64 linear_recurrence(long long k, const vector<i64> &coefficients, const vector<i64> &initial) {
    assert(k >= 0 && !coefficients.empty() && coefficients.size() == initial.size());
    int order = coefficients.size();
    auto normalize = [](i64 x) {
        x %= MOD;
        if (x < 0) x += MOD;
        return x;
    };

    if (k < order) return normalize(initial[k]);

    Matrix trans(order, order), state(order, 1);
    for (int i = 0; i < order; i++) trans[0][i] = normalize(coefficients[i]);
    for (int i = 1; i < order; i++) trans[i][i - 1] = 1;
    for (int i = 0; i < order; i++) state[i][0] = normalize(initial[order - 1 - i]);

    Matrix result = quick_pow(trans, k - order + 1) * state;
    return result[0][0];
}

/*
二阶例子：已知 f_0 = 1，f_1 = 2，且
    f_n = 2f_{n-1} + 3f_{n-2}，求 f_n。

1. 确定状态
   递推式需要前两项，所以把第 n 项和第 n-1 项一起保存：

       S_n = [f_n, f_{n-1}]^T。

   已知 f_1 和 f_0，因此初始状态取

       S_1 = [f_1, f_0]^T = [2, 1]^T。

2. 确定递推矩阵
   下一状态的第一项由原递推式得到，第二项只是把原来的 f_n 下移：

       [f_{n+1}]   [2 3] [f_n    ]
       [f_n    ] = [1 0] [f_{n-1}]。

   因此递推矩阵 T = [[2, 3], [1, 0]]，并且 S_{n+1} = T * S_n。

3. 得到答案
   从 S_1 走到 S_n 一共转移 n-1 次，所以

       S_n = T^(n-1) * S_1，

   结果矩阵的 [0][0] 就是 f_n。例如 n = 4 时：

       Matrix trans(2, 2), initial(2, 1);
       trans[0][0] = 2; trans[0][1] = 3;
       trans[1][0] = 1; trans[1][1] = 0;
       initial[0][0] = 2;
       initial[1][0] = 1;
       Matrix result = quick_pow(trans, 3) * initial;
       // result[0][0] = f_4 = 61

   若 n = 0，应直接返回初值 f_0。

三阶例子：已知 f_0 = 1，f_1 = 2，f_2 = 3，且
    f_n = 2f_{n-1} + 3f_{n-2} + 4f_{n-3}，求 f_n。

1. 递推式依赖前三项，因此状态和初始状态为

       S_n = [f_n, f_{n-1}, f_{n-2}]^T，
       S_2 = [f_2, f_1, f_0]^T = [3, 2, 1]^T。

2. 第一行填写递推系数 2、3、4，后两行负责把状态依次下移：

       [f_{n+1}]   [2 3 4] [f_n    ]
       [f_n    ] = [1 0 0] [f_{n-1}]
       [f_{n-1}]   [0 1 0] [f_{n-2}]。

   因此 T = [[2, 3, 4], [1, 0, 0], [0, 1, 0]]。

3. 从 S_2 到 S_n 需要转移 n-2 次，所以

       S_n = T^(n-2) * S_2。

   例如求 f_5：

       Matrix trans(3, 3), initial(3, 1);
       trans[0][0] = 2; trans[0][1] = 3; trans[0][2] = 4;
       trans[1][0] = 1;
       trans[2][1] = 1;
       initial[0][0] = 3;
       initial[1][0] = 2;
       initial[2][0] = 1;
       Matrix result = quick_pow(trans, 3) * initial;
       // result[0][0] = f_5 = 158

一般的 k 阶线性递推同理：把连续 k 项放入状态，递推系数写在矩阵第一行，
其余各行用 1 完成状态下移。

上面两个例子也可以直接调用封装函数：

    linear_recurrence(4, {2, 3}, {1, 2});          // f_4 = 61
    linear_recurrence(5, {2, 3, 4}, {1, 2, 3});   // f_5 = 158
*/

int main() {
    return 0;
}
