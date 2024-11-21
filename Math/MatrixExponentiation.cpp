#include <bits/stdc++.h>

using namespace std;
#define ll long long

const ll MOD = 1e9 + 7;
ll binpow(ll a, ll b)
{
    if (b < 0)
        return 0;

    ll res = 1;
    while (b > 0)
    {
        if (b & 1)
            res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}
ll mod_inv(ll a)
{
    return binpow(a, MOD - 2);
}

// matrix exponentiation

struct Matrix
{
    vector<vector<ll>> mat;
    ll n, m;
    Matrix(ll n, ll m)
    {
        this->n = n;
        this->m = m;
        mat.resize(n, vector<ll>(m, 0));
    }
    Matrix(vector<vector<ll>> mat)
    {
        this->mat = mat;
        n = mat.size();
        m = mat[0].size();
    }
    Matrix operator*(Matrix &other)
    {
        Matrix product(n, other.m);
        for (ll i = 0; i < n; i++)
        {
            for (ll j = 0; j < other.m; j++)
            {
                for (ll k = 0; k < m; k++)
                {
                    product.mat[i][j] += mat[i][k] * other.mat[k][j];
                    product.mat[i][j] %= MOD;
                }
            }
        }
        return product;
    }
    Matrix operator^(ll p)
    {
        Matrix res(n, m);
        for (ll i = 0; i < n; i++)
            res.mat[i][i] = 1;
        Matrix a = *this;
        while (p)
        {
            if (p & 1)
                res = res * a;
            a = a * a;
            p >>= 1;
        }
        return res;
    }
};

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    // kth fibonacci number in O(log n) using matrix exponentiation

    ll n;

    cin >> n;

    Matrix base(2, 2);
    // 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377
    base.mat[0][0] = 1;
    base.mat[0][1] = 1;
    base.mat[1][0] = 1;
    base.mat[1][1] = 0;

    /*
    [1 1]
    [1 0]
    */

    Matrix res = base ^ n;

    cout << res.mat[0][1] << endl;
}
