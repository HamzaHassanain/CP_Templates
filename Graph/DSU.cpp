#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct DSU
{

    vector<int> parent, size;

    DSU(int n, int base)
    {
        parent = size = vector<int>(n + 5);
        for (int i = base; i <= n; i++)
            parent[i] = i, size[i] = 1;
    }

    int find(int x)
    {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    bool unionSet(int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return false;
        if (size[x] < size[y])
            swap(x, y);
        parent[y] = x;
        size[x] += size[y];
        return true;
    }

    bool same(int x, int y)
    {
        return find(x) == find(y);
    }

    int getSize(int x)
    {
        return size[find(x)];
    }
};