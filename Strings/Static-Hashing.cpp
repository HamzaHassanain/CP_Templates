#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll mod = 1e9 + 7;
const ll nod = 1e9 + 11;
const ll base = 31;
const ll maxn = 1e6 + 5;
ll p1[maxn], p2[maxn];
void calc()
{
    p1[0] = 1;
    p2[0] = 1;
    for (int i = 1; i < maxn; i++)
    {
        p1[i] = (p1[i - 1] * base) % mod;
        p2[i] = (p2[i - 1] * base) % nod;
    }
}
struct Hash
{
    vector<ll> h1, h2;
    Hash(string &s)
    {
        int n = s.size();

        h1.resize(n + 1);
        h2.resize(n + 1);

        for (int i = 0; i < n; i++)
        {
            h1[i + 1] = (h1[i] * base + s[i] - 'a' + 1) % mod;
            h2[i + 1] = (h2[i] * base + s[i] - 'a' + 1) % nod;
        }
    }
    pair<ll, ll> query(int l, int r)
    {

        return {
            (h1[r] - (h1[l - 1] * p1[r - l + 1]) % mod + mod) % mod,
            (h2[r] - (h2[l - 1] * p2[r - l + 1]) % nod + nod) % nod};
    }
};