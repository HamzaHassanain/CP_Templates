template <typename T> // Can work with string, vector
struct Z_Algo
{

    vector<int> z_function(T &s)
    {
        int n = s.size();
        vector<int> z(n);
        for (int i = 1, l = 0, r = 0; i < n; i++)
        {
            if (i <= r)
                z[i] = min(r - i + 1, z[i - l]);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]])
                z[i]++;
            if (i + z[i] - 1 > r)
                l = i, r = i + z[i] - 1;
        }
        return z;
    }

    // like kmp stuff
    vector<int> z_function(T &s, T &t, function<T(T &, T &)> combine)
    {
        T str = combine(s, t); // t + "$" + s
        vector<int> z = z_function(str);

        vector<int> res;
        for (int i = t.size() + 1; i < z.size(); i++)
            if (z[i] == t.size())
                res.push_back(i - t.size() - 1);
        return res;
    }
};
