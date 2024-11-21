#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
struct BinaryTrie
{

    struct Node
    {
        Node *child[2];
        ll cnt = 0;
        Node()
        {
            child[0] = child[1] = nullptr;
        }
    };

    Node *root = new Node();

    void insert(ll x)
    {
        Node *cur = root;
        for (ll i = 60; i >= 0; i--)
        {
            bool b = (1ll << i) & x;
            if (cur->child[b] == nullptr)
            {
                cur->child[b] = new Node();
            }
            cur = cur->child[b];
            cur->cnt++;
        }
    }

    void erase(ll x)
    {
        Node *cur = root;
        for (ll i = 60; i >= 0; i--)
        {
            bool b = (1ll << i) & x;
            cur = cur->child[b];
            cur->cnt--;
        }
    }
    ll maxXor(ll x)
    {
        Node *cur = root;
        ll ans = 0;
        for (ll i = 60; i >= 0; i--)
        {
            bool b = (1ll << i) & x;
            if (cur->child[b ^ 1] != nullptr)
            {
                ans |= (1ll << i);
                cur = cur->child[b ^ 1];
            }
            else
            {
                cur = cur->child[b];
            }
        }
        return ans;
    }
    ll minXor(ll x)
    {
        Node *cur = root;
        ll ans = 0;
        for (ll i = 60; i >= 0; i--)
        {
            bool b = (1ll << i) & x;
            if (cur->child[b] != nullptr)
            {
                cur = cur->child[b];
            }
            else
            {
                ans |= (1ll << i);
                cur = cur->child[b ^ 1];
            }
        }
        return ans;
    }
    ll cntXorLessThanK(ll x, ll k)
    {
        Node *cur = root;
        ll ans = 0;
        for (ll i = 60; i >= 0; i--)
        {
            if (cur == nullptr)
                break;
            bool b = (1ll << i) & x;
            bool kb = (1ll << i) & k;
            if (kb)
            {
                if (cur->child[b] != nullptr)
                {
                    ans += cur->child[b]->cnt;
                }
                cur = cur->child[1 - b];
            }
            else
            {
                cur = cur->child[b];
            }
        }
        if (cur)
            ans += cur->cnt;
        return ans;
    }
    ll cntGreaterThanK(ll x, ll k)
    {
        Node *cur = root;
        ll ans = 0;
        ll curXor = 0;

        for (ll i = 60; i >= 0; i--)
        {
            if (not cur)
                break;
            bool b = (1ll << i) & x;
            if ((curXor | (1ll << i)) >= k)
            {
                ans += cur->child[b ^ 1] ? cur->child[b ^ 1]->cnt : 0;
                cur = cur->child[b];
            }
            else
            {
                cur = cur->child[b ^ 1];
                curXor |= (1ll << i);
            }
        }

        if (curXor >= k)
            ans += cur->cnt;

        return ans;
    }
};
