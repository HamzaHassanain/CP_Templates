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

#define ll long long

struct Node
{
    int val, prf, suf, size;
    Node() : val(0), prf(0), suf(0), size(0) {}
    Node(int val, int prf, int suf, int size) : val(val), prf(prf), suf(suf), size(size) {}
};
struct Operation
{
    virtual Node work(Node, Node) { return Node(); }
};
// sgt for max conseqtuive ones in a range
struct Sgt
{
    int size;
    Node DEFAULT;
    vector<Node> data;
    Operation *operation;
    Sgt(int n, Operation *operation)
    {
        init(n, Node(0, 0, 0, 1), operation);
    }
    void init(int n, Node DEFAULT, Operation *operation)
    {
        size = 1;
        this->operation = operation;
        this->DEFAULT = DEFAULT;
        while (size < n)
            size *= 2;
        data.assign(2 * size, DEFAULT);
    }
    void set(int i, Node value, int node, int beginSeg, int endSeg)
    {
        if (beginSeg == endSeg)
        {
            data[node] = value;
            return;
        }

        int mid = (beginSeg + endSeg) / 2;
        if (i <= mid)
            set(i, value, 2 * node, beginSeg, mid);
        else
            set(i, value, 2 * node + 1, mid + 1, endSeg);

        data[node] = operation->work(data[2 * node], data[2 * node + 1]);
    }

    Node query(int left, int right, int node, int beginSeg, int endSeg)
    {
        if (beginSeg > right || left > endSeg)
            return DEFAULT;

        if (beginSeg >= left && endSeg <= right)
            return data[node];

        int mid = (beginSeg + endSeg) / 2;
        Node a = this->query(left, right, 2 * node, beginSeg, mid);
        Node b = this->query(left, right, 2 * node + 1, mid + 1, endSeg);

        return operation->work(a, b);
    }

    Node query(int left, int right)
    {

        return query(left, right, 1, 1, size);
    }
    void set(int i, Node value)
    {
        set(i, value, 1, 1, size);
    }
};
struct OPS : Operation
{
    Node work(Node a, Node b)
    {
        Node ans;
        ans.size = a.size + b.size;
        ans.val = max({a.val, b.val, a.suf + b.prf});
        ans.prf = (a.prf == a.size ? a.prf + b.prf : a.prf);
        ans.suf = (b.suf == b.size ? b.suf + a.suf : b.suf);
        return ans;
    }
};

Sgt sgt(200100ll, new OPS());
int freq[200100ll];
struct sack
{

    vector<vector<int>> adj;
    vector<int> val;
    vector<int> sz, bigest;
    // queries[u] = vector of (l,r, idx)
    vector<vector<tuple<int, int, int>>> queries;
    vector<int> ans;

    sack(int n, int q) : adj(1 + n), sz(1 + n), queries(1 + n), ans(1 + q), bigest(1 + n)
    {
    }
    void set_a(vector<int> &val)
    {
        this->val = val;
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
        {
            freq[val[u]]++;
            if (freq[val[u]] == 1)
            {
                sgt.set(val[u], Node(1, 1, 1, 1));
            }
        }
        else
        {
            freq[val[u]]--;
            if (!freq[val[u]])
            {
                sgt.set(val[u], Node(0, 0, 0, 1));
            }
        }

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

        // f.add(w[u], val[u]);
        // add
        freq[val[u]]++;
        if (freq[val[u]] == 1)
        {
            sgt.set(val[u], Node(1, 1, 1, 1));
        }
        for (auto v : adj[u])
        {
            if (v == p || v == bigest[u])
                continue;
            add_sub(v, u);
        }

        for (auto [l, r, idx] : queries[u])
        {
            ans[idx] = sgt.query(l, r).val;
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
    vector<int> val(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> val[i];

    sack s(n, q);

    for (int i = 1; i < n; i++)
    {
        int u, v;
        cin >> u >> v;
        s.add(u, v);
    }

    s.set_a(val);
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
    memset(freq, 0, sizeof(freq));
    files();
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
}
