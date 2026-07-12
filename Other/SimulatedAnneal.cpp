#include <bits/stdc++.h>
using namespace std;

double func(double x) {
    double res = x;
    return res;
}

double simulated_anneal(double eps, double T, double delta) { // eps = 1e-8, T = 100, delta = 0.98
    double now, next;
    while (T > eps) {
        double dE = func(next) - func(now);
        if (dE >= 0) now = next;
        else if (exp(dE / T) > rand()) now = next;
        T *= delta;
    }
    return now;
}

int main() {
    return 0;
}