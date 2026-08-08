#include <bits/stdc++.h>
using namespace std;

#define int long long

#define fastio                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);                    \
    cout.tie(NULL);

struct Node
{
    int msk_even = 0, msk_odd = 0, len_even = 0, len_odd = 0, lazy = -1;
};

Node merge(Node &a, Node &b)
{
    Node res;
    res.msk_even = a.msk_even | b.msk_even;
    res.msk_odd = a.msk_odd | b.msk_odd;
    res.len_even = a.len_even + b.len_even;
    res.len_odd = a.len_odd + b.len_odd;
    res.lazy = -1;
    return res;
}

struct Sgt
{
    int size;
    vector<Node> nodes;
    void init(int n)
    {
        size = 1;
        while (size < n)
            size *= 2;
        nodes.assign(2 * size, Node());
    }

    void build(int x, int lx, int rx, const string &s)
    {
        if (rx - lx == 1)
        {
            if (lx < s.size())
            {
                int bit = (1LL << (s[lx] - 'a'));
                if (lx % 2 == 0)
                {
                    nodes[x].len_even = 1;
                    nodes[x].msk_even = bit;
                }
                else
                {
                    nodes[x].len_odd = 1;
                    nodes[x].msk_odd = bit;
                }
            }
            return;
        }
        int mid = lx + (rx - lx) / 2;
        build(2 * x + 1, lx, mid, s);
        build(2 * x + 2, mid, rx, s);
        nodes[x] = merge(nodes[2 * x + 1], nodes[2 * x + 2]);
    }

    void push(int x, int lx, int rx)
    {
        if (nodes[x].lazy != -1)
        {
            int val = nodes[x].lazy;
            int bit = (1LL << val);

            if (nodes[2 * x + 1].len_even > 0)
                nodes[2 * x + 1].msk_even = bit;
            else
                nodes[2 * x + 1].msk_even = 0;

            if (nodes[2 * x + 1].len_odd > 0)
                nodes[2 * x + 1].msk_odd = bit;
            else
                nodes[2 * x + 1].msk_odd = 0;
            nodes[2 * x + 1].lazy = val;

            if (nodes[2 * x + 2].len_even > 0)
                nodes[2 * x + 2].msk_even = bit;
            else
                nodes[2 * x + 2].msk_even = 0;

            if (nodes[2 * x + 2].len_odd > 0)
                nodes[2 * x + 2].msk_odd = bit;
            else
                nodes[2 * x + 2].msk_odd = 0;
            nodes[2 * x + 2].lazy = val;

            nodes[x].lazy = -1;
        }
    }

    void update(int l, int r, int val, int x, int lx, int rx)
    {
        if (lx >= r || rx <= l)
            return;
        if (lx >= l && rx <= r)
        {
            int bit = (1LL << val);
            if (nodes[x].len_even > 0)
                nodes[x].msk_even = bit;
            else
                nodes[x].msk_even = 0;

            if (nodes[x].len_odd > 0)
                nodes[x].msk_odd = bit;
            else
                nodes[x].msk_odd = 0;

            nodes[x].lazy = val;
            return;
        }
        push(x, lx, rx);
        int mid = lx + (rx - lx) / 2;
        update(l, r, val, 2 * x + 1, lx, mid);
        update(l, r, val, 2 * x + 2, mid, rx);
        nodes[x] = merge(nodes[2 * x + 1], nodes[2 * x + 2]);
    }

    Node query(int l, int r, int x, int lx, int rx)
    {
        if (lx >= r || rx <= l)
            return Node();
        if (lx >= l && rx <= r)
            return nodes[x];
        push(x, lx, rx);
        int mid = lx + (rx - lx) / 2;
        Node left = query(l, r, 2 * x + 1, lx, mid);
        Node right = query(l, r, 2 * x + 2, mid, rx);
        return merge(left, right);
    }
};

void solve()
{
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;

    Sgt st;
    st.init(n);
    st.build(0, 0, st.size, s);

    while (q--)
    {
        string type;
        cin >> type;
        if (type == "QUERY")
        {
            int l, r;
            cin >> l >> r;
            l--;

            Node res = st.query(l, r, 0, 0, st.size);

            int p_even = __builtin_popcountll(res.msk_even);
            int p_odd = __builtin_popcountll(res.msk_odd);

            if (p_even <= 1 && p_odd <= 1 && (res.msk_even & res.msk_odd) == 0)
            {
                cout << "YES\n";
            }
            else
            {
                cout << "NO\n";
            }
        }
        else if (type == "SET")
        {
            int l, r;
            char c;
            cin >> l >> r >> c;
            l--;
            st.update(l, r, c - 'a', 0, 0, st.size);
        }
    }
}

signed main()
{
    fastio
#ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    int t = 1;
    cin >> t;
    while (t--)
    {
        solve();
    }
    return 0;
}
