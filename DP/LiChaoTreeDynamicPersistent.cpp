#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// for min value of family of functions with transcending property
const ll inf = 1e18;
const ll C = 1e6 + 10;
struct LiChaoTreePersistent
{
    struct Function
    {
        ll m, b;
        ll operator()(ll x) { return m * x + b; }
    };
    struct Node
    {
        Function seg;
        Node *left, *right;
        Node(Function _seg) : seg(_seg), left(0), right(0) {}
    };

    vector<Node *> roots;

    ll LIM_L = -C, LIM_R = C;

    LiChaoTreePersistent(int N)
    {
        for (int i = 0; i <= N; i++)
            roots.push_back(new Node({0, inf}));
    }

    Node *insert(ll l, ll r, Function seg, Node *node)
    {
        if (l > r)
            return node;
        if (l < LIM_L or r > LIM_R)
            return node;

        if (l + 1 == r)
        {
            return new Node(seg);
        }
        Node *res = new Node(node->seg);
        if (node and node->left)
            res->left = node->left;
        if (node and node->right)
            res->right = node->right;

        ll mid = (l + r) >> 1;
        if (seg.m < res->seg.m)
            swap(seg, res->seg);
        if (res->seg(mid) > seg(mid))
        {
            swap(seg, res->seg);
            if (res->right)
                res->right = insert(mid, r, seg, res->right);
            else
                res->right = new Node(seg);
        }
        else
        {
            if (res->left)
                res->left = insert(l, mid, seg, res->left);
            else
                res->left = new Node(seg);
        }

        return res;
    }
    ll query(ll l, ll r, ll x, Node *node)
    {
        if (x < LIM_L || x > LIM_R)
            return inf;
        if (!node)
            return inf;

        if (l + 1 == r)
            return node->seg(x);
        ll mid = (l + r) >> 1;
        if (x < mid && node->left)
            return min(node->seg(x), query(l, mid, x, node->left));
        else if (node->right)
            return min(node->seg(x), query(mid, r, x, node->right));
        return node->seg(x);
    }
    void del(Node *node)
    {
        if (node->left)
            del(node->left);
        if (node->right)
            del(node->right);
        delete node;
    }
    void del(int i)
    {
        del(roots[i]);
    }

    ll query(ll x, int i)
    {
        return query(LIM_L, LIM_R, x, roots[i]);
    }

    void insert(Function seg, int i, int par)
    {
        roots[i] = insert(LIM_L, LIM_R, seg, roots[par]);
    }
};

void solve()
{
    int Q;
    cin >> Q;

    LiChaoTreePersistent lct(1e6 + 10);
    int current_index = 0;

    lct.insert({0, 0}, 0, 0);

    while (Q--)
    {
        int type;
        cin >> type;
        if (type == 1)
        {
            int new_idx_va;
            cin >> new_idx_va;
            current_index = new_idx_va;
        }
        else if (type == 2)
        {
            int b, a, index_value;
            cin >> a >> b >> index_value;
            lct.insert({-a, -b}, index_value, current_index);
            current_index = index_value;
        }
        else
        {
            int x;
            cin >> x;
            cout << -lct.query(x, current_index) << "\n";
        }
    }
}