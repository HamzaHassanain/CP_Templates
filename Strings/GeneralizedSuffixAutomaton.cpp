#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset> 

using namespace std;

template <size_t MAX_STRINGS>
struct GeneralSuffixAutomaton {
    struct State {
        int len, link, sz;
        int next[26];
        bitset<MAX_STRINGS> mask; 
        
        // FIX 1: Initialize sz to 0 in the constructor. 
        // Failing to do this leaves garbage values in memory, ruining the DP addition later.
        State() : len(0), link(-1), sz(0) { 
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
                st[last].mask.set(string_id); 
                // FIX 2: We traversed an existing path for a new prefix. 
                // This state represents a valid ending position, so we increment its occurrence count.
                st[last].sz++; 
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
                st[last].mask.set(string_id); 
                // FIX 3: The clone acts as the separated ending position for the current string's prefix.
                // We increment its size. The original state `q` retains its own previously counted `sz`.
                st[last].sz++; 
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
                // FIX 4: Note that we DO NOT increment sz for this clone. 
                // This clone is purely structural for the DAG suffix links.
                // Its sz remains 0, which is correct.
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
        st[last].mask.set(string_id); 
        // FIX 5: This newly created state represents the ending position of the current prefix.
        st[last].sz++; 
    }

    void build_frequencies_and_masks() {
        int n = st.size();
        
        int max_len = 0;
        for (int i = 0; i < n; i++) {
            max_len = max(max_len, st[i].len);
        }
        
        vector<int> cnt(max_len + 1, 0); 
        vector<int> order(n);
        
        for (int i = 0; i < n; i++) {
            cnt[st[i].len]++;
        }
        for (int i = 1; i <= max_len; i++) { 
            cnt[i] += cnt[i - 1];
        }
        for (int i = 0; i < n; i++) {
            order[--cnt[st[i].len]] = i;
        }
        
        for (int i = n - 1; i > 0; i--) {
            int u = order[i];
            int p = st[u].link;
            
            if (p != -1) {
                // FIX 6: The DP step safely pushes the accumulated sizes and masks 
                // up the suffix link tree. The sz logic you provided here was already perfect.
                st[p].sz += st[u].sz;       
                st[p].mask |= st[u].mask;   
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

const int MAXN = 11; 

int main() {
    int n;
    cin >> n;
    vector<string> strings(n);
    for (int i = 0; i < n; ++i) {
        cin >> strings[i];
    }
    
    GeneralSuffixAutomaton<MAXN> gsam;

    for(int i = 0; i < n; i++) {
        gsam.reset(); 
        for(auto& c: strings[i]) gsam.extend(c, i);
    }
    
    gsam.build_frequencies_and_masks();
    
    // FIX 7: Added an example query to demonstrate the difference between mask and sz.
    // Let's find the frequency and string spread of the character 'a'.
    int state_for_a = gsam.st[0].next['a' - 'a'];
    if (state_for_a != -1) {
        cout << "The substring 'a' appears in " << gsam.st[state_for_a].mask.count() << " distinct strings." << endl;
        cout << "The substring 'a' appears a TOTAL of " << gsam.st[state_for_a].sz << " times across all strings." << endl;
    }
   
    return 0;
}
