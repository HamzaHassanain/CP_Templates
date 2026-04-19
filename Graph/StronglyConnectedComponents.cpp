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
        inStack.assign(n, 0); // FIX 1: Use assign for safety to ensure clean state
        lowLink.resize(n);
        dfn.assign(n, -1);
        ndfn = 0;
        comp.assign(n, -1);
        comps.clear();
        // FIX 2: cntSrc/cntSnk should be initialized in computeCompGraph after comps are found
        inDeg.assign(n, 0);
        outDeg.assign(n, 0);
        dagList.assign(n, vector<int>());
    }

    void Run()
    {
        for (int i = 0; i < (int)adjList.size(); i++)
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

        int children = 0; // FIX 3: Needed to correctly identify the root as an articulation point

        for (int i = 0; i < (int)adjList[node].size(); i++)
        {
            int ch = adjList[node][i];
            if (!DIRECTED_GRAPH && ch == parent)
                continue;

            if (dfn[ch] == -1)
            {
                children++;
                tarjan(ch, node);
                lowLink[node] = min(lowLink[node], lowLink[ch]);
                
                // FIX 4: Only relevant for undirected graphs
                if (!DIRECTED_GRAPH) {
                    if (lowLink[ch] > dfn[node])
                        brdges.push_back({node, ch});
                    if (parent != -1 && lowLink[ch] >= dfn[node])
                        articulationPoints.push_back(node);
                }
            }
            else if (inStack[ch])
                lowLink[node] = min(lowLink[node], dfn[ch]);
        }
        
        // FIX 5: Special case for root articulation point in undirected graphs
        if (!DIRECTED_GRAPH && parent == -1 && children > 1)
            articulationPoints.push_back(node);

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
        if (!DIRECTED_GRAPH)
            return;
            
        int csz = comps.size();
        // FIX 6: Corrected typo 'dajList' to 'dagList' and reset sizes based on csz, not n
        dagList.assign(csz, vector<int>());
        inDeg.assign(csz, 0);
        outDeg.assign(csz, 0);
        cntSrc = cntSnk = csz;

        for (int i = 0; i < (int)adjList.size(); i++)
        {
            for (int j = 0; j < (int)adjList[i].size(); j++)
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
