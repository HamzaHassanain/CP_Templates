#include <bits/stdc++.h>
using namespace std;
#define dl '\n'
#define ll long long

vector<vector<int>> adjList;
ll n, m;
ll max_biapartite_matching()
{
    vector<int> matchR(n, -1);
    vector<bool> vis;
    function<bool(int)> dfs = [&](int u)
    {
        if (vis[u])
            return false;
        vis[u] = true;
        for (int v : adjList[u])
        {
            if (matchR[v] == -1 || dfs(matchR[v]))
            {
                matchR[v] = u;
                return true;
            }
        }
        return false;
    };
    ll ans = 0;
    for (int i = 0; i < n; i++)
    {
        vis.assign(m, false);
        ans += dfs(i);
    }
    return ans;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> n >> m;
    adjList = vector<vector<int>>(n);
    while (m--)
    {

        int u, v;
        cin >> u >> v;
        u--, v--;

        adjList[u].push_back(v);
    }

    cout << max_biapartite_matching() / 2 << dl;

    return 0;
}