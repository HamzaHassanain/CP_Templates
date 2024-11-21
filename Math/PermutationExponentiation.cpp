#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct PermutationExponentiation
{

    ll MOD;
    PermutationExponentiation(ll MOD)
    {
        this->MOD = MOD;
    }

    vector<ll> mul(vector<ll> &sequence, vector<ll> &permutation)
    {

        vector<ll> newSequence(sequence.size());

        for (int i = 0; i < sequence.size(); i++)
        {
            newSequence[i] = sequence[permutation[i]];
        }
        return newSequence;
    }
    vector<ll> exp(vector<ll> &sequence, vector<ll> &permutation, ll b)
    {

        while (b > 0)
        {
            if (b & 1)
            {
                sequence = mul(sequence, permutation);
            }
            permutation = mul(permutation, permutation);
            b >>= 1;
        }
        return sequence;
    }
};

// PermutationExponentiation