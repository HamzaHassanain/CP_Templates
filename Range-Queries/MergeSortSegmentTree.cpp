#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct MergeSortSegmentTree // 0-Based Indexing
{
    vector<vector<int>> seg;
    int sz, skip = 0;

    MergeSortSegmentTree(vector<int> &s)
    {
        sz = 1;
        while (sz < s.size())
            sz *= 2;
        seg = vector<vector<int>>(sz * 2);
        build(s, 0, sz - 1, 0);
    }

    void merge(vector<int> &ans, vector<int> &a, vector<int> &b)
    {
        int l = 0, r = 0;
        ans.reserve(a.size() + b.size());
        while (l < a.size() && r < b.size())
        {
            if (a[l] < b[r])
                ans.push_back(a[l++]);
            else
                ans.push_back(b[r++]);
        }
        while (l < a.size())
            ans.push_back(a[l++]);
        while (r < b.size())
            ans.push_back(b[r++]);
    }
    void build(vector<int> &s, int l, int r, int node)
    {
        if (l == r)
        {
            if (l < s.size())
            {
                seg[node] = {s[l]};
            }
            return;
        }
        int mid = (l + r) / 2;
        build(s, l, mid, 2 * node + 1);
        build(s, mid + 1, r, 2 * node + 2);
        merge(seg[node], seg[2 * node + 1], seg[2 * node + 2]);
    }
    int range_statistics(vector<int> &v, int k)
    {
        // number of elements equal to k
        return upper_bound(v.begin(), v.end(), k) - lower_bound(v.begin(), v.end(), k);
    }
    int query(int l, int r, int node, int lx, int rx, int k)
    {
        if (l > rx || r < lx)
        {
            return skip;
        }
        if (l >= lx && r <= rx)
        {
            return range_statistics(seg[node], k);
        }
        int mid = l + r >> 1;
        return query(l, mid, 2 * node + 1, lx, rx, k) + query(mid + 1, r, 2 * node + 2, lx, rx, k);
    }

    int query(int l, int r, int val)
    {

        return query(0, sz - 1, 0, l, r, val);
    }
};

int main()
{
    vector<int> a = {5, 2, 3, 5, 5, 6, 7, 8};
    MergeSortSegmentTree mst(a);
    cout << mst.query(0, 3, 5) << '\n';
    cout << mst.query(0, 7, 10) << '\n';
    cout << mst.query(0, 7, 1) << '\n';
    cout << mst.query(0, 7, 8) << '\n';
    return 0;
}