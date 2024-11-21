#include <bits/stdc++.h>
using namespace std;

struct TimedDSU
{

    vector<int> parent, size, time;
    vector<vector<pair<int, int>>> history;
    // first is size, second is version (time)
    TimedDSU(int n, int base)
    {
        parent = size = time = vector<int>(n + 5);
        history = vector<vector<pair<int, int>>>(n + 5);
        for (int i = base; i <= n; i++)
        {

            history[i].push_back({1, 0});
            parent[i] = i, size[i] = 1;
        }
    }

    int find(int x, int t)
    {
        if (parent[x] == x)
            return x;
        if (time[x] > t)
            return x;
        return find(parent[x], t);
    }

    bool unionSet(int x, int y, int t)
    {
        x = find(x, t);
        y = find(y, t);
        if (x == y)
            return false;
        if (size[x] < size[y])
            swap(x, y);
        parent[y] = x;
        time[y] = t;
        size[x] += size[y];

        history[x].push_back({history[x].back().first + history[y].back().first, t});
        return true;
    }

    bool same(int x, int y, int t)
    {
        return find(x, t) == find(y, t);
    }

    int get_size(int a, int t)
    {

        a = find(a, t);
        int pos = upper_bound(history[a].begin(), history[a].end(), make_pair(t, INT_MAX)) - history[a].begin();
        return history[a][pos - 1].first;
    }
};
