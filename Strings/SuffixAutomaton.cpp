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
    SuffixAutomaton() {
        st.emplace_back();
        last = 0;
    }
    void extend(char c) {
        int cur = st.size();
        st.emplace_back();
        st[cur].len = st[last].len + 1;
        
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
    long long countDistinctSubstrings() {
        long long total = 0;
        for (int i = 1; i < (int)st.size(); i++) {
            total += st[i].len - st[st[i].link].len; // this is contrubtoin 
            // we have substrings of lenths st[st[i].link].len+1 to st[i].len
        }
        return total;
    }
};
