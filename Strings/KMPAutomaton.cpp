#include <bits/stdc++.h>

using  namespace std;


int LAST_J = -1;
string s;

template <typename T> // Can work with string, vector
struct KMPAutomaton
{
    T s;
    vector<int> lps; // longest prefix suffix: lps[i] = length of the longest proper prefix which is also a suffix for the substring s[0..i]
    vector<vector<int>>nxt; // represents the next state (new j) given current state i and next character c (where c goes from 'a' to 'z').
    
    KMPAutomaton(T &s) : s(s) // s is the pattern
    {
        lps.resize(s.size());
        nxt.resize(s.size() + 1, vector<int>(26, 0));

        nxt[0][s[0] - 'a'] = 1;

        for (int i = 1; i <= s.size(); i++)
        {
            for (int c = 0; c < 26; c++)
            {
                if (i < s.size() && s[i] == c + 'a')
                {
                    nxt[i][c] = i + 1;
                }
                else
                {
                    nxt[i][c] = nxt[lps[i - 1]][c];
                }
            }
            if (i < s.size())
            {
                lps[i] = nxt[lps[i - 1]][s[i] - 'a'];
            }
        }
    }
    char eval_j(const string& t, int j) {
        if(j < s.size()) return s[j];
        else return t[j - s.size()];
    }

    void extend_lps(const string& t) {
        int n = t.size();
        int j = lps.back(); // last value of lps for s
        lps.resize(s.size() + n);
        

        for (int i = 0; i < n; i++) {
            while (j >= s.size() && t[i] != eval_j(t,j))
                j = lps[j - 1];

            if( j < s.size()) {
                j = nxt[j][t[i] - 'a'];
            } else j++;

            lps[s.size() + i] = j;
        }
    }

    void shrink() {
        
        lps.resize(s.size());
    }
};


int main() {

    ios_base::sync_with_stdio(false), cin.tie(nullptr) , cout.tie(nullptr);

    cin >> s;

    int q;
    cin >> q;

    KMPAutomaton<string> kmp(s);

    while(q--) {
        string t;
        cin >> t;

        kmp.extend_lps(t);

        for(int i = 0; i < t.size(); i++) {
            cout << kmp.lps[s.size() + i] << " ";
        }
        cout << "\n";

        kmp.shrink();
    }
    


}
