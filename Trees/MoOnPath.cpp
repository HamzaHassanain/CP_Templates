#include <bits/stdc++.h>
using namespace std;
const string dl = "\n";
typedef long long ll;
#define all(v) v.begin(), v.end()

struct EulerTour
{
    vector<vector<int>> adj;
    vector<int> in, out;
    vector<int> euler;
    int timer = 1;

    EulerTour(vector<vector<int>> &adj)
    {
        this->adj = adj;
        euler.push_back(0);
        in.resize(adj.size());
        out.resize(adj.size());
    }
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
    }

    void build(int node, int par)
    {
        in[node] = timer++;
        euler.push_back(node);
        for (int child : adj[node])
        {
            if (child != par)
            {
                build(child, node);
            }
        }
        euler.push_back(node);
        out[node] = timer++;
    }
};

struct LCA
{

    vector<vector<int>> adj;
    vector<vector<int>> up;
    vector<int> depth;
    int LOG;
    int N;

    LCA(vector<vector<int>> &adj)
    {
        this->adj = adj;
        N = adj.size();
        LOG = 32 - __builtin_clz(N);
        up.assign(N, vector<int>(LOG));
        depth.assign(N, 0);
    }

    void dfs(int u, int p)
    {
        up[u][0] = p;
        for (int i = 1; i < LOG; i++)
        {
            up[u][i] = up[up[u][i - 1]][i - 1];
        }
        for (int v : adj[u])
        {
            if (v != p)
            {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        }
    }

    int lca(int u, int v)
    {
        if (depth[u] < depth[v])
            swap(u, v);
        for (int i = LOG - 1; i >= 0; i--)
        {
            if (depth[up[u][i]] >= depth[v])
            {
                u = up[u][i];
            }
        }
        if (u == v)
            return u;
        for (int i = LOG - 1; i >= 0; i--)
        {
            if (up[u][i] != up[v][i])
            {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    int dist(int u, int v)
    {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
};
const int MAXN = 40000 + 5;
int SQ;

struct MO_Query
{
    int l, r, q_idx, block_idx;
    int k, lca;
    MO_Query() {}
    MO_Query(int _l, int _r, int __k, int __lca, int _q_idx)
    {
        k = __k, lca = __lca;
        l = _l, r = _r, q_idx = _q_idx, block_idx = _l / SQ;
    }
    friend bool operator<(const MO_Query &a, const MO_Query &b)
    {
        if (a.block_idx == b.block_idx)
        {
            if (a.block_idx & 1)
                return a.r < b.r;
            else
                return a.r > b.r;
        }

        return a.l < b.l;
    }
};

struct MOs
{
    int n, q;
    vector<ll> vals, q_ans;
    // long long res = 0;
    vector<MO_Query> queries;
    vector<int> eularTable;
    vector<bool> nodeOccs;
    vector<ll> cnts;
    int res = 0;
    MOs(int _n, vector<ll> &_arr, EulerTour &_et, vector<MO_Query> &_queries)
    {
        n = _n;
        q = _queries.size();
        vals = _arr;        // arr is one based
        queries = _queries; // queries are zero based
        eularTable = _et.euler;
        q_ans = vector<ll>(q, -1);
        nodeOccs = vector<bool>(n + 1, 0);
        cnts = vector<ll>(_arr.size() + 10);
    }
    void buildAnswer()
    {

        sort(queries.begin(), queries.end());

        int l = queries[0].l, r = queries[0].l - 1;
        for (int i = 0; i < q; i++)
        {
            while (r < queries[i].r)
                update(++r);
            while (r > queries[i].r)
                update(r--);
            while (l < queries[i].l)
                update(l++);
            while (l > queries[i].l)
                update(--l);
            int lca = queries[i].lca;

            if (lca != -1)
                update(lca);
            q_ans[queries[i].q_idx] = res;
            if (lca != -1)
                update(lca);
        }
    };
    void printAnswer()
    {
        for (int i = 0; i < q; i++)
        {
            cout << q_ans[i] << dl;
        }
    }
    void update(int idx)
    {
        int u = eularTable[idx];
        int val = vals[u];

        if (not nodeOccs[u] and ++cnts[val] == 1)
        {
            res++;
        }
        else if (nodeOccs[u] and --cnts[val] == 0)
        {
            res--;
        }
        nodeOccs[u] = !nodeOccs[u];
    }
};

void CP()
{
    // Really IDK what problem was this :)

    int n, q;
    cin >> n >> q;
    vector<vector<int>> adj(n + 1);

    vector<ll> vals(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> vals[i];

    for (int i = 1; i < n; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[v].push_back(u);
        adj[u].push_back(v);
    }

    SQ = sqrt(n) + 1;
    vector<ll> cc = vals;
    sort(all(cc));
    cc.resize(unique(all(cc)) - cc.begin());
    for (int i = 1; i <= n; i++)
    {
        vals[i] = lower_bound(all(cc), vals[i]) - cc.begin();
    }

    EulerTour et(adj);
    et.build(1, 0);

    LCA bl(adj);
    bl.dfs(1, 1);
    vector<MO_Query> queries;
    for (int idx = 0; idx < q; idx++)
    {
        int u, v;
        cin >> u >> v;
        int lca = bl.lca(u, v);
        int k = -1;
        if (et.in[u] > et.in[v])
            swap(u, v);
        if (lca == u)
        {
            queries.push_back(MO_Query(et.in[u], et.in[v], k, -1, idx));
        }
        else
        {
            queries.push_back(MO_Query(et.out[u], et.in[v], k, et.in[lca], idx));
        }
    }

    MOs mo(n, vals, et, queries);
    mo.buildAnswer();
    mo.printAnswer();
}

int main()
{

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    CP();
    return 0;
}