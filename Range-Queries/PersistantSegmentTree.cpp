#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct PersistantSegmentTree
{

    struct Node
    {

        ll val;
        Node *left, *right;

        Node(ll _val = 0)
        {
            this->val = _val;
            left = right = this;
        }
        Node(Node *node)
        {
            val = node->val;
            left = node->left;
            right = node->right;
        }
        Node(Node *l, Node *r)
        {
            // Operation But With Pointers
            val = l->val + r->val;
            left = l;
            right = r;
        }
        Node(Node *node, Node *l, Node *r)
        {
            val = node->val;
            left = l;
            right = r;
        }
    };

    vector<Node *> roots;
    ll N, Lx, Rx;

    PersistantSegmentTree(int n = 0, ll lx = 0, ll rx = 1e9) : N(n), Lx(lx), Rx(rx)
    {
        // roots = vector<Node *>(n + 5, new Node);
        roots.push_back(new Node());
    }

    Node *build(const vector<ll> &nums, ll l, ll r)
    {
        if (l == r) // if nums is 0-based indexed, otherwise, use nums[l] only if 1-based indexed
            return new Node(nums[l - 1]);
        ll mx = l + (r - l) / 2;
        Node *L = build(nums, l, mx);
        Node *R = build(nums, mx + 1, r);
        return new Node(new Node(L, R), L, R);
    }

    void build(const vector<ll> &nums)
    {
        roots[0] = build(nums, Lx, Rx);
    }

    Node operation(Node a, Node b)
    {
        Node Merged = new Node();
        Merged.val = a.val + b.val;
        return Merged;
    }

    Node *update(Node *root, int idx, ll val, ll lx, ll rx)
    {
        if (idx < lx || idx > rx)
            return root;
        if (lx == rx)
            return new Node(val);
        ll mx = lx + (rx - lx) / 2;
        Node *L = update(root->left, idx, val, lx, mx);
        Node *R = update(root->right, idx, val, mx + 1, rx);
        return new Node(new Node(L, R), L, R);
    }

    void insert(int idx, ll val, int curr_time, int prev_time)
    {
        roots[curr_time] = update(roots[prev_time], idx, val, Lx, Rx);
    }

    void update(int idx, ll val, int curr_time)
    {
        roots[curr_time] = update(roots[curr_time], idx, val, Lx, Rx);
    }

    Node query(Node *root, int l, int r, ll lx, ll rx)
    {
        if (root == nullptr)
            return new Node(); // Base case for null pointer
        if (lx > r || l > rx)
            return new Node(); // Base case for out-of-range interval
        if (lx >= l && rx <= r)
            return root;
        int mx = (lx + rx) / 2;
        Node L = query(root->left, l, r, lx, mx);
        Node R = query(root->right, l, r, mx + 1, rx);
        return operation(L, R);
    }

    Node query(int l, int r, int time)
    {
        return query(roots[time], l, r, Lx, Rx);
    }
};

int main()
{
    // https://cses.fi/problemset/task/1737/
    int n, q;
    cin >> n >> q;

    vector<ll> a(n);

    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    PersistantSegmentTree pst(n + 1, 1, n);

    pst.build(a);

    // for (int i = 0; i < n; i++)
    // {
    //     cout << pst.query(i + 1, i + 1, 0).val << " ";
    // }
    // cout << endl;

    while (q--)
    {
        int t;
        cin >> t;

        if (t == 1)
        {
            int k, i, x;
            cin >> k >> i >> x;

            k--;

            pst.update(i, x, k);
        }
        else if (t == 2)
        {
            int k, l, r;
            cin >> k >> l >> r;
            k--;

            cout << pst.query(l, r, k).val << "\n";
        }
        else
        {
            int k;
            cin >> k;
            k--;

            pst.roots.push_back(pst.roots[k]);
        }
    }

    return 0;
}
