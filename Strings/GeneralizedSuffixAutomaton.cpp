#include <iostream>
#include <vector>
#include <string>

using namespace std;

// TODO: Implement GSAM structure and functions
// Each state should have: len, link, and next[26]
// Provide functions to add a new string and to compute total distinct substrings
// See solution for reference

struct GeneralSuffixAutomaton {
    struct State {
        int len, link;
        int next[26];
        State() : len(0), link(-1) {
            fill(next, next+26, -1);
        }
    };
    vector<State> st;
    int last;
    GeneralSuffixAutomaton() {
        st.emplace_back();
        last = 0;
    }
    
    // reset 'last' back to the root state (0) before processing each new string.
    void reset() {
        last = 0;
    }

    void extend(char c) {
        // If a transition for 'c' already exists from the current 'last' state, 
        // it means another string previously inserted shared this exact prefix.
        if (st[last].next[c - 'a'] != -1) {
            int q = st[last].next[c - 'a'];
            // Case A: The existing state is contiguous. We just advance 'last'.
            if (st[last].len + 1 == st[q].len) {
                last = q;
                return; // MUST return here to prevent creating duplicate states.
            } else {
                // We must clone it to separate the paths, just like in standard SAM.
                int clone = st.size();
                st.emplace_back();
                st[clone].len = st[last].len + 1;
                
                for (int i = 0; i < 26; ++i) {
                    st[clone].next[i] = st[q].next[i];
                }
                st[clone].link = st[q].link;
                
                int p = last;
                while (p != -1 && st[p].next[c - 'a'] == q) {
                    st[p].next[c - 'a'] = clone;
                    p = st[p].link;
                }
                
                st[q].link = clone;
                last = clone;
                return;
            }
        }

        // Standard SAM extension if no transition previously existed.
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

int main() {
    int n;
    cin >> n;
    vector<string> strings(n);
    for (int i = 0; i < n; ++i) {
        cin >> strings[i];
    }
    GeneralSuffixAutomaton gsam;

    for(auto& s : strings) {
        // FIX 1: Reset the last pointer to the root state (0) before traversing a new string.
        // This ensures the strings share the same Trie root and avoids appending 
        // the current string to the tail of the previously processed string.
        gsam.reset(); 
        for(auto& c: s) gsam.extend(c);
    }
    cout << gsam.countDistinctSubstrings() << endl;
    // Build GSAM   
    // Compute result
    // Output result
    return 0;
}
