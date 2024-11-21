#include <bits/stdc++.h>

using namespace std;
#define ll long long

struct PollardsRho
{

    ll gcd(ll a, ll b)
    {
        return b == 0 ? a : gcd(b, a % b);
    }

    ll f(ll x, ll c, ll mod)
    {
        return (x * x + c) % mod;
    }

    ll pollardRho(ll n)
    {
        if (n % 2 == 0)
            return 2;
        ll x = rand() % n + 1;
        ll y = x;
        ll c = rand() % n + 1;
        ll d = 1;
        while (d == 1)
        {
            x = f(x, c, n);
            y = f(f(y, c, n), c, n);
            d = gcd(abs(x - y), n);
        }
        return d;
    }

    vector<ll> factorize(ll n)
    {
        if (n == 1)
            return {};
        if (isPrime(n))
            return {n};
        ll d = pollardRho(n);
        vector<ll> left = factorize(d);
        vector<ll> right = factorize(n / d);
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }

    bool isPrime(ll n)
    {
        if (n < 2)
            return false;
        if (n < 4)
            return true;
        if (n % 2 == 0)
            return false;
        ll d = n - 1;
        while (d % 2 == 0)
            d >>= 1;
        for (int i = 0; i < 10; i++)
        {
            ll a = rand() % (n - 3) + 2;
            ll x = pow(a, d, n);
            if (x == 1 || x == n - 1)
                continue;
            bool flag = false;
            for (ll j = 0; j < 32; j++)
            {
                x = mul(x, x, n);
                if (x == n - 1)
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
                return false;
        }
        return true;
    }

    ll mul(ll a, ll b, ll mod)
    {
        ll res = 0;
        while (b)
        {
            if (b & 1)
                res = (res + a) % mod;
            a = (a + a) % mod;
            b >>= 1;
        }
        return res;
    }

    ll pow(ll a, ll b, ll mod)
    {
        ll res = 1;
        while (b)
        {
            if (b & 1)
                res = mul(res, a, mod);
            a = mul(a, a, mod);
            b >>= 1;
        }
        return res;
    }
};

int main()
{
    PollardsRho p;
    ll n;
    cin >> n;
    vector<ll> factors = p.factorize(n);
    for (ll factor : factors)
        cout << factor << " ";
    cout << endl;
    return 0;
}
