#include <iostream>
using namespace std;
class Sample {
public:
	int v;
    Sample(int n=0) {
        v = n;
    }
    Sample(const Sample & _b) {
        if(_b.v == 5) v=-1;
        if(_b.v == -1) v=9;
        if(_b.v == 20) v=22;
    }
};
void PrintAndDouble(Sample o)
{
	cout << o.v;
	cout << endl;
}
int main()
{
	Sample a(5);
	Sample b = a;
	PrintAndDouble(b);
	Sample c = 20;
	PrintAndDouble(c);
	Sample d;
	d = a;
	cout << d.v;
	return 0;
}