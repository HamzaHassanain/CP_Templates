#include <bits/stdc++.h>
using namespace std;

struct DSURollback
{
    vector<int> p, sz;
    // stores previous unites
    vector<pair<int &, int>> history;
    int ConnectedComponents = 0;
    DSURollback() {}
    DSURollback(int n)
    {
        set(n);
    }

    void set(int n)
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
        if (a == b)
            return;
        if (sz[a] < sz[b])
            swap(a, b);
        history.push_back({sz[a], sz[a]}), history.push_back({p[b], p[b]});
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
        while (history.size() > until)
        {
            if (history.back().first != history.back().second)
                ConnectedComponents++;
            history.back().first = history.back().second, history.pop_back();
            history.back().first = history.back().second, history.pop_back();
        }
    }
};