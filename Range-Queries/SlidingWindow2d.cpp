#include <bits/stdc++.h>
using namespace std;
const string dl = "\n";
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()

class MinQ
{
public:
    queue<int> q;
    deque<int> dq;
    void push(int x)
    {
        q.push(x);
        while (!dq.empty() && x < dq.back())
        {
            dq.pop_back();
        }
        dq.push_back(x);
    }
    void pop()
    {
        if (q.front() == dq.front())
        {
            q.pop();
            dq.pop_front();
        }
        else
            q.pop();
    }
    int min()
    {
        return dq.front();
    }
};

vector<int> minSliding_1d_Window(vector<int> &v, int k)
{
    MinQ q;
    int n = v.size();
    vector<int> ans;
    for (int i = 0; i < k; i++)
    {
        q.push(v[i]);
    }
    for (int i = k; i < n; i++)
    {
        ans.push_back(q.min());
        q.pop();
        q.push(v[i]);
    }
    ans.push_back(q.min());

    return ans;
}
vector<vector<int>> minSliding_2d_Window(vector<vector<int>> &v, int subN, int subM)
{
    int n = v.size();
    int m = v[0].size();

    vector<vector<int>> ans(n, vector<int>(m));

    for (int i = 0; i < m; i++)
    {
        vector<int> temp;
        for (int j = 0; j < n; j++)
        {
            temp.push_back(v[j][i]);
        }
        vector<int> res = minSliding_1d_Window(temp, subN);
        for (int j = 0; j < n - subN + 1; j++)
        {
            ans[j][i] = res[j];
        }
    }

    vector<vector<int>> finalAns(n - subN + 1, vector<int>(m - subM + 1));
    for (int i = 0; i < n - subN + 1; i++)
    {
        vector<int> temp;
        for (int j = 0; j < m; j++)
        {
            temp.push_back(ans[i][j]);
        }
        vector<int> res = minSliding_1d_Window(temp, subM);
        for (int j = 0; j < m - subM + 1; j++)
        {
            finalAns[i][j] = res[j];
        }
    }

    return finalAns;
}

// max sliding window

class MaxQ
{
public:
    queue<int> q;
    deque<int> dq;
    void push(int x)
    {
        q.push(x);
        while (!dq.empty() && x > dq.back())
        {
            dq.pop_back();
        }
        dq.push_back(x);
    }
    void pop()
    {
        if (q.front() == dq.front())
        {
            q.pop();
            dq.pop_front();
        }
        else
            q.pop();
    }
    int max()
    {
        return dq.front();
    }
};

vector<int> maxSliding_1d_Window(vector<int> &v, int k)
{
    MaxQ q;
    int n = v.size();
    vector<int> ans;
    for (int i = 0; i < k; i++)
    {
        q.push(v[i]);
    }
    for (int i = k; i < n; i++)
    {
        ans.push_back(q.max());
        q.pop();
        q.push(v[i]);
    }
    ans.push_back(q.max());

    return ans;
}

vector<vector<int>> maxSliding_2d_Window(vector<vector<int>> &v, int subN, int subM)
{
    int n = v.size();
    int m = v[0].size();

    vector<vector<int>> ans(n, vector<int>(m));

    for (int i = 0; i < m; i++)
    {
        vector<int> temp;
        for (int j = 0; j < n; j++)
        {
            temp.push_back(v[j][i]);
        }
        vector<int> res = maxSliding_1d_Window(temp, subN);
        for (int j = 0; j < n - subN + 1; j++)
        {
            ans[j][i] = res[j];
        }
    }

    vector<vector<int>> finalAns(n - subN + 1, vector<int>(m - subM + 1));
    for (int i = 0; i < n - subN + 1; i++)
    {
        vector<int> temp;
        for (int j = 0; j < m; j++)
        {
            temp.push_back(ans[i][j]);
        }
        vector<int> res = maxSliding_1d_Window(temp, subM);
        for (int j = 0; j < m - subM + 1; j++)
        {
            finalAns[i][j] = res[j];
        }
    }

    return finalAns;
}

int main()
{

    vector<vector<int>> v = {{1, 3, -1, -3, 5, 3, 6, 7},
                             {1, 3, -1, -3, 5, 3, 6, 7},
                             {1, 3, -1, -3, 5, 3, 6, 7},
                             {1, 3, -1, -3, 5, 3, 6, 7},
                             {1, 3, -1, -3, 5, 3, 6, 7},
                             {1, 3, -1, -3, 5, 3, 6, 7},
                             {1, 3, -1, -3, 5, 3, 6, 7},
                             {1, 3, -1, -3, 5, 3, 6, 7}};

    vector<vector<int>> ans = maxSliding_2d_Window(v, 3, 3);

    for (auto x : ans)
    {
        for (auto y : x)
        {
            cout << y << " ";
        }
        cout << endl;
    }

    return 0;
}
