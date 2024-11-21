#include <bits/stdc++.h>
using namespace std;

int SQ;
struct MO_Query
{
    int l, r, q_idx, block_idx;
    MO_Query() {}
    MO_Query(int _l, int _r, int _q_idx)
    {
        l = _l, r = _r, q_idx = _q_idx, block_idx = _l / SQ;
    }
    bool operator<(const MO_Query &y) const
    {
        if (block_idx != y.block_idx)
        {
            return block_idx < y.block_idx;
        }
        return r < y.r;
    }
};
struct MOs
{
    long long n, q;
    vector<long long> arr, q_ans;
    long long res = 0;
    vector<MO_Query> queries;

    MOs() {}
    MOs(long long _n, long long _q, vector<long long> &_arr, vector<MO_Query> &_queries)
    {
        n = _n;
        q = _q;
        arr = _arr;         // arr is one based
        queries = _queries; // queries are zero based
        q_ans.resize(q);
    }
    void buildAnswer()
    {

        sort(queries.begin(), queries.end());

        int l = queries[0].l, r = queries[0].l - 1;
        for (int i = 0; i < q; i++)
        {
            while (r < queries[i].r)
                add(++r);
            while (r > queries[i].r)
                remove(r--);
            while (l < queries[i].l)
                remove(l++);
            while (l > queries[i].l)
                add(--l);

            q_ans[queries[i].q_idx] = res;
        }
    };
    void printAnswer()
    {
        for (int i = 0; i < q; i++)
        {
            cout << q_ans[i] << ' ';
        }
        cout << '\n';
    }
    void add(int idx)
    {
    }
    void remove(int idx)
    {
    }
};

signed main()
{
    int tc;
    cin >> tc;
    while (tc--)
    {

        long long n, q;
        cin >> n >> q;

        SQ = sqrt(n) + 1;

        vector<long long> arr(n + 1); // Please Pass the array in 1-based indexing
        for (int i = 1; i <= n; i++)
            cin >> arr[i];

        vector<MO_Query> queries;
        for (int i = 0; i < q; i++)
        {
            int l, r;
            cin >> l >> r;
            queries.push_back(MO_Query(l, r, i));
        }

        MOs mo(n, q, arr, queries);
        mo.buildAnswer();
        mo.printAnswer();
    }
}
