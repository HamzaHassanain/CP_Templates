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

        // if the kth is required, we must normalize
        normalize();
    }

    void normalize()
    {
        for (int i = 0; i < (int)Basis.size(); ++i)
        {
            for (int j = 0; j < (int)Basis.size(); ++j)
            {
                if (i != j && (Basis[j] & highest_bit_mask(Basis[i])) != 0)
                {
                    Basis[j] ^= Basis[i];
                }
            }
        }
        sort(Basis.begin(), Basis.end(),
             [&](ll x, ll y)
             {
                 return highest_bit_index(x) > highest_bit_index(y);
             });
    }
    ll highest_bit_mask(ll x)
    {
        return (1LL << (63 - __builtin_clzll(x)));
    }
    ll highest_bit_index(ll x)
    {
        return 63 - __builtin_clzll(x);
    }

    bool is_doable(ll mask)
    {
        for (ll base : Basis)
            mask = min(mask, mask ^ base);
        return mask == 0;
    }
    ll max_number()
    {
        ll ans = 0;
        for (ll base : Basis)
            ans = max(ans, ans ^ base);
        return ans;
    }
    ll cnt_eqaul_x(ll x, ll N) // (n is the total number of the set)
    {
        ll ans = 0;
        for (ll base : Basis)
            x = min(x, x ^ base);
        if (x == 0)
            return power(2, N - Basis.size());

        return 0;
    }
    ll kth_smallest_number(ll k) // k is zero based
    {

        ll res = 0;
        for (int i = 0; i < (int)Basis.size(); ++i)
        {
            if (k & (1LL << i))
                res ^= Basis[Basis.size() - i - 1];
        }
        return res;
    }
    ll kth_largest_number(ll k) // k is zero based
    {
        return kth_smallest_number(cnt_numbers() - k - 1);
    }
    ll cnt_numbers()
    {
        return (1LL << Basis.size());
    }
};
