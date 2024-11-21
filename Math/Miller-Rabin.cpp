#include <bits/stdc++.h>

using namespace std;
#define ll long long

struct MillerRabin // Big Prime Number Test
{

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
};

int main()
{

    MillerRabin mr;
    cout << mr.isPrime(1000000007) << endl; // 1
    cout << mr.isPrime(1000000009) << endl; // 1
    cout << mr.isPrime(1000000021) << endl; // 1
    cout << mr.isPrime(1000000033) << endl; // 1
    cout << mr.isPrime(1000000000) << endl; // 0
    cout << mr.isPrime(1000000001) << endl; // 0
    cout << mr.isPrime(1000000002) << endl; // 0
    cout << mr.isPrime(1000000003) << endl; // 0

    cout << mr.isPrime(729644203597) << endl; // 1
    return 0;
}