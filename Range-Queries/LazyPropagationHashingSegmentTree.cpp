#include <bits/stdc++.h>
using namespace std;
#define ll long long

// Template Begins

#define ll long long

#define int long long
const int N = 3e5 + 5, mod1 = 1e9 + 7, mod2 = 2e9 + 11;
ll base1 = 31, base2 = 37, pw1[N + 1], pw2[N + 1], inv1[N + 1], inv2[N + 1], prefixPow1[N + 1], prefixPow2[N + 1];
ll powmod(ll a, ll b, ll m)
{
    ll ans = 1;
    while (b > 0)
    {
        if (b & 1)
        {
            ans = (ans * a) % m;
        }
        a = (a * a) % m;
        b >>= 1;
    }
    return ans;
}
void init()
{
    pw1[0] = pw2[0] = inv1[0] = inv2[0] = 1;
    int temp1 = powmod(base1, mod1 - 2, mod1);
    int temp2 = powmod(base2, mod2 - 2, mod2);

    for (ll i = 1; i < N; i++)
    {
        pw1[i] = (base1 * pw1[i - 1]) % mod1;
        pw2[i] = (base2 * pw2[i - 1]) % mod2;
        inv1[i] = (inv1[i - 1] * temp1) % mod1;
        inv2[i] = (inv2[i - 1] * temp2) % mod2;

        prefixPow1[i] = (prefixPow1[i - 1] + pw1[i]) % mod1;
        prefixPow2[i] = (prefixPow2[i - 1] + pw2[i]) % mod2;
    }
}
struct LazyHashingSegmentTree // 1-Based Indexing
{
    vector<pair<int, int>> seg;
    int sz;
    vector<pair<int, int>> lazy;

    LazyHashingSegmentTree(int n)
    {
        sz = 1;
        while (sz <= n)
            sz *= 2;
        seg = vector<pair<int, int>>(sz * 2);
        lazy = vector<pair<int, int>>(sz * 2, {-1, -1});
    }

    pair<int, int> merge(pair<int, int> l, pair<int, int> r)
    {
        pair<int, int> ret = l;
        ret.first = (ret.first + r.first) % mod1;
        ret.second = (ret.second + r.second) % mod2;
        return ret;
    }
    // pair<int, int> lazy_merge(pair<int, int> l, pair<int, int> r)
    // {
    //     return r;
    // }
    void propagate(int l, int r, int node)
    {
        if (lazy[node].first == -1)
            return;

        char ch = lazy[node].first;
        int beg = l + 1, end = r + 1;
        // use the prefix powers to calculate the hash value of the range
        int len = r - l + 1;
        int firMod = (prefixPow1[end] - (beg ? prefixPow1[beg - 1] : 0ll) + mod1) % mod1;
        int secMod = (prefixPow2[end] - (beg ? prefixPow2[beg - 1] : 0ll) + mod2) % mod2;

        seg[node] = {(firMod * ch) % mod1, (secMod * ch) % mod2};
        // cout << "seg[node]: " << seg[node].first << dl;
        if (l != r)
        {
            lazy[2 * node + 1] = lazy[2 * node + 2] = lazy[node];
            // beg is the beginning index of the range
            // end is the ending index of the range
            // beg =
            // lazy[2 * node + 1] = {ch, beg, end};
        }

        lazy[node] = {-1, -1};
    }
    void update(int l, int r, int node, int idx, int ch)
    {
        if (l == r)
        {
            seg[node] = {(ch * pw1[l + 1]) % mod1, (ch * pw2[idx + 1]) % mod2};
            return;
        }
        int mid = l + r >> 1;
        if (idx <= mid)
            update(l, mid, 2 * node + 1, idx, ch);
        else
            update(mid + 1, r, 2 * node + 2, idx, ch);
        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }
    pair<int, int> query(int l, int r, int node, int lx, int rx)
    {
        propagate(l, r, node);
        if (l >= lx && r <= rx)
        {
            return seg[node];
        }
        if (l > rx || r < lx)
            return {0, 0};
        int mid = l + r >> 1;
        pair<int, int> lft = query(l, mid, 2 * node + 1, lx, rx);
        pair<int, int> rgt = query(mid + 1, r, 2 * node + 2, lx, rx);
        return merge(lft, rgt);
    }
    void set_range_to_char(int l, int r, int node, int lx, int rx, int ch)
    {
        propagate(l, r, node);
        if (l >= lx && r <= rx)
        {
            lazy[node] = {ch, ch};
            propagate(l, r, node);
            return;
        }
        if (l > rx || r < lx)
            return;
        int mid = l + r >> 1;
        set_range_to_char(l, mid, 2 * node + 1, lx, rx, ch);
        set_range_to_char(mid + 1, r, 2 * node + 2, lx, rx, ch);
        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }

    void update(int idx, char ch)
    {
        update(0, sz - 1, 0, idx, ch - 'a' + 1);
    }
    void set_range_to_char(int l, int r, char ch)
    {
        set_range_to_char(0, sz - 1, 0, l, r, ch - 'a' + 1);
    }
    pair<int, int> query(int l, int r)
    {
        pair<int, int> ret = query(0, sz - 1, 0, l, r);
        ret.first = (ret.first * inv1[l - 1]) % mod1;
        ret.second = (ret.second * inv2[l - 1]) % mod2;
        return ret;
    }
};

// Template Ends

int32_t main()
{
    init();
    string s;
    cin >> s;
    int n = s.size();
    LazyHashingSegmentTree a(n);
    for (int i = 0; i < n; i++)
    {
        a.update(i + 1, s[i]);
    }
    int q;
    cin >> q;
    while (q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int l, r;
            cin >> l >> r;
            pair<int, int> ans = a.query(l, r);
            cout << ans.first << " " << ans.second << endl;
        }
        else
        {
            int l, r;
            char ch;
            cin >> l >> r >> ch;
            a.set_range_to_char(l, r, ch);
        }
    }
    return 0;
}