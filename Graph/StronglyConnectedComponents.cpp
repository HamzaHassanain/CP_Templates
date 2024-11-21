#include <bits/stdc++.h>
using namespace std;
#define dl '\n'
#define ll long long

template <int DIRECTED_GRAPH = 1>
struct SCC
{
#define sz(x) (int)x.size()
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

    vector<vector<int>> adjList, comps, dagList;
    vector<int> inStack, lowLink, dfn, comp, inDeg, outDeg;
    stack<int> stk;
    int ndfn, cntSrc, cntSnk;

    vector<pair<int, int>> brdges;
    vector<int> articulationPoints;
    SCC(vector<vector<int>> &adj)
    {
        int n = adj.size();
        adjList = adj;
        inStack.resize(n);
        lowLink.resize(n);
        dfn.assign(n, -1);
        ndfn = 0;
        comp.assign(n, -1);
        comps.clear();
        cntSrc = cntSnk = n;
        inDeg.resize(n);
        outDeg.resize(n);
        dagList.resize(n);
    }

    void Run()
    {
        for (int i = 0; i < adjList.size(); i++)
        {
            if (dfn[i] == -1)
                tarjan(i, -1);
        }
        computeCompGraph();
    }

    void tarjan(int node, int parent)
    {
        lowLink[node] = dfn[node] = ndfn++, inStack[node] = 1;
        stk.push(node);

        for (int i = 0; i < adjList[node].size(); i++)
        {
            if (not DIRECTED_GRAPH && adjList[node][i] == parent)
                continue;
            // ignore the edge to the parent in undirected graphs
            // (this is the only change from the directed version of the code

            // get bredges and articulation points
            int ch = adjList[node][i];
            if (dfn[ch] == -1)
            {
                tarjan(ch, node);
                lowLink[node] = min(lowLink[node], lowLink[ch]);
                if (lowLink[ch] > dfn[node])
                    brdges.push_back({node, ch});
                if (lowLink[ch] >= dfn[node])
                    articulationPoints.push_back(node);
            }
            else if (inStack[ch])
                lowLink[node] = min(lowLink[node], dfn[ch]);
        }

        if (lowLink[node] == dfn[node])
        {
            comps.push_back(vector<int>());
            int x = -1;
            while (x != node)
            {
                x = stk.top(), stk.pop(), inStack[x] = 0;
                comps.back().push_back(x);
                comp[x] = sz(comps) - 1;
            }
        }
    }

    void computeCompGraph()
    {
        if (not DIRECTED_GRAPH)
            return;
        int csz = comps.size();
        dajList.resize(csz);
        for (int i = 0; i < adjList.size(); i++)
        {
            for (int j = 0; j < adjList[i].size(); j++)
            {
                int k = adjList[i][j];
                if (comp[k] != comp[i])
                {
                    dagList[comp[i]].push_back(comp[k]);
                    if (!(inDeg[comp[k]]++))
                        cntSrc--;
                    if (!(outDeg[comp[i]]++))
                        cntSnk--;
                }
            }
        }
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n);
    while (m--)
    {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
    }

    SCC<> scc(adj);
    scc.Run();

    cout << scc.comps.size() << dl;

    return 0;
}