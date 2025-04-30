#include <iostream>
#include<list>
#include<map>
#include<set>
#include<vector>
#include <cmath>
#include <cstring>
#include<sstream>
using namespace std;

int main() {
	int n;
	cin >> n;
	string command;
	cin.ignore();
	map<int, list<int>>seq;

	while (n--) {
		string cmd;
		getline(cin, command);
		stringstream ss(command);
		ss >> cmd;

		if (cmd == "new") {
			int id;
			ss >> id;
			seq[id] = list<int>();
		}
		else if (cmd == "add") {
			int id, num;
			ss >> id >> num;
			seq[id].push_back(num);
		}
		else if (cmd == "unique") {
			int id;
			ss >> id;
			seq[id].sort();
			seq[id].unique();
		}
		else if (cmd == "out") {
			int id;
			ss >> id;
            seq[id].sort();
			for (auto it = seq[id].begin(); it != seq[id].end(); ++it) {
				if (it != seq[id].begin())
					cout << ' ';
				cout << (*it);
			}
			cout << endl;
		}
		else {
			int id1, id2;
			ss >> id1 >> id2;
			if (id1 != id2) {
				seq[id1].merge(seq[id2]);
                seq[id2].clear();
			}
		}
	}
	return 0;
}