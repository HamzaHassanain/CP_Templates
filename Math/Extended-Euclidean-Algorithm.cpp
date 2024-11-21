#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct ExtendedEuclidean
{
    ll x, y, d;
    ExtendedEuclidean(ll a, ll b)
    {
        if (b == 0)
        {
            x = 1;
            y = 0;
            d = a;
            return;
        }
        ExtendedEuclidean e(b, a % b);
        x = e.y;
        y = e.x - a / b * e.y;
        d = e.d;
    }
};

int main()
{
    // when to use ? when you need to find x and y such that ax + by = gcd(a, b)

    // example for getting mod inverse using Extended Euclidean Algorithm

    ll a = 7, m = 11;

    ExtendedEuclidean e(a, m);

    ll invA = (e.x % m + m) % m; // that is the modular inverse of a mod m

    cout << invA << endl;
}