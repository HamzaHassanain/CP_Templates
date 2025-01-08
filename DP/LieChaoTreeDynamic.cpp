#include <bits/stdc++.h>
using namespace std;
typedef long long ll;



// for min value of family of functions with transcending property
const ll inf = 1e18;
const ll C = 1e6 + 10;
struct LiChaoTree
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

    Node *root;
    ll LIM_L = -C, LIM_R = C;
    ll sz = 0;

    LiChaoTree()
    {
        sz = 0;
        root = new Node({0, inf});
    }

    void insert(ll l, ll r, Function seg, Node *node)
    {
        sz++;
        if (l + 1 == r)
        {
            if (seg(l) < node->seg(l))
                node->seg = seg;
            return;
        }
        ll mid = (l + r) >> 1;
        if (seg.m < node->seg.m)
            swap(seg, node->seg);
        if (node->seg(mid) > seg(mid))
        {
            swap(seg, node->seg);
            if (node->right)
                insert(mid, r, seg, node->right);
            else
                node->right = new Node(seg);
        }
        else
        {
            if (node->left)
                insert(l, mid, seg, node->left);
            else
                node->left = new Node(seg);
        }
    }
    ll query(ll l, ll r, ll x, Node *node)
    {
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
    void del()
    {
        del(root);
    }
    void merge(Node *node)
    {
        if (node->seg.m != 0 || node->seg.b != inf)
            insert(-C, C, node->seg, root);
        if (node->left)
            merge(node->left);
        if (node->right)
            merge(node->right);
    }
    void merge(LiChaoTree &other)
    {
        this->sz += other.sz;
        merge(other.root);
    }

    ll query(ll x)
    {
        if (sz == 0)
            return 0;

        return query(LIM_L, LIM_R, x, root);
    }
    void insert(Function seg)
    {
        insert(LIM_L, LIM_R, seg, root);
    }

    int size()
    {
        return sz;
    }
};
