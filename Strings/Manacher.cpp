template <typename T> // Can work with string, vector
struct Manacher
{

    // manacher_odd, manacher_even
    vector<int> m_odd, m_even;
    int n;

    Manacher(T &s)
    {
        n = s.size();
        m_odd.resize(s.size());
        m_even.resize(s.size());

        // manacher odd
        for (int i = 0, l = 0, r = -1; i < s.size(); i++)
        {
            int k = (i > r) ? 1 : min(m_odd[l + r - i], r - i + 1);
            while (0 <= i - k && i + k < s.size() && s[i - k] == s[i + k])
                k++;
            m_odd[i] = k--;
            if (i + k > r)
                l = i - k, r = i + k;
        }

        // manacher even
        for (int i = 0, l = 0, r = -1; i < s.size(); i++)
        {
            int k = (i > r) ? 0 : min(m_even[l + r - i + 1], r - i + 1);
            while (0 <= i - k - 1 && i + k < s.size() && s[i - k - 1] == s[i + k])
                k++;
            m_even[i] = k--;
            if (i + k > r)
                l = i - k - 1, r = i + k;
        }

        // m_odd[i] = length of the longest odd palindrome centered at i
        // m_even[i] = length of the longest even palindrome centered at i
    }

    vector<pair<int, int>> get_odd_ls_rs()
    {
        vector<pair<int, int>> ans;
        for (int i = 0; i < n; i++)
        {
            if (m_odd[i] == 0)
                continue;
            int l = i - m_odd[i] + 1;
            int r = i + m_odd[i] - 1;
            ans.push_back({l, r});
        }

        return ans;
    }

    vector<pair<int, int>> get_even_ls_rs()
    {
        vector<pair<int, int>> ans;
        for (int i = 0; i < n; i++)
        {
            if (m_even[i] == 0)
                continue;
            int l = i - m_even[i];
            int r = i + m_even[i] - 1;
            ans.push_back({l, r});
        }

        return ans;
    }
};
