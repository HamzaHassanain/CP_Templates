#include <bits/stdc++.h>
using namespace std;
#define ll long long

// Preferable Not to use a struct for the segment tree node, for efficiency
struct LazyPropagationSegmentTree // 1-Based Indexing
{
    int size;
    long long DATA_DEFAULT, LAZY_DEFAULT;
    vector<long long> data, lazy;
    void init(int n, long long DATA_DEFAULT = 0, long long LAZY_DEFAULT = -1)
    {
        size = 1;
        this->DATA_DEFAULT = DATA_DEFAULT;
        this->LAZY_DEFAULT = LAZY_DEFAULT;

        while (size < n)
            size *= 2;
        data.assign(2 * size, DATA_DEFAULT);
        lazy.assign(2 * size, LAZY_DEFAULT);
    }
    void workLazy(int node, int beginSeg, int endSeg)
    {
        if (lazy[node] != LAZY_DEFAULT)
        {
            long long dx = lazy[node];
            lazy[node] = LAZY_DEFAULT;

            // set the value of the range that this node represents to dx
            data[node] = dx * (endSeg - beginSeg + 1);

            if (beginSeg != endSeg)
            {
                lazy[2 * node] = dx;     // cascade the update to the children
                lazy[2 * node + 1] = dx; // cascade the update to the children
            }
        }
    }
    void set(int left, int right, int node, int beginSeg, int endSeg, long long value)
    {
        workLazy(node, beginSeg, endSeg);

        if (beginSeg > right || left > endSeg)
            return;

        if (beginSeg >= left && endSeg <= right)
        {

            lazy[node] = value; // This overwrites the previous value
            workLazy(node, beginSeg, endSeg);

            return;
        }

        int mid = (beginSeg + endSeg) / 2;
        this->set(left, right, 2 * node, beginSeg, mid, value);
        this->set(left, right, 2 * node + 1, mid + 1, endSeg, value);

        data[node] = data[2 * node] + data[2 * node + 1];
    }

    long long query(int left, int right, int node, int beginSeg, int endSeg)
    {
        workLazy(node, beginSeg, endSeg);

        if (beginSeg > right || left > endSeg)
            return DATA_DEFAULT;

        if (beginSeg >= left && endSeg <= right)
            return data[node];

        int mid = (beginSeg + endSeg) / 2;
        long long a = this->query(left, right, 2 * node, beginSeg, mid);
        long long b = this->query(left, right, 2 * node + 1, mid + 1, endSeg);

        return a + b;
    }
    long long query(int left, int right)
    {

        return query(left, right, 1, 1, size);
    }
    void set(int left, int right, long long value)
    {
        set(left, right, 1, 1, size, value);
    }
};

int main()
{
    vector<long long> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    LazyPropagationSegmentTree st;

    st.init(a.size());

    for (int i = 0; i < a.size(); i++)
    {
        st.set(i + 1, i + 1, a[i]);
    }
    st.set(1, 9, 10);
    cout << st.query(1, 9) << endl; // 90
    st.set(1, 1, 1);
    st.set(9, 9, 9);
    st.set(2, 8, 0);
    cout << st.query(1, 9) << endl; // 10
    return 0;
}