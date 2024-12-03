#include <bits/stdc++.h>

using namespace std;

struct CentroidDecomposition
{

    vector<vector<int>> adj;
    vector<bool> blocked;
    vector<int> sub, level, par;
    int n;
    CentroidDecomposition(int n) : n(n)
    {
        level.resize(n + 10);
        sub.resize(n + 10);
        adj.resize(n + 10);
        par.resize(n + 10);
        blocked.assign(n + 10, false);
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int get_size(int u, int p)
    {
        sub[u] = 1;
        for (int v : adj[u])
        {
            if (v == p or blocked[v])
                continue;

            sub[u] += get_size(v, u);
        }

        return sub[u];
    }

    void build()
    {
        decomp(1);
    }

    int get_centroid(int lim, int u, int p)
    {

        for (int v : adj[u])
        {
            if (v == p or blocked[v])
                continue;

            if (sub[v] > lim)
                return get_centroid(lim, v, u);
        }
        return u;
    }

    void decomp(int u, int p = -1)
    {
        int centroid = get_centroid(get_size(u, -1) / 2, u, -1);
        blocked[centroid] = true;
        par[centroid] = p;

        // Do stuff with centroid

        for (auto &v : adj[centroid])
        {
            if (blocked[v])
                continue;
            decomp(v, centroid);
        }
    }
};