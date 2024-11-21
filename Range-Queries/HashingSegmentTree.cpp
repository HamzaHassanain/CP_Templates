#include <bits/stdc++.h>
using namespace std;
#define ll long long

// Template Begins
#define int long long

const int N = 1e5 + 5, mod = 1e9 + 7, nod = 2e9 + 11;
ll base = 31, nase = 37, poww[N + 1], coww[N + 1], inv1[N + 1], inv2[N + 1];

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
    poww[0] = coww[0] = inv1[0] = inv2[0] = 1;
    int temp1 = powmod(base, mod - 2, mod);
    int temp2 = powmod(nase, nod - 2, nod);
    for (int i = 1; i < N; i++)
    {
        poww[i] = (base * poww[i - 1]) % mod;
        coww[i] = (nase * coww[i - 1]) % nod;
        inv1[i] = (inv1[i - 1] * temp1) % mod;
        inv2[i] = (inv2[i - 1] * temp2) % nod;
    }
}

struct Node
{
    ll h1, h2;

    Node(ll h1 = 0, ll h2 = 0) : h1(h1), h2(h2) {}

    bool operator==(const Node &other) const
    {
        return h1 == other.h1 && h2 == other.h2;
    }

    Node operator+(const Node &other) const
    {
        return Node((h1 + other.h1) % mod, (h2 + other.h2) % nod);
    }
};

struct HashingSegmentTree // 1-Based Indexing
{
    vector<Node> seg;
    int sz;
    Node merge(Node l, Node r)
    {
        return l + r;
    }
    void update(int l, int r, int node, int idx, int ch)
    {
        if (l == r)
        {
            seg[node] = {(ch * poww[idx]) % mod, (ch * coww[idx]) % nod};
            return;
        }
        int mid = (l + r) >> 1;
        if (idx <= mid)
            update(l, mid, 2 * node + 1, idx, ch);
        else
            update(mid + 1, r, 2 * node + 2, idx, ch);
        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }
    Node query(int l, int r, int node, int lx, int rx)
    {
        if (l >= lx && r <= rx)
        {
            return seg[node];
        }
        if (l > rx || r < lx)
            return {0, 0};
        int mid = l + r >> 1;
        Node lft = query(l, mid, 2 * node + 1, lx, rx);
        Node rgt = query(mid + 1, r, 2 * node + 2, lx, rx);
        return merge(lft, rgt);
    }

    HashingSegmentTree(int n)
    {
        sz = 1;
        while (sz <= n)
            sz *= 2;
        seg = vector<Node>(sz * 2);
    }
    void update(int idx, char ch)
    {
        update(0, sz - 1, 0, idx, ch - 'a' + 1);
    }
    Node query(int l, int r)
    {
        Node ret = query(0, sz - 1, 0, l, r);

        ret.h1 = (ret.h1 * inv1[l - 1]) % mod;
        ret.h2 = (ret.h2 * inv2[l - 1]) % nod;

        return ret;
    }
};
bool isPalindrome(HashingSegmentTree &a, HashingSegmentTree &b, int l, int r, int n)
{
    return (a.query(l, r) == b.query(n - r + 1, n - l + 1));
}
// Template Ends
int32_t main()
{
    string s = "abacaba";
    vector<pair<int, int>> queries = {{1, 7}, {1, 1}, {2, 7}, {2, 6}, {3, 6}, {3, 5}, {4, 5}, {4, 4}, {5, 4}, {5, 5}, {6, 5}, {6, 6}, {7, 6}, {7, 7}};
    int n = s.size();
    int q = queries.size();

    init();
    string t = s;
    reverse(t.begin(), t.end());

    HashingSegmentTree a(n), b(n);
    for (int i = 0; i < n; i++)
    {
        a.update(i + 1, s[i]);
        b.update(i + 1, t[i]);
    }

    // cout << a.query(1, 1).h1 << " " << a.query(1, 1).h2 << endl;
    // cout << a.query(n, n).h1 << " " << a.query(n, n).h2 << endl;

    for (auto [l, r] : queries)
    {
        l++, r++;
        cout << (isPalindrome(a, b, l, r, n) ? "YES" : "NO") << '\n';
    }

    return 0;
}