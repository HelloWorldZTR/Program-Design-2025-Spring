#include <iostream>
using namespace std;
// 在此处补充你的代码
template<class A, class B>
auto f(A a) {
    auto g = [a](B b){return (b+a);};
    return g;
}
int main()
{
   cout << f<int,int>(7)(9) << endl;   //16
   cout << f<string,string> (" hello!")("world")  <<endl; // world hello!
   cout << f<char,string> ('!')("world") << endl;
   
   
   return 0;    //world!
}