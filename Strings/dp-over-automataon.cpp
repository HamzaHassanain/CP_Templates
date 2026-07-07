#include <bits/stdc++.h>

using  namespace std;


template <typename T> // Can work with string, vector
struct KMPAutomaton
{
    T pattern;
    vector<int> lps; // longest prefix suffix: lps[i] = length of the longest proper prefix which is also a suffix for the substring s[0..i]
    vector<vector<int>>nxt; // represents the next state (new j) given current state i and next character c (where c goes from 'a' to 'z').
    
    KMPAutomaton(T &s) : pattern(s) // s is the pattern
    {
        lps.resize(pattern.size());
        nxt.resize(pattern.size() + 1, vector<int>(26, 0));

        nxt[0][pattern[0] - 'a'] = 1;

        for (int i = 1; i <= pattern.size(); i++)
        {
            for (int c = 0; c < 26; c++)
            {
                if (i < pattern.size() && pattern[i] == c + 'a')
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


    int max_replacement(string& text) {
        vector<vector<int>>dp;
        dp = vector<vector<int>>(text.size() + 1, vector<int>(pattern.size() + 1, -1));


        for(int j = 0; j <= pattern.size(); j++) {
            dp[text.size()][j] = 0;
        }
        
        int n = text.size();
        int m = pattern.size();

        for(int i = n-1;  i >= 0; i--) {
            for(int j = 0; j <= m; j++) {
                if(text[i] == '?') {
                    int ans = 0;
                    for(char ch = 'a';ch <= 'z';ch++) {

                            int nj = nxt[j][ch - 'a'];
                            if(nj == pattern.size()) {
                                ans = max(ans, 1 + dp[i + 1][lps[nj - 1]]);
                            } else {
                                ans = max(ans, dp[i + 1][nj]);
                            }
                    }

                    dp[i][j] = ans;

                } else {
                    int nj = nxt[j][text[i] - 'a'];
                    if(nj == pattern.size()) {
                        dp[i][j] = 1 + dp[i + 1][lps[nj - 1]];
                    } else {
                        dp[i][j] = dp[i + 1][nj];
                    }
                }
            }
        }

        return dp[0][0];
    }
};

int main() {

    ios_base::sync_with_stdio(false), cin.tie(nullptr) , cout.tie(nullptr);

    string s,t,pattern;

    cin >> s >> pattern;


    // KMP<string> kmp(pattern);
    // auto text  = longestCommonSubsequence(s, t);
    // vector<int> matches = kmp.match(text);

    // for(int i = 0; i < matches.size(); i++) {
    //     cout << matches[i] << " ";
    // }

    // cout << "\n";
    KMPAutomaton<string> kmp(pattern);
    cout << kmp.max_replacement(s) << "\n";

}
