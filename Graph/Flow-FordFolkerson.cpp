#include <bits/stdc++.h>
using namespace std;
#define dl '\n'
#define ll long long

struct FordFolkerson
{
    vector<vector<int>> adj;
    vector<vector<int>> capacity;
    vector<vector<int>> flow;
    vector<int> parent;
    int n;

    FordFolkerson(int n) : n(n)
    {
        adj.resize(n);
        capacity.assign(n, vector<int>(n, 0));
        flow.assign(n, vector<int>(n, 0));
        parent.resize(n);
    }

    void add_edge(int from, int to, int cap)
    {
        adj[from].push_back(to);
        adj[to].push_back(from);
        capacity[from][to] = cap;
    }

    int bfs(int source, int sink)
    {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = -2;
        queue<pair<int, int>> q;
        q.push({source, INT_MAX});

        while (!q.empty())
        {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adj[cur])
            {
                if (parent[next] == -1 && capacity[cur][next] - flow > 0)
                {
                    parent[next] = cur;
                    int new_flow = min(flow, capacity[cur][next] - flow);
                    if (next == sink)
                    {
                        return new_flow;
                    }
                    q.push({next, new_flow});
                }
            }
        }

        return 0;
    }

    int max_flow(int source, int sink)
    {
        int cur_flow = 0;
        int new_flow;

        while (new_flow = bfs(source, sink))
        {
            cur_flow += new_flow;
            int cur = sink;
            while (cur != source)
            {
                int prev = parent[cur];
                flow[prev][cur] += new_flow;
                flow[cur][prev] -= new_flow;
                cur = prev;
            }
        }

        return cur_flow;
    }
};