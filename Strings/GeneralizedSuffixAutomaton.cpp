#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset> // FIX 1: Included std::bitset to support masks of arbitrary compile-time sizes


/*

You enable bitmasks in a Generalized Suffix Automaton (GSAM) when the problem
requires tracking the exact identities of the strings that contain a given substring, 
rather than just knowing that the substring exists somewhere in the trie.
If a problem only asks for "the number of unique substrings across all strings combined," 
you do not need bitmasks. You only need the bitmask when the problem logic depends on intersections,
unions, or specific string combinations.According to standard 
competitive programming literature (such as CP-Algorithms / E-Maxx on Suffix Automata), 
bitmasks are the standard approach when the number of strings $N$ is relatively 
small (typically $N \le 64$ for integers, or $N \le 1000$ for std::bitset).



"Longest Common Substring of $N$ strings" (where $N > 2$)"
Appears in exactly $K$ strings" or "Appears in at least $K$ strings""
Appears in string $A$ but NOT in string $B$" (Exclusion queries)"
Which strings contain the pattern $P$?" (Multiple queries)"
Number of distinct strings containing..."

*/

using namespace std;

// FIX 2: Converted the struct into a template parameterized by a size_t MAX_STRINGS.
// This allows us to instantiate the GSAM with a generic constant constraint.
template <size_t MAX_STRINGS>
struct GeneralSuffixAutomaton {
    struct State {
        int len, link;
        int next[26];
        bitset<MAX_STRINGS> mask; // FIX 3: Replaced long long with std::bitset<MAX_STRINGS>
        
        // std::bitset is automatically default-initialized to all zeros, 
        // so we don't need to manually clear it in the constructor.
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
    
    void reset() {
        last = 0;
    }

    void extend(char c, int string_id) {
        if (st[last].next[c - 'a'] != -1) {
            int q = st[last].next[c - 'a'];
            if (st[last].len + 1 == st[q].len) {
                last = q;
                // FIX 4: Use .set(index) to turn on the bit for string_id.
                // This replaces the manual bitwise shift (1LL << string_id).
                st[last].mask.set(string_id); 
                return; 
            } else {
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
                st[last].mask.set(string_id); // FIX 4
                return;
            }
        }

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
        st[last].mask.set(string_id); // FIX 4
    }

    void propagateMasks() {
        int max_len = 0;
        for (int i = 0; i < (int)st.size(); i++) max_len = max(max_len, st[i].len);
        
        vector<vector<int>> by_len(max_len + 1);
        for (int i = 1; i < (int)st.size(); i++) {
            by_len[st[i].len].push_back(i);
        }
        
        for (int l = max_len; l >= 1; l--) {
            for (int u : by_len[l]) {
                int p = st[u].link;
                if (p != -1) {
                    // FIX 5: std::bitset overloads the |= operator natively. 
                    // This performs a highly optimized chunk-by-chunk bitwise OR 
                    // between the two bitsets, behaving exactly like an integer OR.
                    st[p].mask |= st[u].mask; 
                }
            }
        }
    }

    long long countDistinctSubstrings() {
        long long total = 0;
        for (int i = 1; i < (int)st.size(); i++) {
            total += st[i].len - st[st[i].link].len; 
        }
        return total;
    }
};

// Define a constant for the maximum number of strings expected in a problem.
const int MAXN = 1005; 

int main() {
    int n;
    cin >> n;
    vector<string> strings(n);
    for (int i = 0; i < n; ++i) {
        cin >> strings[i];
    }
    
    // FIX 6: Instantiate the struct template with our compile-time constant constraint.
    GeneralSuffixAutomaton<MAXN> gsam;

    for(int i = 0; i < n; i++) {
        gsam.reset(); 
        for(auto& c: strings[i]) gsam.extend(c, i);
    }
    
    gsam.propagateMasks();
    
    cout << gsam.countDistinctSubstrings() << endl;
    return 0;
}
