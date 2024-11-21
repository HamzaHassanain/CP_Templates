#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Phi // Euler's Totient Function, Gets the number of integers less than n that are coprime with n
{
    vector<ll> phi;
    Phi(ll n)
    {
        phi.resize(n + 1);
        phi[0] = 0;
        phi[1] = 1;
        for (ll i = 2; i <= n; i++)
        {
            phi[i] = i;
        }
        for (ll i = 2; i <= n; i++)
        {
            if (phi[i] == i)
            {
                for (ll j = i; j <= n; j += i)
                {
                    phi[j] -= phi[j] / i;
                }
            }
        }
    }
    ll operator[](ll i)
    {
        return phi[i];
    }
};