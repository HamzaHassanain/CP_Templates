#include <bits/stdc++.h>

using namespace std;
#define sz(x) (int)x.size()
#define ll long long

struct FenwickTreeRangeUpdate // 0-Based indexing
{
    int N;
    int DEFAULT;
    vector<int> M, C;

    FenwickTreeRangeUpdate(int sz = 0)
    {
        init(sz);
    }
    FenwickTreeRangeUpdate(vector<int> &nums)
    {
        init(sz(nums));
        build(nums);
    }
    void init(int sz = 0)
    {
        N = sz + 1, DEFAULT = 0;
        M = C = vector<int>(N + 10);
    }
    void build(vector<int> &nums)
    {
        for (int i = 0; i < sz(nums); i++)
            add(i, i, nums[i]);
    }

    inline int lb(int idx)
    {
        return (idx & -idx);
    }

    inline void add_range(int idx, int addM, int addC)
    {
        idx++;
        while (idx <= N)
        {
            M[idx] += addM;
            C[idx] += addC;
            idx += lb(idx);
        }
    }

    inline void add(int idx, int val) // better not to use this function
    {
        add(idx, idx, val);
    }

    inline int get(int idx) // better not to use this function
    {
        int ans = DEFAULT;
        int pos = idx++;
        while (idx)
        {
            ans += pos * M[idx] + C[idx];
            idx -= lb(idx);
        }
        return ans;
    }

    void add(int l, int r, int val)
    {
        add_range(l, val, -val * (l - 1));
        add_range(r + 1, -val, val * r);
    }
    int query(int L, int R)
    {
        if (L > R)
            return DEFAULT;
        return get(R) - get(L - 1);
    }
};

int main()
{

    vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    FenwickTreeRangeUpdate ft(a);

    cout << ft.query(0, 8) << endl;

    // ft.add(0, 8, 1);

    cout << ft.query(0, 8) << endl;

    ft.add(0, 0, 10);

    cout << ft.query(0, 1) << endl;

    return 0;
}