
struct XorBasis
{
    vector<ll> Basis;

    XorBasis()
    {
        Basis = vector<ll>();
    }

    void insert(ll mask)
    {
        for (ll base : Basis)
            mask = min(mask, mask ^ base);
        if (mask)
            Basis.push_back(mask);
    }

    bool is_doable(ll mask)
    {
        for (ll base : Basis)
            mask = min(mask, mask ^ base);
        return mask == 0;
    }

    ll kth_number(ll k) // returns kth largest number that can be formed by the basis
    {
        ll ans = 0;
        for (ll bit = 60; ~bit; bit--)
        {
            ll cnt = 0;
            for (ll base : Basis)
                cnt += (base >> bit) & 1;
            if (cnt < k && (ans ^ (1LL << bit)) <= ans)
            {
                ans ^= (1LL << bit);
                k -= cnt;
            }
        }
        return ans;
    }
    ll kth_smallest_number(ll k) // returns kth smallest number that can be formed by the basis
    {
        return kth_number(cnt_numbers() - k + 1);
    }
    ll cnt_numbers() // returns the number of numbers that can be formed by the basis
    {
        return (1LL << Basis.size());
    }
};

XorBasis operator+(XorBasis a, XorBasis b)
{
    if (a.Basis.size() < b.Basis.size())
        swap(a, b);

    for (ll mask : b.Basis)
        a.insert(mask);

    return a;
}

XorBasis operator&(XorBasis a, ll mask)
{
    XorBasis res;
    for (ll base : a.Basis)
        res.insert(base & mask);
    return res;

    return res;
}