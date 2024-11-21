#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct PrefixSum2D
{

    vector<vector<ll>> prefixSum;
    int n, m;

    PrefixSum2D(vector<vector<ll>> &a)
    {
        n = a.size();
        m = a[0].size();
        prefixSum = vector<vector<ll>>(n + 1, vector<ll>(m + 1, 0));
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                prefixSum[i][j] = a[i - 1][j - 1] + prefixSum[i - 1][j] + prefixSum[i][j - 1] - prefixSum[i - 1][j - 1];
            }
        }
    }

    ll query(int x1, int y1, int x2, int y2)
    {
        return prefixSum[x2][y2] - prefixSum[x1 - 1][y2] - prefixSum[x2][y1 - 1] + prefixSum[x1 - 1][y1 - 1];
    }
};

int main()
{
    vector<vector<ll>> mat = {{1, 2, 3, 4},
                              {5, 3, 8, 1},
                              {4, 6, 7, 5},
                              {2, 4, 8, 9}};

    PrefixSum2D ps(mat);

    cout << ps.query(1, 1, 2, 2) << '\n';

    cout << ps.query(1, 1, 3, 3) << '\n';

    cout << ps.query(2, 2, 3, 3) << '\n';
}