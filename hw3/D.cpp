#include <iostream>
#include <cstring>
using namespace std;

class Array2
{
    // 在此处补充你的代码
    private:
        int *data;
        int w, h;
    public:
        Array2(){
            data = NULL;
            w = 0;
            h = 0;
        }
        Array2(int h, int w) {
            this->w = w;
            this->h = h;
            data = (int*)malloc(sizeof(int)*w*h);
        }
        ~Array2() {
            free(data);
        }
        int operator()(int x, int y) {
            if(x<0 || x>=h || y<0 || y>=w)  return 0;
            return *(data + x*w + y);
        }
        int* operator[](int x) {
            return data + x*w;
        }
        Array2& operator=(Array2& b) {
            if(this->data == b.data)    return *this;
            if(this->data)  free(this->data);
            this->w = b.w;
            this->h = b.h;
            this->data = (int*)malloc(sizeof(int)*w*h);
            memcpy(this->data, b.data, sizeof(int)*w*h);
            return *this;
        }
        Array2(Array2& b) {
            this->w = b.w;
            this->h = b.h;
            if(!b.data) this->data = NULL;
            else {
                this->data = (int*)malloc(sizeof(int)*w*h);
                memcpy(this->data, b.data, sizeof(int)*w*h);
            }
        }
};

int main()
{
    Array2 a(3, 4);
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 4; j++)
            a[i][j] = i * 4 + j;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 4; j++)
        {
            cout << a(i, j) << ",";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Array2 b;
    b = a;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 4; j++)
        {
            cout << b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}