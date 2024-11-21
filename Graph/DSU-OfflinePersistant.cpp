#include <bits/stdc++.h>
using namespace std;
const string dl = "\n";
typedef long long ll;
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()

struct DSURollback
{
    vector<int> p, sz;
    vector<pair<int &, int>> history;
    int ConnectedComponents = 0;
    DSURollback() {}
    DSURollback(int n)
    {
        set(n);
    }

    void set(int n)
    {
        p.resize(n + 10);
        sz.resize(n + 10, 1);
        iota(p.begin(), p.end(), 0);
        ConnectedComponents = n;
    }

    int get(int x) { return x == p[x] ? x : get(p[x]); }

    void merge(int a, int b)
    {
        a = get(a);
        b = get(b);

        if (sz[a] < sz[b])
        {
            swap(a, b);
        }

        history.push_back({sz[a], sz[a]});
        history.push_back({p[b], p[b]});

        if (a == b)
        {
            return;
        }

        ConnectedComponents--;
        p[b] = a;
        sz[a] += sz[b];
    }

    int snapshot() { return history.size(); }
    int getans() { return ConnectedComponents; }

    void rollback(int until = -1)
    {
        if (until == -1)
            until = history.size() - 2;
        while (history.size() > until)
        {
            if (history.back().first != history.back().second)
                ConnectedComponents++;
            history.back().first = history.back().second, history.pop_back();
            history.back().first = history.back().second, history.pop_back();
        }
    }
};
struct Query
{
    int type;
    int k, u, v;
};
void solve()
{
    // https://judge.yosupo.jp/problem/persistent_unionfind

    int n, q;
    cin >> n >> q;

    vector<Query> queries(q + 1);

    vector<vector<int>> q_adj(q + 2);

    for (int i = 1; i <= q; i++)
    {
        cin >> queries[i].type >> queries[i].k >> queries[i].u >> queries[i].v;
        ++queries[i].k;
        q_adj[queries[i].k].push_back(i);
    }

    vector<ll> ans(q + 2, 0);

    DSURollback dsu(n);

    function<void(ll)> dfs = [&](int u)
    {
        if (queries[u].type == 0)
        {
            dsu.merge(queries[u].u, queries[u].v);
        }
        else
        {
            ans[u] = dsu.get(queries[u].u) == dsu.get(queries[u].v);
        }

        for (int v : q_adj[u])
        {
            dfs(v);
        }

        if (queries[u].type == 0)
        {
            dsu.rollback();
        }
    };

    dfs(0);

    for (int i = 1; i <= q; i++)
    {
        if (queries[i].type == 1)
        {
            cout << ans[i] << dl;
        }
    }
}
int main()
{

    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

    int tc = 1;

    for (int t = 1; t <= tc; t++)
    {
        solve();
    }
    return 0;
}