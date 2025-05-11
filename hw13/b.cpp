#include <iostream>
#include <queue>
#include <cmath>
#include <vector>
#include <sstream>
#include <stack>
#include <cstring>
#include <set>
#include <cstdio>

using namespace std;

priority_queue<double, vector<double>, greater<double>> memory;
// multiset<double> memory;
char cmd[200];
stack<double> st;


int main() {
    cin.tie(0);
    cout.tie(0);

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        double x;
        scanf("%lf", &x);
        memory.push(x);
    }
    cin.ignore();

    
    while(cin.getline(cmd, 200)) {
        for(int i = 0; i<strlen(cmd); i++) {
            if (cmd[i] == ' ') continue;
            if (cmd[i] == '=') break;
            if (cmd[i] == '+') {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(a + b);
            } else if (cmd[i] == '-') {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(b - a);
            } else if (cmd[i] == '*') {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(a * b);
            } else if (cmd[i] == '/') {
                double a = st.top(); st.pop();
                double b = st.top(); st.pop();
                st.push(b / a);
            } else if(cmd[i] == '^'){
                double exp = st.top(); st.pop();
                double base = st.top(); st.pop();
                st.push(pow(base, exp));
            } else if(cmd[i] == 'e'){
                double log10 = st.top(); st.pop();
                double base = st.top(); st.pop();
                st.push(base * pow(10, log10));
            } else {
                double x = 0;
                while (cmd[i] >= '0' && cmd[i] <= '9') {
                    x = x * 10 + (cmd[i] - '0');
                    i++;
                }
                if (cmd[i] == '.') {
                    i++;
                    double d = 1;
                    while (cmd[i] >= '0' && cmd[i] <= '9') {
                        d /= 10;
                        x += (cmd[i] - '0') * d;
                        i++;
                    }
                } 
                st.push(x);
            }
        }
        double res = st.top(); st.pop();
        printf("%.6e\n", res);
        memory.pop();
        memory.push(res);
    }

    printf("\n");
    int m = memory.size();
    for(int i=1; i<=m; i++) {
        if(i%10 == 0) {
            printf("%.6e\n", memory.top());
        }
        else if(i==m) {
            printf("%.6e", memory.top());
        }
        else {
            printf("%.6e ", memory.top());
        }
        memory.pop();
    }
    printf("\n");

    return 0;
}