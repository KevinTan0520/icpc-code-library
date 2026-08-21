#include <bits/stdc++.h>
using namespace std;

void test() {
    int n = 100;
    for (int i = 1; i <= n; i++) {
        system("./Agen > Atest.in");
        system("./A < Atest.in > A.out");
        system("./Abrute < Atest.in > Abrute.out");
        if (system("fc Abrute.out A.out")) {
            cout << "WA\n";
            return;
        }
    }
    cout << "AC\n";
    return;
}

int main() {
    test();
    return 0;
}