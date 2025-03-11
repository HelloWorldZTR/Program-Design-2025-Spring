#include <iostream> 
#include <cstring> 
#include <cstdlib> 
#include <cstdio> 
using namespace std;
const int MAX = 110; 
class CHugeInt {
    private:
        int d[MAX];
        int hi;
        void _add(CHugeInt& b) {
            int l1 = this->hi, l2 = b.hi;
            this->hi = max(l1, l2);

            int overflow = 0;
            for(int i=0; i<this->hi; i++) {
                if(i>=l1)  d[i] = b.d[i] + overflow;
                else if(i>=l2)  d[i] += overflow;
                else  d[i] += b.d[i] + overflow;
                overflow = d[i] / 10;
                d[i] %= 10;
            }

            if(overflow) {
                d[this->hi] = overflow;
                this->hi++;
            }
        }
    public:
        CHugeInt(){
            hi = 0;
        }
        CHugeInt(const CHugeInt& b) {
            hi = b.hi;
            memcpy(d, b.d, sizeof(d));
        }
        CHugeInt(char *s) {
            for(hi=0; ;hi++) {
                if(!s[hi]) break;
                d[hi] = s[hi] - '0';
            }
            for(int i=hi-1, j=0; i>j; i--, j++) {
                swap(d[i], d[j]);
            }
        }
        CHugeInt(long a) {
            hi = 0;
            while(a) {
                d[hi] = a%10;
                a/=10;
                hi++;
            }
        }
        CHugeInt operator+(CHugeInt & b) {
            CHugeInt temp(*this);
            temp._add(b);
            return temp;
        }
        CHugeInt& operator++() {
            CHugeInt one(1);
            _add(one);
            return *this;
        }
        CHugeInt operator++(int) {
            CHugeInt temp(*this);
            CHugeInt one(1);
            _add(one);
            return temp;
        }
        CHugeInt& operator+=(int b) {
            CHugeInt temp(b);
            _add(temp);
            return *this;
        }
        friend ostream& operator<<(ostream& out, const CHugeInt& a) {
            for(int i=a.hi-1; i>=0; i--) {
                out<<a.d[i];
            }
            return out;
        }
        friend CHugeInt operator+(int a, CHugeInt& b) {
            CHugeInt temp(a);
            temp._add(b);
            return temp;
        }
        friend CHugeInt operator+(CHugeInt& a, int b) {
            CHugeInt temp(b);
            temp._add(a);
            return temp;
        }
};
int  main() 
{ 
	char s[210];
	int n;

	while (cin >> s >> n) {
		CHugeInt a(s);
		CHugeInt b(n);

		cout << a + b << endl;
		cout << n + a << endl;
		cout << a + n << endl;
		b += n;
		cout  << ++ b << endl;
		cout << b++ << endl;
		cout << b << endl;
	}
	return 0;
}