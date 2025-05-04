#include <iostream>
#include <cstring>
using namespace std;
template <int bitNum>
struct MyBitset 
{
	char a[bitNum/8+1];
	MyBitset() { memset(a,0,sizeof(a));};
	void Set(int i,int v) {
		char & c = a[i/8];
		int bp = i % 8;
		if( v ) 
			c |= (1 << bp);
		else 
			c &= ~(1 << bp);
	}
// 在此处补充你的代码
    class Proxy{
        int pos;
        MyBitset* parent;

        public:
        Proxy(MyBitset* parent, int pos):parent(parent), pos(pos){}
        int getVal() const{
            char &c = parent->a[pos/8];
            int bp = pos%8;
            return (c & (1 << bp)) >> bp;
        }
        Proxy operator=(const Proxy other) {
            int v = other.getVal();
            parent->Set(pos, v);
            return *this;
        }
        Proxy operator=(const int v) {
            parent->Set(pos, v);
            return *this;
        }
        friend ostream& operator<<(ostream& out, Proxy b) {
            out << b.getVal();
            return out;
        }
    };

    Proxy operator[](int i) {
        return Proxy(this, i);
        
    }
    void Print() {
		for(int i = 0;i < bitNum; ++i) 
			cout << (*this) [i];
		cout << endl;
	}

};

int main()
{
	int n;
	int i,j,k,v;
	while( cin >>  n) {
		MyBitset<20> bs;
		for(int i = 0;i < n; ++i) {
			int t;
			cin >> t;
			bs.Set(t,1);
		}
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		bs[i] = bs[j] = bs[k];
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		(bs[i] = bs[j]) = bs[k];
		bs.Print();
	}
	return 0;
}