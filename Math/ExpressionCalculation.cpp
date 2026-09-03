#include<bits/stdc++.h>
using namespace std;

double calc_expression(string &s) {
    string expr = s; 
    size_t pos = 0;

    function<void()> skipWhitespace = [&]() {
        while (pos < expr.size() && isspace(expr[pos])) ++pos;
    };

    function<double()> parseNumber;
    function<double()> parseFactor;
    function<double()> parseTerm;
    function<double()> parseExpr;

    parseNumber = [&]() -> double {
        size_t start = pos;
        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.')) ++pos;
        string num = expr.substr(start, pos - start);
        if (num.empty()) throw runtime_error("number expected");
        size_t dots = 0;
        for (char c : num) if (c == '.') ++dots;
        if (dots > 1) throw runtime_error("invalid number");
        return stod(num);
    };

    parseFactor = [&]() -> double {
        skipWhitespace();
        if (pos >= expr.size()) throw runtime_error("unexpected end");
        char c = expr[pos];
        if (c == '+') { ++pos; return parseFactor(); }
        if (c == '-') { ++pos; return -parseFactor(); }
        if (c == '(') {
            ++pos;
            double val = parseExpr();
            skipWhitespace();
            if (pos >= expr.size() || expr[pos] != ')')
                throw runtime_error("missing ')'");
            ++pos;
            return val;
        }
        if (isdigit(c) || c == '.') return parseNumber();
        throw runtime_error("unexpected character");
    };

    parseTerm = [&]() -> double {
        double val = parseFactor();
        while (true) {
            skipWhitespace();
            if (pos >= expr.size()) break;
            char op = expr[pos];
            if (op == '*' || op == '/') {
                ++pos;
                double rhs = parseFactor();
                if (op == '*') val *= rhs;
                else {
                    if (rhs == 0.0) throw runtime_error("division by zero");
                    val /= rhs;
                }
            } else break;
        }
        return val;
    };

    parseExpr = [&]() -> double {
        double val = parseTerm();
        while (true) {
            skipWhitespace();
            if (pos >= expr.size()) break;
            char op = expr[pos];
            if (op == '+' || op == '-') {
                ++pos;
                double rhs = parseTerm();
                if (op == '+') val += rhs;
                else val -= rhs;
            } else break;
        }
        return val;
    };

    double result = parseExpr();
    skipWhitespace();
    if (pos != expr.size()) throw runtime_error("extra characters");
    return result;
}

bool check_expression(string &s) {
    try {
        calc_expression(s);  // 尝试求值，若抛出异常则非法
        return true;
    } catch (...) {
        return false;
    }
}


int main() {
    string s;
    cin >> s;
    cout << check_expression(s) << endl;
    cout << calc_expression(s) << endl;
    return 0;
}