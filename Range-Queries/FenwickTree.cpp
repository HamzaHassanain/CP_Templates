#include <bits/stdc++.h>
using namespace std;

// Template Begins

struct FenwickTree // 0-based indexing
{
    vector<int> data;
    int n;

    FenwickTree(int n) : n(n)
    {
        data = vector<int>(n, 0);
    }

    FenwickTree(vector<int> a)
    {

        n = a.size();
        data = vector<int>(n, 0);

        for (int i = 0; i < a.size(); i++)
            add(i, a[i]);
    }

    int sum(int r)
    {
        int ans = 0;

        while (r >= 0)
        {
            ans += data[r];
            r = (r & (r + 1)) - 1;
        }

        return ans;
    }

    int sum(int l, int r)
    {
        return sum(r) - sum(l - 1);
    }

    void add(int i, int x)
    {
        while (i < n)
        {
            data[i] += x;
            i = i | (i + 1);
        }
    }

    void set(int i, int x)
    {
        add(i, x - sum(i, i));
    }
};
// Template Ends

int main()
{
    vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8};
    FenwickTree ft(a);

    cout << ft.sum(0, 7) << endl; // 36
    cout << ft.sum(0, 3) << endl; // 10

    ft.add(3, 10); // increase the value at index 3 by 10

    cout << ft.sum(3, 3) << endl; // 14
    cout << ft.sum(0, 7) << endl; // 46
    cout << ft.sum(0, 3) << endl; // 20

    ft.set(3, 10);
    cout << ft.sum(3, 3) << endl; // 10
    cout << ft.sum(0, 7) << endl; // 42
    cout << ft.sum(5, 7) << endl; // 21

    return 0;
}
