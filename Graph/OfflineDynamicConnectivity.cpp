#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct DSURollback
{
    vector<int> p, sz;
    // stores previous unites
    vector<pair<int &, int>> history;
    int ConnectedComponents = 0;
    DSURollback() {}
    DSURollback(int n)
    {
        init(n);
    }

    void init(int n)
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

        // Normally We would have done this Here
        // if (a == b)
        // {
        //     return;
        // }

        // save this unite operation
        history.push_back({sz[a], sz[a]});
        history.push_back({p[b], p[b]});

        // For DC Use this here instead of above
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
            until = history.size() - 2; // rollback only one unite operation
        // cout << history.size() << " " << until << dl;
        while (history.size() > until)
        {
            if (history.back().first != history.back().second)
            {
                ConnectedComponents++;
            }
            history.back().first = history.back().second;
            history.pop_back();

            history.back().first = history.back().second;
            history.pop_back();
        }
        // cout << "After rollback: " << history.size() << dl;
        // update connected components
    }
};


struct DynamicConnectivity
{
    int n, m, q;
    int N;
    vector<array<int, 2>> ed;
    vector<vector<int>> adj, seg, add, cut;
    vector<int> ans;

    DSURollback d;

    map<array<int, 2>, int> mp;
    DynamicConnectivity() {}
    DynamicConnectivity(int n, int q) : n(n), q(q)
    {
        N = 1;
        while (N <= q + 5)
            N <<= 1;
        seg.resize(N << 1);
        ans.resize(N);

        adj.resize(n + 1);
        add.resize(q + 1);
        cut.resize(q + 1);

        ed.push_back({0, 0});
    }
    void insert_add_query(int a, int b, int i) // i is the time
    {
        if (b > a)
            swap(a, b);

        if (mp.find({a, b}) == mp.end())
            ed.push_back({a, b}), mp[ed.back()] = ed.size() - 1;
        int idx = mp[{a, b}];
        add[idx].push_back(i);
        
    }
    void insert_remove_query(int a, int b, int i) // i is the time
    {
        if (b > a)
            swap(a, b);

        int idx = mp[{a, b}];
        cut[idx].push_back(i);
    }
    void finalize_cuts()
    {
        for (int i = 1; i < ed.size(); i++)
            cut[i].push_back(q + 1);
    }
    void finalize_adds()
    {
        for (int i = 1; i < ed.size(); i++)
        {
            for (auto it : add[i])
            {
                update(0, N - 1, 0, it, *lower_bound(cut[i].begin(), cut[i].end(), it) - 1, i);
            }
        }
    }

    void work()
    {
        d.init(n);
        build(0, N - 1, 0);
    }

    void update(int l, int r, int node, int lx, int rx, int idx)
    {
        if (r < lx || l > rx)
            return;
        if (l >= lx && r <= rx)
        {
            seg[node].push_back(idx);
            return;
        }
        int mid = l + r >> 1;
        update(l, mid, 2 * node + 1, lx, rx, idx);
        update(mid + 1, r, 2 * node + 2, lx, rx, idx);
    }
    void build(int l, int r, int node)
    {
        for (auto it : seg[node])
        {
            d.merge(ed[it][0], ed[it][1]);
        }
        if (l == r)
        {
            ans[l] = d.getans();
            for (auto it : seg[node])
                d.rollback();
            return;
        }
        int mid = l + r >> 1;
        build(l, mid, 2 * node + 1);
        build(mid + 1, r, 2 * node + 2);
        for (auto it : seg[node])
            d.rollback();
    }
};

signed main()
{

    // https://cses.fi/problemset/task/2133/

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, q;
    cin >> n >> m >> q;
    DynamicConnectivity dc(n, q + m);
    vector<int> queries;

    for (int i = 1; i <= m; i++)
    {
        int a, b;
        cin >> a >> b;
        if (b > a)
            swap(a, b);
        dc.insert_add_query(a, b, i);
    }
    queries.push_back(m);
    for (int i = m + 1; i <= q + m; i++)
    {
        int op;
        cin >> op;
        int a, b;
        cin >> a >> b;
        if (b > a)
            swap(a, b);
        if (op == 1)
        {
            dc.insert_add_query(a, b, i);
        }
        else
        {
            dc.insert_remove_query(a, b, i);
        }

        queries.push_back(i);
    }
    dc.finalize_cuts();
    dc.finalize_adds();

    dc.work();
    for (auto it : queries)
        cout << dc.ans[it] << ' ';
    return 0;
}