#include <iostream>
using namespace std;
class A {
public:
	int val;
	A(int a) {
        val = a;
    }
    A& GetObj() {
        return *this;
    }
    A(){
        val = 123;
    }

// 在此处补充你的代码
};
int main()
{
	int m,n;
	A a;
	cout << a.val << endl;
	while(cin >> m >> n) {
		a.GetObj() = m;
		cout << a.val << endl;
		a.GetObj() = A(n);
		cout << a.val<< endl;
	}
	return 0;
}