#include <iostream>
#include <string>
#include <cstring>
using namespace std;
class MyString {
	char * p;
public:
	MyString(const char * s) {
		if( s) {
			p = new char[strlen(s) + 1];
			strcpy(p,s);
		}
		else
			p = NULL;

	}
    ~MyString() { if(p) delete [] p; }
// 在此处补充你的代码
    friend ostream& operator<< (ostream& out, MyString str) {
        out<<str.p;
        return out;
    }
    void Copy(const char * s) {
        delete [] p;
		if(s) {
			p = new char[strlen(s) + 1];
			strcpy(p,s);
		}
		else
			p = NULL;

	}
    MyString& operator=(MyString& s) {
        //Check if is the same
        if(s.p == p)    return *this;

        delete [] p;
        p = new char[strlen(s.p)];
        strcpy(p, s.p);
        return *this;
    }
    MyString& operator=(char* s) {
        delete [] p;
        if(s) {
			p = new char[strlen(s) + 1];
			strcpy(p,s);
		}
		else
			p = NULL;
        return *this;
    }
    MyString(MyString & s) {
        if(s.p) {
            p = new char[strlen(s.p)];
            strcpy(p, s.p);
        }
        else {
            p = NULL;
        }
    }
};



int main()
{
	char w1[200],w2[100];
	while( cin >> w1 >> w2) {
		MyString s1(w1),s2 = s1;
		MyString s3(NULL);
		s3.Copy(w1);
		cout << s1 << "," << s2 << "," << s3 << endl;

		s2 = w2;
		s3 = s2;
		s1 = s3;
		cout << s1 << "," << s2 << "," << s3 << endl;
		
	}
}