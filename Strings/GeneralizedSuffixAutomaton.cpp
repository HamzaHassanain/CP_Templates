#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>

using namespace std;

template <size_t MAX_STRINGS>
struct GeneralSuffixAutomaton
{
    struct State
    {
        int len, link, sz;
        int next[26];
        bitset<MAX_STRINGS> mask;

        int str_idx, end_pos;

        // FIX 1: Initialize sz to 0 in the constructor.
        // Failing to do this leaves garbage values in memory, ruining the DP addition later.
        State() : len(0), link(-1), sz(0), str_idx(-1), end_pos(-1)
        {
            fill(next, next + 26, -1);
        }
    };

    vector<State> st;
    int last;

    GeneralSuffixAutomaton()
    {
        st.emplace_back();
        last = 0;
    }

    void reset()
    {
        last = 0;
    }

    void extend(char c, int string_id, int current_pos)
    {
        if (st[last].next[c - 'a'] != -1)
        {
            int q = st[last].next[c - 'a'];
            if (st[last].len + 1 == st[q].len)
            {
                last = q;
                st[last].mask.set(string_id);

                st[last].end_pos = current_pos;
                st[last].str_idx = string_id;

                st[last].sz++;
                return;
            }
            else
            {
                int clone = st.size();
                st.emplace_back();
                st[clone].len = st[last].len + 1;

                for (int i = 0; i < 26; ++i)
                {
                    st[clone].next[i] = st[q].next[i];
                }
                st[clone].link = st[q].link;

                st[clone].end_pos = st[q].end_pos;
                st[clone].str_idx = st[q].str_idx;

                int p = last;
                while (p != -1 && st[p].next[c - 'a'] == q)
                {
                    st[p].next[c - 'a'] = clone;
                    p = st[p].link;
                }

                st[q].link = clone;
                last = clone;
                st[last].mask.set(string_id);
                st[last].sz++;
                return;
            }
        }

        int cur = st.size();
        st.emplace_back();
        st[cur].len = st[last].len + 1;
        st[cur].end_pos = current_pos;
        st[cur].str_idx = string_id;

        int p = last;
        while (p != -1 && st[p].next[c - 'a'] == -1)
        {
            st[p].next[c - 'a'] = cur;
            p = st[p].link;
        }

        if (p == -1)
        {
            st[cur].link = 0;
        }
        else
        {
            int q = st[p].next[c - 'a'];
            if (st[p].len + 1 == st[q].len)
            {
                st[cur].link = q;
            }
            else
            {
                int clone = st.size();
                st.emplace_back();

                st[clone].len = st[p].len + 1;
                st[clone].end_pos = st[q].end_pos;
                st[clone].str_idx = st[q].str_idx;

                for (int i = 0; i < 26; ++i)
                {
                    st[clone].next[i] = st[q].next[i];
                }
                st[clone].link = st[q].link;

                while (p != -1 && st[p].next[c - 'a'] == q)
                {
                    st[p].next[c - 'a'] = clone;
                    p = st[p].link;
                }

                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
        st[last].mask.set(string_id);
        st[last].sz++;
    }

    void build_frequencies_and_masks()
    {
        int n = st.size();

        int max_len = 0;
        for (int i = 0; i < n; i++)
        {
            max_len = max(max_len, st[i].len);
        }

        vector<int> cnt(max_len + 1, 0);
        vector<int> order(n);

        for (int i = 0; i < n; i++)
        {
            cnt[st[i].len]++;
        }
        for (int i = 1; i <= max_len; i++)
        {
            cnt[i] += cnt[i - 1];
        }
        for (int i = 0; i < n; i++)
        {
            order[--cnt[st[i].len]] = i;
        }

        for (int i = n - 1; i > 0; i--)
        {
            int u = order[i];
            int p = st[u].link;

            if (p != -1)
            {
                st[p].sz += st[u].sz;
                st[p].mask |= st[u].mask;
            }
        }
    }

    long long countDistinctSubstrings()
    {
        long long total = 0;
        for (int i = 1; i < (int)st.size(); i++)
        {
            total += st[i].len - st[st[i].link].len;
        }
        return total;
    }
};

const int MAXN = 11;

int main()
{

    vector<string> strings;

    string s;

    while (cin >> s)
    {
        strings.push_back(s);
    }

    int n = strings.size();

    GeneralSuffixAutomaton<MAXN> gsam;

    for (int i = 0; i < n; i++)
    {
        gsam.reset();
        for (int j = 0; j < strings[i].size(); j++)
            gsam.extend(strings[i][j], i, j);
    }

    gsam.build_frequencies_and_masks();

    int best_state = 0;
    int max_lcs_len = 0;

    for (int i = 1; i < gsam.st.size(); i++)
    {
        if (gsam.st[i].mask.count() == n)
        {
            if (gsam.st[i].len > max_lcs_len)
            {
                max_lcs_len = gsam.st[i].len;
                best_state = i;
            }
        }
    }
    if (max_lcs_len == 0)
    {
        cout << 0;
    }
    else
    {
        int string_id = gsam.st[best_state].str_idx;
        int end_index = gsam.st[best_state].end_pos;
        int start_index = end_index - max_lcs_len + 1;

        string lcs = strings[string_id].substr(start_index, max_lcs_len);
        cout << lcs.size();
    }

    return 0;
}
