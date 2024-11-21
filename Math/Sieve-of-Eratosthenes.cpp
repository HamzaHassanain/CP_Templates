#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct Sieve
{
    vector<ll> primes;
    vector<bool> isPrime;

    Sieve(ll n)
    {
        isPrime.resize(n + 1, true);
        isPrime[0] = isPrime[1] = false;
        for (ll i = 2; i <= n; i++)
        {
            if (isPrime[i])
            {
                primes.push_back(i);
                for (ll j = i * i; j <= n; j += i)
                {
                    isPrime[j] = false;
                }
            }
        }
    }

    bool operator[](ll n)
    {
        return isPrime[n];
    }
};