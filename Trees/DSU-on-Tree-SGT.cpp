#include <bits/stdc++.h>
using namespace std;

#define int long long
#define endl "\n"
#define all(x) x.begin(), x.end()

void files()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
}
const int mod = 1e9 + 7;
int power(int a, int b)
{
    int res = 1;
    while (b)
    {
        if (b & 1)
            res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

int mul(int a, int b)
{
    return (a * b) % mod;
}
int inv(int a)
{
    return power(a, mod - 2);
}
int dive(int a, int b)
{
    return mul(a, inv(b));
}

struct fin
{
    vector<int> data;
    int n;
    fin(int n) : n(n)
    {
        data.assign(n + 1, 1);
    }

    int _mul(int r)
    {
        int res = 1;

        while (r >= 0)
        {
            res = mul(res, data[r]);
            r = (r & (r + 1)) - 1;
        }
        return res;
    }
    int get(int l, int r)
    {
        return dive(_mul(r), _mul(l - 1));
    }

    void add(int idx, int val)
    {
        while (idx <= n)
        {
            data[idx] = mul(data[idx], val);
            idx |= idx + 1;
        }
    }

    void set(int idx, int val)
    {
        add(idx, dive(val, get(idx, idx)));
    }
};

fin f(100100ll);

struct sack
{

    vector<vector<int>> adj;
    vector<int> val, w;
    vector<int> sz, bigest;
    // queries[u] = vector of (l,r, idx)
    vector<vector<tuple<int, int, int>>> queries;
    vector<int> ans;

    sack(int n, int q) : adj(1 + n), sz(1 + n), queries(1 + n), ans(1 + q), bigest(1 + n)
    {
    }
    void set_a(vector<int> &val, vector<int> &w)
    {
        this->val = val, this->w = w;
    }
    void add(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void add_query(int u, int l, int r, int idx)
    {
        queries[u].push_back({l, r, idx});
    }

    void print_ans()
    {

        for (int i = 1; i < (int)ans.size(); i++)
            cout << ans[i] << endl;
    }
    void dfs_sz(int u, int p)
    {
        sz[u] = 1;
        for (auto v : adj[u])
        {
            if (v == p)
                continue;
            dfs_sz(v, u);

            sz[u] += sz[v];
            if (sz[v] > sz[bigest[u]])
                bigest[u] = v;
        }
    }

    void add_sub(int u, int p, bool remove = false)
    {
        if (!remove)
            f.add(w[u], val[u]);
        else
            f.add(w[u], inv(val[u]));

        for (auto v : adj[u])
        {
            if (v == p)
                continue;
            add_sub(v, u, remove);
        }
    }

    void dfs_ans(int u, int p, bool keeping)
    {

        for (auto v : adj[u])
        {
            if (v == p || v == bigest[u])
                continue;
            dfs_ans(v, u, 0);
        }

        if (bigest[u])
            dfs_ans(bigest[u], u, 1);

        f.add(w[u], val[u]);

        for (auto v : adj[u])
        {
            if (v == p || v == bigest[u])
                continue;
            add_sub(v, u);
        }

        for (auto [l, r, idx] : queries[u])
        {
            ans[idx] = f.get(l, r);
        }

        if (!keeping)
            add_sub(u, p, 1);
    }

    void go(int u)
    {
        dfs_sz(u, 0);
        dfs_ans(u, 0, 0);
    }
};

void solve()
{
    int n, q;
    cin >> n >> q;
    vector<int> val(n + 1), w(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> val[i];
    for (int i = 1; i <= n; i++)
        cin >> w[i];

    sack s(n, q);

    for (int i = 1; i < n; i++)
    {
        int u, v;
        cin >> u >> v;
        s.add(u, v);
    }

    s.set_a(val, w);
    int qi = 1;
    while (q--)
    {
        int u, l, r;
        cin >> u >> l >> r;
        s.add_query(u, l, r, qi++);
    }

    s.go(1);

    s.print_ans();
}

signed main()
{
    files();
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}
