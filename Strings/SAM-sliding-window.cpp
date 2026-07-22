#include <bits/stdc++.h>

using namespace std;
struct SuffixAutomaton
{
    struct State
    {
        int len, link;
        int next[26];
        int sz;

        int firstpos; // Optional: to store the ending of the first occurrence of the substring represented by this state.
        int lastpos;  // Optional: to store the ending of the last occurrence of the substring represented by this state.

        State() : len(0), link(-1), sz(0)
        {
            fill(next, next + 26, -1);
        }
    };

    vector<State> st;
    int last;
    vector<int> pos; // pos[i] is the state corresponding to the prefix S[0..i]

    SuffixAutomaton()
    {
        st.emplace_back();
        last = 0;
    }

    void extend(char c)
    {
        int cur = st.size();
        st.emplace_back();
        st[cur].len = st[last].len + 1;

        // because it represents a string ending at this brand new position.
        st[cur].sz = 1;

        st[cur].firstpos = st[cur].len - 1; // Optional: store the first position of this substring.
        st[cur].lastpos = st[cur].len - 1;  // Optional: store the last position of this substring.

        pos.push_back(cur);

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

                // They only inherit occurrences from states that link to them later.
                st[clone].sz = 0;

                st[clone].firstpos = st[q].firstpos; // Optional: inherit the first position from q.
                st[clone].lastpos = 0;               // will be updated in the build_frequencies function if needed.

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
    }

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
};

long long substring_count(const SuffixAutomaton &sam, const string &query)
{

    int current_state = 0;
    for (char c : query)
    {
        if (sam.st[current_state].next[c - 'a'] == -1)
        {
            return 0; // The substring does not exist in the original string.
        }
        current_state = sam.st[current_state].next[c - 'a'];
    }
    return sam.st[current_state].sz; // Return the frequency of the substring.
}

signed main()
{
    ios::sync_with_stdio(false), cin.tie(nullptr);

    string s;
    cin >> s;

    SuffixAutomaton sam;
    for (auto c : s)
    {
        sam.extend(tolower(c));
    }

    sam.build_frequencies();

    int q;
    cin >> q;
    vector<int> visited(sam.st.size(), -2); // To avoid counting the same state multiple times

    while (q--)
    {
        string query;
        cin >> query;
        int m = query.size();
        query += query; // Concatenate the query with itself to handle circular substrings

        int current_state = 0;
        int match_length = 0;
        long long total_count = 0;

        int i = 0;

// BLOCK 1: Process the incoming character
        for (char c : query)
        {
            // Convert character to 0-25 index. Using tolower prevents negative index crashes 
            // if an uppercase letter sneaks into the query.
            int idx = tolower(c) - 'a';

            // BLOCK 2: The Fallback (Mismatch Handling)
            // If we are not at the root (0) AND the current state has no outgoing edge for 'idx'...
            while (current_state > 0 && sam.st[current_state].next[idx] == -1)
            {
                // Follow the suffix link to find the next longest valid suffix of what we've matched so far.
                current_state = sam.st[current_state].link;
                
                // Shrink our tracked match length to match the max length of this new suffix state.
                match_length = sam.st[current_state].len;
            }

            // BLOCK 3: The Advancement (Match Handling)
            // If a valid edge for 'idx' exists from our current state (which might be the root)...
            if (sam.st[current_state].next[idx] != -1)
            {
                // Take the transition to the new state.
                current_state = sam.st[current_state].next[idx];
                
                // Increment our matched length because we successfully appended the character.
                match_length++;
            }
            else
            {
                // If we are at the root and STILL have no valid transition, this character 
                // breaks the sequence completely. Reset everything to zero.
                current_state = 0;
                match_length = 0;
            }

            // BLOCK 4: The Trim (Sliding Window Magic)
            // If our matched string is now longer than the required window size 'm'...
            if (match_length > m)
            {
                // Check if the parent state (the suffix link) ALSO contains strings of length 'm'.
                // If its max length is >= m, then our current state only holds strings strictly > m.
                while (current_state > 0 && sam.st[sam.st[current_state].link].len >= m)
                {
                    // Move up the suffix tree to the parent state that actually contains length 'm'.
                    current_state = sam.st[current_state].link;
                }
                
                // Artificially clamp our match length back to 'm'. This simulates "dropping" 
                // the earliest characters from the front of the window.
                match_length = m;
            }

            // BLOCK 5: Tally the Result
            // If we currently have a valid cyclic shift of exactly length 'm', AND we haven't 
            // already counted this exact state during this specific query 'q'...
            if (match_length == m && visited[current_state] != q)
            {
                // Add the frequency of this substring in the original string to our total.
                total_count += sam.st[current_state].sz;
                
                // Mark this state as visited for query 'q' using the O(1) query ID trick, 
                // preventing duplicates from being counted twice.
                visited[current_state] = q;
            }
        }

        cout << total_count << "\n";
    }

    /*

    baabaabaaa
    5
    a
    ba
    baa
    aabaa
    aaba

    */
}
