

#include <bits/stdc++.h>

using namespace std;
#define int long long
struct SuffixAutomaton
{
    struct State
    {
        int len, link;
        int next[30];
        // FIX 1: Added `sz` to store the size of the endpos set (the frequency of the substrings in this state).
        int sz;

        int firstpos; // Optional: to store the ending of the first occurrence of the substring represented by this state.
        int lastpos;  // Optional: to store the ending of the last occurrence of the substring represented by this state.

        State() : len(0), link(-1), sz(0)
        {
            fill(next, next + 30, -1);
        }
    };

    vector<State> st;
    int last;
    vector<int> pos; // pos[i] is the state corresponding to the prefix S[0..i]
    const int LOG = 20;
    vector<vector<int>> up;

    SuffixAutomaton()
    {
        st.emplace_back();
        last = 0;
    }

    void extend(char c)
    {
        int c_index = c == '#' ? 26 : c - 'a'; // Handle the separator character
        int cur = st.size();
        st.emplace_back();
        st[cur].len = st[last].len + 1;

        // FIX 2: A newly created state for a prefix gets an initial frequency of 1,
        // because it represents a string ending at this brand new position.
        st[cur].sz = 1;

        st[cur].firstpos = st[cur].len - 1; // Optional: store the first position of this substring.
        st[cur].lastpos = st[cur].len - 1;  // Optional: store the last position of this substring.

        pos.push_back(cur);

        int p = last;
        while (p != -1 && st[p].next[c_index] == -1)
        {
            st[p].next[c_index] = cur;
            p = st[p].link;
        }

        if (p == -1)
        {
            st[cur].link = 0;
        }
        else
        {
            int q = st[p].next[c_index];
            if (st[p].len + 1 == st[q].len)
            {
                st[cur].link = q;
            }
            else
            {
                int clone = st.size();
                st.emplace_back();
                st[clone].len = st[p].len + 1;

                // FIX 3: Cloned states do NOT represent a new end position.
                // They only inherit occurrences from states that link to them later.
                st[clone].sz = 0;

                st[clone].firstpos = st[q].firstpos; // Optional: inherit the first position from q.
                st[clone].lastpos = 0;               // will be updated in the build_frequencies function if needed.

                for (int i = 0; i <= 26; ++i)
                {
                    st[clone].next[i] = st[q].next[i];
                }
                st[clone].link = st[q].link;

                while (p != -1 && st[p].next[c_index] == q)
                {
                    st[p].next[c_index] = clone;
                    p = st[p].link;
                }

                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    // FIX 4: The Merge Function. We use a counting sort (O(N) time) to sort the
    // states by length. Then we iterate backwards (longest to shortest) and push
    // the sizes up the suffix link tree.
    void build_frequencies()
    {
        int n = st.size();
        vector<int> cnt(st[last].len + 1, 0);
        vector<int> order(n);

        // 1. Count occurrences of each length
        for (int i = 0; i < n; i++)
        {
            cnt[st[i].len]++;
        }
        // 2. Prefix sums to find positions
        for (int i = 1; i <= st[last].len; i++)
        {
            cnt[i] += cnt[i - 1];
        }
        // 3. Place states in the sorted array
        for (int i = 0; i < n; i++)
        {
            order[--cnt[st[i].len]] = i;
        }

        // 4. Traverse in reverse topological order (longest strings first)
        for (int i = n - 1; i > 0; i--)
        {
            int u = order[i];
            if (st[u].link != -1)
            {
                st[st[u].link].sz += st[u].sz;

                // merge lastpos information if needed
                st[st[u].link].lastpos = max(st[st[u].link].lastpos, st[u].lastpos);
            }
        }
    }

    void build_lifting()
    {
        int n = st.size();
        up.assign(n, vector<int>(LOG, 0));

        for (int i = 0; i < n; i++)
        {
            up[i][0] = (st[i].link == -1) ? 0 : st[i].link;
        }
        for (int j = 1; j < LOG; j++)
        {
            for (int i = 0; i < n; i++)
            {
                up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }
    }

    int get_state(int l, int r)
    {
        int L = r - l + 1;
        int u = pos[r];

        for (int j = LOG - 1; j >= 0; j--)
        {
            int ancestor = up[u][j];
            if (ancestor != 0 && st[ancestor].len >= L)
            {
                u = ancestor;
            }
        }
        return u;
    }

    // FIX 5: A helper function to wrap the logic for the user.
    // It finds the state for S[l..r] and returns its merged endpos size.
    int get_substring_frequency(int l, int r)
    {
        int state_id = get_state(l, r);
        return st[state_id].sz;
    }
};
signed main()
{
    ios::sync_with_stdio(false), cin.tie(nullptr);

    string s, t;
    cin >> s >> t;

    string combined = s + "#" + t; // Use a separator to avoid overlap

    SuffixAutomaton sam;
    for (auto c : combined)
    {
        sam.extend((c));
    }

    sam.build_frequencies();

    int ans = 0;
    for (int i = 1; i < sam.st.size(); i++)
    {
        int st_len = sam.st[i].len; // Length of the substring represented by this state

        int firstpos = sam.st[i].firstpos;             // First position of the substring in the combined string
        int lastpos = sam.st[i].lastpos;               // Last position of the substring\

        if (firstpos < s.size() && lastpos > s.size()) // Check if the substring occurs in both s and t
        {
            ans = max(ans, st_len);
        }
    }
    cout << ans << "\n";
}
