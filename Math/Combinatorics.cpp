#include <bits/stdc++.h>
using namespace std;

#define MOD 1000000007
#define ll long long

ll power(ll a, ll b)
{
    ll res = 1;
    while (b)
    {
        if (b & 1)
            res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

struct Combinatorics
{
    vector<ll> fact, inv;

    Combinatorics(int n)
    {
        fact.resize(n + 1);
        inv.resize(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; i++)
            fact[i] = (fact[i - 1] * i) % MOD;
        inv[n] = power(fact[n], MOD - 2);
        for (int i = n - 1; i >= 0; i--)
            inv[i] = (inv[i + 1] * (i + 1)) % MOD;
    }

    ll C(int n, int k) // Number of ways to choose k elements from n elements without order
    {
        if (k < 0 or k > n)
            return 0;
        return (fact[n] * inv[k] % MOD) * inv[n - k] % MOD;
    }

    ll P(int n, int k) // Number of ways to choose k elements from n elements with order
    {
        if (k < 0 or k > n)
            return 0;
        return (fact[n] * inv[n - k]) % MOD;
    }

    ll H(int n, int k) // Number of ways to put n indistinguishable balls into k distinguishable boxes
    {
        if (n == 0 and k == 0)
            return 1;
        return C(n + k - 1, k);
    }

    ll catalan(int n) // Number of ways to put n pairs of parentheses correctly
    {
        return C(2 * n, n) * inv[n + 1] % MOD;
    }

    ll derangement(int n) // Number of permutations of n elements such that no element appears in its original position
    {
        ll res = 0;
        for (int i = 0; i <= n; i++)
        {
            ll x = C(n, i) * power(-1, n - i);
            if (i & 1)
                res = (res - x + MOD) % MOD;
            else
                res = (res + x) % MOD;
        }
        return res;
    }

    ll stirling1(int n, int k) // Number of ways to partition n elements into k non-empty cycles
    {
        ll res = 0;
        for (int i = 0; i <= k; i++)
        {
            ll x = C(k, i) * power(k - i, n);
            if ((k - i) & 1)
                res = (res - x + MOD) % MOD;
            else
                res = (res + x) % MOD;
        }
        return res * inv[k] % MOD;
    }

    ll stirling2(int n, int k) // Number of ways to partition n elements into k non-empty sets
    {
        ll res = 0;
        for (int i = 0; i <= k; i++)
        {
            ll x = C(k, i) * power(-1, k - i) * power(i, n);
            if ((k - i) & 1)
                res = (res - x + MOD) % MOD;
            else
                res = (res + x) % MOD;
        }
        return res * inv[k] % MOD;
    }

    ll bell(int n) // Number of ways to partition n elements into any number of non-empty sets
    {
        ll res = 0;
        for (int i = 0; i <= n; i++)
        {
            ll x = C(n, i) * bell(n - i);
            if (i & 1)
                res = (res - x + MOD) % MOD;
            else
                res = (res + x) % MOD;
        }
        return res;
    }

    ll lucas(int n, int k) // Number of ways to choose k elements from n elements with order modulo MOD
    {
        if (k == 0)
            return 1;
        return (C(n % MOD, k % MOD) * lucas(n / MOD, k / MOD)) % MOD;
    }
};