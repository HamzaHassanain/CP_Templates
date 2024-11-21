#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct Node
{
    long long val;
    Node() { val = 0; }
    Node(long long val) : val(val) {}
};
struct Operation
{
    virtual Node work(Node, Node) { return Node(); }
};
struct SegmentTree // 1-Based Indexing
{
    int size;
    Node DEFAULT;
    vector<Node> data;
    Operation *operation;
    void init(int n, Node DEFAULT, Operation *operation)
    {
        size = 1;
        this->operation = operation;
        this->DEFAULT = DEFAULT;
        while (size < n)
            size *= 2;
        data.assign(2 * size, Node());
    }
    void set(int i, Node value, int node, int beginSeg, int endSeg)
    {
        if (beginSeg == endSeg)
        {
            data[node] = value;
            return;
        }

        int mid = (beginSeg + endSeg) / 2;
        if (i <= mid)
            set(i, value, 2 * node, beginSeg, mid);
        else
            set(i, value, 2 * node + 1, mid + 1, endSeg);

        data[node] = operation->work(data[2 * node], data[2 * node + 1]);
    }

    Node query(int left, int right, int node, int beginSeg, int endSeg)
    {
        if (beginSeg > right || left > endSeg)
            return DEFAULT;

        if (beginSeg >= left && endSeg <= right)
            return data[node];

        int mid = (beginSeg + endSeg) / 2;
        Node a = this->query(left, right, 2 * node, beginSeg, mid);
        Node b = this->query(left, right, 2 * node + 1, mid + 1, endSeg);

        return operation->work(a, b);
    }
    Node query(int left, int right)
    {

        return query(left, right, 1, 1, size);
    }
    void set(int i, Node value)
    {
        set(i, value, 1, 1, size);
    }
};
struct SumOperation : Operation
{
    Node work(Node a, Node b)
    {
        return Node(a.val + b.val);
    }
};

int main()
{

    int n;

    cin >> n;

    vector<ll> a(n);

    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    SegmentTree st;
    st.init(n, Node(0), new SumOperation());

    for (int i = 0; i < n; i++)
    {
        st.set(i + 1, Node(a[i]));
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
            cout << st.query(l, r).val << '\n';
        }
        else
        {
            int idx;
            ll val;
            cin >> idx >> val;
            st.set(idx, Node(val));
        }
    }
    return 0;
}
