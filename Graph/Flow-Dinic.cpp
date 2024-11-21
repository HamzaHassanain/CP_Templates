#include <bits/stdc++.h>
using namespace std;
#define dl '\n'
#define ll long long

struct Dinic
{
    struct Edge
    {
        int from, to, capacity, flow;
    };

    vector<vector<int>> adj;
    vector<Edge> edges;
    vector<int> level, ptr;
    int n, m = 0;
    int source, sink;

    Dinic(int n, int source, int sink) : n(n), source(source), sink(sink)
    {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int from, int to, int capacity)
    {
        edges.push_back({from, to, capacity, 0});
        edges.push_back({to, from, 0, 0});
        adj[from].push_back(m);
        adj[to].push_back(m + 1);
        m += 2;
    }

    bool bfs()
    {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(source);
        level[source] = 0;
        while (!q.empty() && level[sink] == -1)
        {
            int u = q.front();
            q.pop();
            for (int id : adj[u])
            {
                int v = edges[id].to;
                if (level[v] == -1 && edges[id].flow < edges[id].capacity)
                {
                    q.push(v);
                    level[v] = level[u] + 1;
                }
            }
        }
        return level[sink] != -1;
    }

    int dfs(int u, int pushed)
    {
        if (pushed == 0)
            return 0;
        if (u == sink)
            return pushed;
        for (int &cid = ptr[u]; cid < (int)adj[u].size(); cid++)
        {
            int id = adj[u][cid];
            int v = edges[id].to;
            if (level[u] + 1 != level[v] || edges[id].flow == edges[id].capacity)
                continue;
            int tr = dfs(v, min(pushed, edges[id].capacity - edges[id].flow));
            if (tr == 0)
                continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    int max_flow()
    {
        int flow = 0;
        while (true)
        {
            if (!bfs())
                break;
            fill(ptr.begin(),
                 ptr.end(), 0);
            while (int pushed = dfs(source, INT_MAX))
                flow += pushed;
        }

        return flow;
    }

    vector<int> min_cut()
    {
        vector<int> ans;
        for (int i = 0; i < n; i++)
        {
            if (level[i] != -1)
                ans.push_back(i);
        }
        return ans;
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int n, m;
    cin >> n >> m;

    int source = 0, sink = n - 1;

    Dinic dinic(n, source, sink);

    while (m--)
    {
        int u, v, c;
        cin >> u >> v >> c;
        u--, v--;
        // dinic.add_edge(u, v, c); // directed graph
    }

    cout << dinic.max_flow() << dl;

    return 0;
}