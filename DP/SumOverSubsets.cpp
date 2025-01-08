#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Sos
{
    int lg, n;
    vector<int> dp;
    Sos(int lg, vector<int> &a)
    {
        this->lg = lg,
        n = 1ll << lg;

        // do something with a;
        dp = vector<int>(n + 1);
    }

    void addToSuperSets()
    {
        for (int bit = 0; bit < lg; bit++)
        {
            for (int msk = 0; msk < n; msk++)
            {
                if (msk & (1 << bit))
                    dp[msk] += dp[msk ^ (1 << bit)];
            }
        }
    }

    void addToSubSets()
    {
        for (int bit = 0; bit < lg; bit++)
        {
            for (int msk = n - 1; msk >= 0; msk--)
            {
                if (msk & (1 << bit))
                    dp[msk ^ (1 << bit)] += dp[msk];
            }
        }
    }

    void removeFromSuperSets()
    {
        for (int bit = 0; bit < lg; bit++)
        {
            for (int msk = n - 1; msk >= 0; msk--)
            {
                if (msk & (1 << bit))
                    dp[msk] -= dp[msk ^ (1 << bit)];
            }
        }
    }
    void removeFromSubSets()
    {
        for (int bit = 0; bit < lg; bit++)
        {
            for (int msk = 0; msk < n; msk++)
            {
                if (msk & (1 << bit))
                    dp[msk ^ (1 << bit)] -= dp[msk];
            }
        }
    }
};
