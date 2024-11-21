#include <bits/stdc++.h>
using namespace std;

struct HopeCraftKarp
{
    int NIL = 0;
    int INF = 2e9;

    vector<vector<int>> G;
    int n, m;

    vector<int> match, dist;

    HopeCraftKarp(int n, int m)
    {
        this->n = n;
        this->m = m;
        G.resize(n + 10);
        match.resize(n + m + 10);
        dist.resize(n + m + 10);
    }

    void addEdge(int u, int v)
    {
        G[u].push_back(v);
    }

    bool bfs()
    {
        int i, u, v, len;
        queue<int> Q;
        for (i = 1; i <= n; i++)
        {
            if (match[i] == NIL)
            {
                dist[i] = 0;
                Q.push(i);
            }
            else
                dist[i] = INF;
        }
        dist[NIL] = INF;
        while (!Q.empty())
        {
            u = Q.front();
            Q.pop();
            if (u != NIL)
            {
                len = G[u].size();
                for (i = 0; i < len; i++)
                {
                    v = G[u][i];
                    if (dist[match[v]] == INF)
                    {
                        dist[match[v]] = dist[u] + 1;
                        Q.push(match[v]);
                    }
                }
            }
        }
        return (dist[NIL] != INF);
    }

    bool dfs(int u)
    {
        int i, v, len;
        if (u != NIL)
        {
            len = G[u].size();
            for (i = 0; i < len; i++)
            {
                v = G[u][i];
                if (dist[match[v]] == dist[u] + 1)
                {
                    if (dfs(match[v]))
                    {
                        match[v] = u;
                        match[u] = v;
                        return true;
                    }
                }
            }
            dist[u] = INF;
            return false;
        }
        return true;
    }

    int hopcroft_karp()
    {
        int matching = 0, i;
        while (bfs())
            for (i = 1; i <= n; i++)
                if (match[i] == NIL && dfs(i))
                    matching++;
        return matching;
    }
};