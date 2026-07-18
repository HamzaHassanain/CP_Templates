struct SuffixAutomaton {
    struct State {
        int len, link;
        int next[26];
        // FIX 1: Added `sz` to store the size of the endpos set (the frequency of the substrings in this state).
        int sz; 
        
        State() : len(0), link(-1), sz(0) {
            fill(next, next+26, -1);
        }
    };
    
    vector<State> st;
    int last;
    vector<int> pos;
    const int LOG = 20; 
    vector<vector<int>> up;

    SuffixAutomaton() {
        st.emplace_back();
        last = 0;
    }
    
    void extend(char c) {
        int cur = st.size();
        st.emplace_back();
        st[cur].len = st[last].len + 1;
        
        // FIX 2: A newly created state for a prefix gets an initial frequency of 1, 
        // because it represents a string ending at this brand new position.
        st[cur].sz = 1; 
        
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
                
                // FIX 3: Cloned states do NOT represent a new end position. 
                // They only inherit occurrences from states that link to them later.
                st[clone].sz = 0; 
                
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

    // FIX 4: The Merge Function. We use a counting sort (O(N) time) to sort the 
    // states by length. Then we iterate backwards (longest to shortest) and push 
    // the sizes up the suffix link tree.
    void build_frequencies() {
        int n = st.size();
        vector<int> cnt(st[last].len + 1, 0);
        vector<int> order(n);
        
        // 1. Count occurrences of each length
        for (int i = 0; i < n; i++) {
            cnt[st[i].len]++;
        }
        // 2. Prefix sums to find positions
        for (int i = 1; i <= st[last].len; i++) {
            cnt[i] += cnt[i - 1];
        }
        // 3. Place states in the sorted array
        for (int i = 0; i < n; i++) {
            order[--cnt[st[i].len]] = i;
        }
        
        // 4. Traverse in reverse topological order (longest strings first)
        for (int i = n - 1; i > 0; i--) {
            int u = order[i];
            if (st[u].link != -1) {
                st[st[u].link].sz += st[u].sz;
            }
        }
    }

    void build_lifting() {
        int n = st.size();
        up.assign(n, vector<int>(LOG, 0));
        
        for (int i = 0; i < n; i++) {
            up[i][0] = (st[i].link == -1) ? 0 : st[i].link;
        }
        for (int j = 1; j < LOG; j++) {
            for (int i = 0; i < n; i++) {
                up[i][j] = up[up[i][j-1]][j-1];
            }
        }
    }

    int get_state(int l, int r) {
        int L = r - l + 1;
        int u = pos[r]; 
        
        for (int j = LOG - 1; j >= 0; j--) {
            int ancestor = up[u][j];
            if (ancestor != 0 && st[ancestor].len >= L) {
                u = ancestor;
            }
        }
        return u;
    }
    
    // FIX 5: A helper function to wrap the logic for the user.
    // It finds the state for S[l..r] and returns its merged endpos size.
    int get_substring_frequency(int l, int r) {
        int state_id = get_state(l, r);
        return st[state_id].sz;
    }
};
