#include <bits/stdc++.h>
using namespace std;

#define ll long long

struct EulerTour
{
    vector<vector<int>> adj;
    vector<int> in, out;
    vector<int> euler = {0};
    int timer = 1;

    EulerTour()
    {
    }
    EulerTour(vector<vector<int>> &adj)
    {
        this->adj = adj;
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