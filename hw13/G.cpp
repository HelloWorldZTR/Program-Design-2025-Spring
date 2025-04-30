#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <set>

using namespace std;

const int N = 10000000;
const int M = 10000000;
int primecount = 0;
int primes[M];
bool is_prime[N];

struct cmp {
    bool operator()(const int a, const int b) const{
        int factor_a = 0, factor_b = 0;
        for(int i=0; i<primecount; i++) {
            if (primes[i] >= a) break;
            if (a % primes[i] == 0) {
                factor_a++;
            }
        }
        for(int i=0; i<primecount; i++) {
            if (primes[i] >= b) break;
            if (b % primes[i] == 0) {
                factor_b++;
            }
        }
        if (factor_a == factor_b) {
            return a>b;
        }
        else {
            return factor_a > factor_b; 
        }
    }
};

// priority_queue<int, vector<int>, cmp> pq;
set<int, cmp> pq;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    fill(is_prime, is_prime + N, true);

    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            primes[primecount++] = i;
        }
        for(int j=0; j<primecount && i*primes[j]<N; j++) {
            is_prime[i*primes[j]] = false;
            if (i % primes[j] == 0) break;
        }
    }


    int num;
    cin >> num;
    while(num --) {
        for(int i=0; i<10; i++) {
            int x;
            cin >> x;
            pq.insert(x);
        }
        cout << *pq.begin() << " " << *pq.rbegin() << endl;
        pq.erase(--pq.end());
        pq.erase(pq.begin());
        
    }
}