#include <bits/stdc++.h>

using namespace std;
#define ll long long

struct Mobius
{

    vector<ll> mobius;
    vector<ll> primes;

    Mobius(ll n)
    {
        mobius.resize(n + 1, 0);
        mobius[0] = 0;
        mobius[1] = -1;
        for (int i = 1; i < n + 1; i++)
        {
            if (mobius[i])
            {
                mobius[i] = -mobius[i];
                for (int j = 2 * i; j < n + 1; j += i)
                {
                    mobius[j] += mobius[i];
                }
            }
        }
    }

    ll operator[](ll n)
    {
        if (n < 0 or n >= mobius.size())
            return 0;
        return mobius[n];
    }
};

int main()
{
    Mobius m(100);
    // m[x] = 0 if x has a squared prime factor
    // m[x] = (-1)^k if x is a product of k distinct primes

    for (int i = 1; i <= 100; i++)
    {
        cout << i << " " << m[i] << endl;
    }
    cout << endl;
    return 0;
}
