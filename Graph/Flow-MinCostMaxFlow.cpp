#include <bits/stdc++.h>
using namespace std;
#define dl '\n'
#define ll long long

struct MinCostMaxFlow
{
#define rep(i, n) for (int i = 0; i < n; i++)
#define ff first
#define ss second

    typedef pair<ll, ll> pTT;
    ll INF = numeric_limits<ll>::max();
    struct Edge
    {
        int v;
        ll c, w;
        Edge(int v, ll c, ll w) : v(v), c(c), w(w) {}
    };

    MinCostMaxFlow(int n) : n(n), E(n), D(n), P(n, 0), V(n, 0) {}

    int n;
    vector<vector<int>> E;
    vector<Edge> L;
    vector<int> F;
    vector<ll> D, P;
    vector<bool> V;

    bool dij(int s, int t)
    {
        D.assign(n, INF);
        F.assign(n, -1);
        V.assign(n, false);
        D[s] = 0;
        rep(_, n)
        {
            int best = -1;
            rep(i, n) if (!V[i] && (best == -1 || D[best] > D[i])) best = i;
            if (D[best] >= INF)
                break;
            V[best] = true;
            for (int e : E[best])
            {
                Edge ed = L[e];
                if (ed.c == 0)
                    continue;
                ll toD = D[best] + ed.w + P[best] - P[ed.v];
                if (toD < D[ed.v])
                    D[ed.v] = toD, F[ed.v] = e;
            }
        }
        return D[t] < INF;
    }

    pTT augment(int s, int t)
    {
        pTT flow(L[F[t]].c, 0);
        for (int v = t; v != s; v = L[F[v] ^ 1].v)
            flow.ff = min(flow.ff, L[F[v]].c), flow.ss += L[F[v]].w;
        for (int v = t; v != s; v = L[F[v] ^ 1].v)
            L[F[v]].c -= flow.ff, L[F[v] ^ 1].c += flow.ff;
        return flow;
    }

    pTT mcmf(int s, int t)
    {
        pTT ans(0, 0);
        if (!dij(s, t))
            return ans;
        rep(i, n) if (D[i] < INF) P[i] += D[i];
        while (dij(s, t))
        {
            auto flow = augment(s, t);
            ans.ff += flow.ff, ans.ss += flow.ff * flow.ss;
            rep(i, n) if (D[i] < INF) P[i] += D[i];
        }
        return ans;
    }

    void addEdge(int u, int v, ll c, ll w)
    {
        E[u].push_back(L.size());
        L.emplace_back(v, c, w);
        E[v].push_back(L.size());
        L.emplace_back(u, 0, -w);
    }
};

int main()
{

    // https://www.spoj.com/problems/SCITIES/

    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

    int tc;
    cin >> tc;

    while (tc--)
    {

        int n1, n2;
        cin >> n1 >> n2;

        int bigSource = 0, bigSink = n1 + n2 + 1;

        MinCostMaxFlow mcf(n1 + n2 + 2);

        while (true)
        {
            int u, v, g;
            cin >> u >> v >> g;

            if (u == 0 && v == 0 && g == 0)
            {
                break;
            }

            mcf.addEdge(u, n1 + v, 1, -g);
        }

        for (int i = 1; i <= n1; i++)
        {
            mcf.addEdge(bigSource, i, 1, 0);
        }

        for (int i = 1; i <= n2; i++)
        {
            mcf.addEdge(n1 + i, bigSink, 1, 0);
        }

        auto ans = mcf.mcmf(bigSource, bigSink);
        cout << -ans.second << dl;
    }
}