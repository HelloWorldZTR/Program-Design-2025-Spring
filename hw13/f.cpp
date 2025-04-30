#include <iostream>
#include <map>

using namespace std;

multimap<int, int> ma;

int main()
{
    cin.tie(0);
    cout.tie(0);

    int n;
    cin >> n;

    ma.insert(make_pair(1000000000,1));
    while (n--)
    {
        int id, power;
        cin >> id >> power;

        auto p = ma.upper_bound(power);
        if (p == ma.end())
        {
            auto p = ma.rbegin();
            int minid = p->second;
            while (p != ma.rbegin() && p->first == prev(p)->first)
            {
                p = prev(p);
                minid = min(minid, p->second);
            }
            cout << id << " " << minid << endl;
        }
        else if (p == ma.begin())
        {
            auto p = ma.begin();
            int minid = p->second;
            while (next(p) != ma.end() && p->first == next(p)->first)
            {
                p = next(p);
                minid = min(minid, p->second);
            }
            cout << id << " " << minid << endl;
        }
        else
        {
            auto pp = prev(p);
            int minid1 = pp->second;
            int minid2 = p->second;
            while (pp != ma.begin() && pp->first == prev(pp)->first)
            {
                pp = prev(pp);
                minid1 = min(minid1, pp->second);
            }
            while (next(p) != ma.end() && p->first == next(p)->first)
            {
                p = next(p);
                minid2 = min(minid2, p->second);
            }
            if (power - pp->first < p->first - power)
            {
                cout << id << " " << minid1 << endl;
            }
            else if (power - pp->first > p->first - power)
            {
                cout << id << " " << minid2 << endl;
            }
            else
            {
                cout << id << " " << min(minid1, minid2) << endl;
            }
        }

        ma.insert(make_pair(power, id));
    }
    return 0;
}