#include <iostream>
#include <set>

using namespace std;

multiset<int> s;
set<int> seen;

int main() {
    cin.tie(0);
    int n;
    cin >> n;

    string cmd;int op;
    while (n--) {
        cin >> cmd;
        if (cmd == "add") {
            cin >> op;
            s.insert(op);
            seen.insert(op);
            cout << s.count(op) << endl;
        }
        else if (cmd == "del") {
            cin >> op;
            cout << s.count(op) << endl;
            s.erase(op);
        }
        else if (cmd == "ask") {
            cin >> op;
            cout << (seen.find(op) != seen.end()) << " " << s.count(op) << endl;
        }
    }
    return 0;
}