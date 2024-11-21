#include <bits/stdc++.h>
using namespace std;
#define ll long long

// Space Complexity: O(n * log(n))
// Time Complexity: O(n * log(n)) for building the table and O(1) for each query
struct SparseTable // 0-Based Indexing
{
    vector<vector<ll>> st;
    vector<ll> log;
    ll merge(ll a, ll b)
    {
        return max(a, b);
    }
    SparseTable(vector<ll> &a)
    {
        int n = a.size();
        log.resize(n + 1);
        log[1] = 0;
        for (int i = 2; i <= n; i++)
            log[i] = log[i / 2] + 1;
        st.assign(n, vector<ll>(log[n] + 1));
        for (int i = 0; i < n; i++)
            st[i][0] = a[i];
        for (int j = 1; (1 << j) <= n; j++)
        {
            for (int i = 0; i + (1 << j) <= n; i++)
            {
                st[i][j] = merge(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
            }
        }
    }
    ll query(int l, int r)
    {
        int j = log[r - l + 1];
        return merge(st[l][j], st[r - (1 << j) + 1][j]);
    }
};

int main()
{
    vector<ll> a = {1, 3, 2, 5, 4, 6, 7, 8};
    SparseTable st(a);
    cout << st.query(0, 7) << endl;
    cout << st.query(0, 3) << endl;
    cout << st.query(4, 7) << endl;
    return 0;
}