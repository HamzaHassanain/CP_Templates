#include <bits/stdc++.h>

using namespace std;
#define sz(x) (int)x.size()
#define ll long long

// Time Complexity Per Update: O(log(n) * log(m))
// Time Complexity Per Query: O(log(n) * log(m))
// Space Complexity: O(n * m)

struct FenwickTree2D // 1-Based Indexing
{

    vector<vector<ll>> data;
    int n, m;
    ll DEFAULT;

    FenwickTree2D(int _n, int _m)
    {
        n = _n + 1;
        m = _m + 1;
        DEFAULT = 0;
        data = vector<vector<ll>>(10 + n, vector<ll>(10 + m, DEFAULT));
    }

    int lb(int idx)
    {
        return (idx & -idx);
    }

    void build(vector<vector<ll>> &nums)
    {
        for (int i = 0; i < sz(nums); i++)
            for (int j = 0; j < sz(nums[0]); j++)
                add(i + 1, j + 1, nums[i][j]);
    }

    ll work(ll a, ll b)
    {
        return a + b;
    }

    void add(int idx, int jdx, ll val)
    {
        int i = idx + 1, j = jdx + 1;
        while (i <= n)
        {
            j = jdx + 1;
            while (j <= m)
            {
                data[i][j] = work(data[i][j], val);
                j += lb(j);
            }
            i += lb(i);
        }
    }

    ll sum(int idx, int jdx)
    {
        ll sum = DEFAULT;
        int i = idx + 1, j = jdx + 1;
        while (i)
        {
            j = jdx + 1;
            while (j)
            {
                sum = work(sum, data[i][j]);
                j -= lb(j);
            }
            i -= lb(i);
        }
        return sum;
    }

    ll query(int i, int j, int k, int l)
    {
        if (i > k)
            swap(i, k);
        if (j > l)
            swap(j, l);
        return sum(k, l) - sum(i - 1, l) - sum(k, j - 1) + sum(i - 1, j - 1);
    }
};

int main()
{
    vector<vector<ll>> a = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    FenwickTree2D ft(sz(a), sz(a[0]));

    ft.build(a);

    for (int i = 0; i <= sz(a); i++)
    {
        for (int j = 0; j <= sz(a[0]); j++)
        {
            cout << ft.query(i, j, i, j) << " ";
        }
        cout << endl;
    }

    cout << ft.query(1, 1, 3, 4) << endl; // 78
    cout << ft.query(2, 2, 3, 3) << endl; // 34

    ft.add(2, 2, 10); // increase the value at index (2, 2) by 10

    return 0;
}