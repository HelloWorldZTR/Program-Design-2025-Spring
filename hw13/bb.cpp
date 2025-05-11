#include <stdlib.h>
#include <iostream>
#include <stack>
#include <queue>
#include <sstream>
#include <cmath>
using namespace std;

int n;
stack<double> st;
string cmd;
priority_queue<double, vector<double>, greater<double> > mem;

vector<string> split(const string& s, char sep=' ') {
    vector<string> result;
    int l=0, r=0;
    while (r < s.size()) {
        while (r < s.size() && s[r] == sep) r++;
        l = r;
        while (r < s.size() && s[r] != sep) r++;
        if (l < r) result.push_back(s.substr(l, r - l));
    }
    return result;
}

int main() {
    scanf("%d", &n);
    for(int i=0; i<n; i++) {
        double t;
        scanf("%lf", &t);
        mem.push(t);
    }
    cin.ignore();
    
    while(getline(cin, cmd)) {
        auto tokens = split(cmd);
        for (auto& token : tokens) {
            if (token == "=") break;
            if (token == "+") {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(a + b);
            } else if (token == "-") {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(b - a);
            } else if (token == "*") {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(a * b);
            } else if (token == "/") {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(b / a);
            } else if (token == "^") {
                double exp = st.top(); st.pop();
                double base = st.top(); st.pop();
                st.push(pow(base, exp));
            } else if (token == "e") {
                double log10 = st.top(); st.pop();
                double base = st.top(); st.pop();
                st.push(base * pow(10, log10));
            } else {
                double t = atof(token.c_str());
                st.push(t);
            }
        }
        double res = st.top(); st.pop();
        printf("%.6e\n", res);
        if (!mem.empty()) {
            mem.pop();
            mem.push(res);
        }
    }

    printf("\n");

    int m = mem.size();
    for(int i=1; i<=m; i++) {
        if (i % 10 == 0) {
            printf("%.6e\n", mem.top());
            mem.pop();
        } else if (i == m) {
            printf("%.6e", mem.top());
            mem.pop();
        } else {
            printf("%.6e ", mem.top());
            mem.pop();
        }
    }
    return 0;
}