#include <iostream>
#include <string>
using namespace std;
// 在此处补充你的代码
template <class T>
void MyForeach(T* start, T* end, void (*f)(T&)) {
    for (T* p = start; p != end; p++) {
        f(*p);
    }
}
template <class T>
void MyForeach(T* start, T* end, void (*f)(T)) {
    for (T* p = start; p != end; p++) {
        f(*p);
    }
}
void Print(string s)
{
	cout << s;
}
void Inc(int & n)
{
	++ n;
}
string aaray[100];
int a[100];
int main() {
	int m,n;
	while(cin >> m >> n) {
		for(int i = 0;i < m; ++i)
			cin >> aaray[i];
		for(int j = 0; j < n; ++j)
			cin >> a[j];
		MyForeach(aaray,aaray+m,Print);		 
		cout << endl;
		MyForeach(a,a+n,Inc);		 
		for(int i = 0;i < n; ++i)
			cout << a[i] << ",";
		cout << endl;
	}
	return 0;
}