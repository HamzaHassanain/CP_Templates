#include <bits/stdc++.h>

using namespace std;

#define ll long long

struct Sack
{

    vector<vector<ll>> adj;
    vector<ll> answer;
    // some data about the nodes
    vector<set<ll>> data;

    Sack(vector<vector<ll>> &adj, ll root = 1)
    {
        this->adj = adj;
        answer.resize(adj.size());
        data.resize(adj.size());

        dfs(1, 0);
    }
    ll merge(ll a, ll b)
    {
        if (data[a].size() < data[b].size())
            swap(data[a], data[b]);

        for (auto &x : data[b])
        {
            // do some magic
        }

        // might change based on the problem
        return a;
    }
    void dfs(ll node, ll parent)
    {

        // might change based on the problem
        ll answerSet = node;

        for (auto &child : adj[node])
        {
            if (child == parent)
                continue;
            dfs(child, node);
            // merge the child into the parent
            answerSet = merge(answerSet, child);
        }

        answer[node] = answerSet;
    }
};
