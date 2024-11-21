#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct LinearDiophantine // Gets the solution of ax + by = c
{

    ll X, Y;

    ll a, b, c;
    LinearDiophantine(ll a, ll b, ll c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    ll extendedEuclid(ll a, ll b, ll &x, ll &y)
    {
        if (b == 0)
        {
            x = 1;
            y = 0;
            return a;
        }
        ll x1, y1;
        ll d = extendedEuclid(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return d;
    }

    bool solve()
    {
        ll x0, y0;
        ll g = extendedEuclid(a, b, x0, y0);
        if (c % g)
            return false;
        X = x0 * (c / g);
        Y = y0 * (c / g);
        return true;
    }
    void shiftSolution(ll x, ll y, ll cnt)
    {
        X += cnt * b;
        Y -= cnt * a;
    }
};