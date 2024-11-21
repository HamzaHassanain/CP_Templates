#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll MOD = 998244353;

ll add_mod(ll a, ll b)
{
    return (a + b) % MOD;
}
ll mul_mod(ll a, ll b)
{
    return (a * b) % MOD;
}
ll sub_mod(ll a, ll b)
{
    return (a - b + MOD) % MOD;
}
ll pow_mod(ll a, ll b)
{
    ll res = 1;
    while (b)
    {
        if (b & 1)
        {
            res = mul_mod(res, a);
        }
        a = mul_mod(a, a);
        b >>= 1;
    }
    return res;
}

ll inv_mod(ll a)
{
    return pow_mod(a, MOD - 2);
}
ll div_mod(ll a, ll b)
{
    return mul_mod(a, inv_mod(b));
}

ll log_mod(ll a, ll b)
{
    // a^n = b mod p
    ll n = sqrt(MOD) + 1;
    map<ll, ll> mp;
    ll an = 1;
    for (ll i = 0; i < n; i++)
    {
        if (mp.find(an) == mp.end())
            mp[an] = i;
        an = (an * a) % MOD;
    }
    ll inva = inv_mod(pow_mod(a, n));
    ll cur = b;
    for (ll i = 0; i < n; i++)
    {
        if (mp.find(cur) != mp.end())
            return i * n + mp[cur];
        cur = (cur * inva) % MOD;
    }
    return -1;
}