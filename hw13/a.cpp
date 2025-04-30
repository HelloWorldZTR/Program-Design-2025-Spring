#include <iostream>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

map<int, list<int>> lists;

int main() {
    int n;
    cin >> n;

    string command;
    int a, b;
    while(n--) {
        cin >> command;
        if (command == "new") {
            cin >> a;
            lists.insert(make_pair(a, list<int>()));
        }
        else if(command == "add") {
            cin >> a >> b;
            auto& list = lists[a]; // Use reference
            list.push_back(b);
        }
        else if (command == "merge") {
            cin >> a >> b;
            if(a == b) continue; // Avoid self-merge
            auto& lista = lists[a]; // Use reference
            auto& listb = lists[b]; // Use reference
            lista.merge(listb);
            listb.clear();
        }
        else if(command == "unique") {
            cin >> a;
            auto& list = lists[a]; // Use reference
            list.sort(); // Sort the list before removing duplicates
            list.unique(); // Remove duplicates
        }
        else if(command == "out") {
            cin >> a;
            auto& list = lists[a]; // Use reference
            list.sort();
            if(list.size()) {
                for(auto ptr=list.begin(); ptr!=list.end(); ++ptr) {
                    cout << *ptr << " ";
                }
            }
            cout << endl;
        }
    }
    return 0;
}