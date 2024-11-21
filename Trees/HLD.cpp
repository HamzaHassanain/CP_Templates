#include <bits/stdc++.h>
using namespace std;

#define ll long long

template <typename T = int>
using Pair = pair<T, T>;

template <typename T = int, const int VAL_ON_EDGE = 0>
struct HLD
{

    const vector<vector<Pair<T>>> &adj;
    vector<T> dep, par, root, pos, SubtreeSz, child;
    int nxtPos;

    HLD(int n, const vector<vector<Pair<T>>> &G, int treeRoot = 1) : adj(G)
    {
        nxtPos = 1;
        // child is heavy child
        dep = par = root = pos = SubtreeSz = child = vector<T>(n + 5);
        init(treeRoot);
        build(treeRoot);
    }

    // init the tree
    void init(int u, int p = -1, int d = 0)
    {
        dep[u] = d, par[u] = p, SubtreeSz[u] = 1;
        for (auto [v, _] : adj[u])
        {
            if (v == p)
                continue;
            init(v, u, d + 1);
            SubtreeSz[u] += SubtreeSz[v];
            if (SubtreeSz[v] > SubtreeSz[child[u]])
                child[u] = v;
        }
    }

    // get the root of the chain
    int get_lca(int u, int v)
    {
        while (root[u] != root[v])
        {
            if (dep[root[u]] < dep[root[v]])
                swap(u, v);
            u = par[root[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    // build the chains
    void build(int u, bool newChain = true)
    {
        root[u] = newChain ? u : root[par[u]];
        pos[u] = nxtPos++;
        if (child[u])
            build(child[u], false);
        for (auto [v, _] : adj[u])
        {
            if (v == par[u] || v == child[u])
                continue;
            build(v, true);
        }
    }

    // make u lower
    void makeULower(int &u, int &v)
    {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v]))
            swap(u, v);
    }

    // move up the chain and also change the next position
    Pair<T> moveUp(int &u)
    {
        Pair<T> ret = {pos[root[u]], pos[u]};
        u = par[root[u]];
        return ret;
    }

    vector<Pair<T>> queryPath(int u, int v)
    {
        // return all ranges in segment tree
        vector<Pair<T>> ret;
        while (root[u] != root[v])
        {
            makeULower(u, v);
            ret.push_back(moveUp(u));
        }
        // add range between u and v
        makeULower(u, v);
        if (!VAL_ON_EDGE) // value on nodes
            ret.push_back({pos[v], pos[u]});
        else if (u != v) // don't include the root node
            ret.push_back({pos[v] + 1, pos[u]});
        return ret;
    }

    // get child of a node for problem that value on edges
    int getChild(int u, int v)
    {
        if (par[u] == v)
            return u;
        return v;
    }
};
