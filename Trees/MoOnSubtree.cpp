#include <bits/stdc++.h>
using namespace std;
const string dl = "\n";
typedef long long ll;

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

    vector<MO_Query> queries;

    vector<int> eularTable, nodeOccs;

    int res = 0;

    MOs(int _n, vector<ll> &_arr, EulerTour &_et, vector<MO_Query> &_queries)
    {
        n = _n;
        q = _queries.size();
        vals = _arr;        // arr is one based
        queries = _queries; // queries are zero based
        eularTable = _et.euler;
        q_ans = vector<ll>(q, -1);
        nodeOccs = vector<int>(n + 1, 0);
    }
    void buildAnswer()
    {

        sort(queries.begin(), queries.end());

        int l = queries[0].l, r = queries[0].l - 1;
        for (int i = 0; i < q; i++)
        {
            while (r < queries[i].r)
                add(++r);
            while (r > queries[i].r)
                remove(r--);
            while (l < queries[i].l)
                remove(l++);
            while (l > queries[i].l)
                add(--l);

            q_ans[queries[i].q_idx] = res;
        }
    };
    void printAnswer()
    {
        for (int i = 0; i < q; i++)
        {
            cout << q_ans[i] << dl;
        }
    }
    void add(int idx)
    {
        idx++; // because euler is 1-based

        int u = eularTable[idx];

        nodeOccs[u]++;
        if (nodeOccs[u] == 2)
        {
            // do your adding logic
        }
    }
    void remove(int idx)
    {
        idx++; // because euler is 1-based

        int u = eularTable[idx];

        nodeOccs[u]++;

        if (nodeOccs[u] == 1)
        {
            // do your removing logic
        }
    }
};

void CP()
{
}
int main()
{

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    CP();
    return 0;
}