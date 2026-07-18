struct SuffixAutomaton {
    struct State {
        int len, link;
        int next[26];
        State() : len(0), link(-1) {
            fill(next, next+26, -1);
        }
    };
    vector<State> st;
    int last;

    // FIX 1: Added variables for substring queries. 
    // `pos` stores the state index for the prefix ending at each string index.
    // `up` is the sparse table for binary lifting, and LOG is the maximum jump size.
    vector<int> pos;
    const int LOG = 20; // 2^20 > 10^6, sufficient for string lengths up to 1,000,000
    vector<vector<int>> up;

    SuffixAutomaton() {
        st.emplace_back();
        last = 0;
    }
    
    void extend(char c) {
        int cur = st.size();
        st.emplace_back();
        st[cur].len = st[last].len + 1;
        
        // FIX 2: Store the newly created state `cur` as the state representing 
        // the prefix that ends at the character we just processed.
        pos.push_back(cur);
        
        int p = last;
        while (p != -1 && st[p].next[c - 'a'] == -1) {
            st[p].next[c - 'a'] = cur;
            p = st[p].link;
        }
        
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c - 'a'];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = st.size();
                st.emplace_back(); 
                st[clone].len = st[p].len + 1;
                
                for (int i = 0; i < 26; ++i) {
                    st[clone].next[i] = st[q].next[i];
                }
                st[clone].link = st[q].link;
                
                while (p != -1 && st[p].next[c - 'a'] == q) {
                    st[p].next[c - 'a'] = clone;
                    p = st[p].link;
                }
                
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    // FIX 3: Added method to precompute the binary lifting table.
    // This MUST be called exactly once after the entire string is inserted into the SAM.
    void build_lifting() {
        int n = st.size();
        up.assign(n, vector<int>(LOG, 0));
        
        // Base case: 2^0 ancestors (direct suffix links)
        for (int i = 0; i < n; i++) {
            up[i][0] = (st[i].link == -1) ? 0 : st[i].link;
        }
        
        // Fill the sparse table
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                up[i][j] = up[up[i][j-1]][j-1];
            }
        }
    }

    // FIX 4: Added the query function to find the state for substring S[l..r].
    // It calculates the length L, starts at the prefix state for `r`, and jumps up.
    int get_state(int l, int r) {
        int L = r - l + 1;
        int u = pos[r]; 
        
        for (int j = LOG - 1; j >= 0; j--) {
            int ancestor = up[u][j];
            // If the ancestor exists and can accommodate a string of length L, jump to it
            if (ancestor != 0 && st[ancestor].len >= L) {
                u = ancestor;
            }
        }
        return u;
    }

    long long countDistinctSubstrings() {
        long long total = 0;
        for (int i = 1; i < (int)st.size(); i++) {
            total += st[i].len - st[st[i].link].len; // this is contrubtoin 
            // we have substrings of lenths st[st[i].link].len+1 to st[i].len
        }
        return total;
    }
};
