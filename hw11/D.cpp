#include <iostream>
using namespace std;
class MyCin
{
// 在此处补充你的代码
    bool isStopped = false;
public:
    operator bool() {
        return !isStopped;
    }
    MyCin& operator >>(int& dest) {
        int t;
        cin >> t;
        if (t==-1) {
            isStopped = true;
        }
        dest = t;
        return *this;
    }
};
int main()
{
    MyCin m;
    int n1,n2;
    while( m >> n1 >> n2) 
        cout  << n1 << " " << n2 << endl;
    return 0;
}