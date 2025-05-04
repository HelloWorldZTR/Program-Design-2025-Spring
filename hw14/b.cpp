#include <iostream>
using namespace std;
// 在此处补充你的代码
template <typename f, typename g, typename T>
auto combine(f f1, g g1) {
    return [=](T x) {
        return f1(f1(x) + g1(x));
    };
}
int main()
{
    auto Square = [] (double a) { return a * a; };
    auto Inc = [] (double a) { return a + 1; };
    cout << combine<decltype(Square),decltype(Inc),int>(Square,Inc)(3) << endl;
    cout << combine<decltype(Inc),decltype(Square),double>(Inc,Square)(2.5) << endl;

    return 0;
}