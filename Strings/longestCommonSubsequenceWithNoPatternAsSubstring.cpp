#include <bits/stdc++.h>

using  namespace std;


template <typename T> // Can work with string, vector
struct KMPAutomaton
{
    T pattern;
    vector<int> lps; // longest prefix suffix: lps[i] = length of the longest proper prefix which is also a suffix for the substring s[0..i]
    vector<vector<int>>nxt; // represents the next state (new j) given current state i and next character c (where c goes from 'A' to 'z').
    
    KMPAutomaton(T &s) : pattern(s) // s is the pattern
    {
        lps.resize(pattern.size());
        nxt.resize(pattern.size() + 1, vector<int>(26, 0));

        nxt[0][pattern[0] - 'A'] = 1;

        for (int i = 1; i <= pattern.size(); i++)
        {
            for (int c = 0; c < 26; c++)
            {
                if (i < pattern.size() && pattern[i] == c + 'A')
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
                lps[i] = nxt[lps[i - 1]][s[i] - 'A'];
            }
        }
    }
};

string longestCommonSubsequenceWithNoPatternAsSubstring(string &s, string &t , string& pattern) {

    KMPAutomaton<string> kmp(pattern);

    int n = s.size() , m = t.size() , p = pattern.size();

    // dp[i][j][k] = length of the longest common subsequence of s[0..i-1] and t[0..j-1] such that the longest prefix of pattern that is a suffix of the subsequence is of length k
    vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(m + 1, vector<int>(p + 1, 0)));
    vector<vector<vector<array<int, 4>>>> parent(n + 1, vector<vector<array<int, 4>>>(m + 1, vector<array<int, 4>>(p + 1)));
    // if kmp.nxt[k][c] == p, then we cannot take this character as it will form the pattern as a substring

   for(int i =n;i>=0;i--) {
    for(int j = m;j>=0;j--) {
        for(int k = 0;k< p;k++)
        {
            if(i < n) {
                dp[i][j][k] = max(dp[i][j][k], dp[i + 1][j][k]);
                if(dp[i][j][k] == dp[i + 1][j][k])
                    parent[i][j][k] = {i + 1, j, k , 0} ;
            }
            if(j < m) {
                dp[i][j][k] = max(dp[i][j][k], dp[i][j + 1][k]);
                if(dp[i][j][k] == dp[i][j + 1][k])
                    parent[i][j][k] = {i, j + 1, k , 0} ;
            }
            if(i < n && j < m && s[i] == t[j]) {
                int next_k = kmp.nxt[k][s[i] - 'A'];
                if(next_k < p) {
                    dp[i][j][k] = max(dp[i][j][k], 1 + dp[i + 1][j + 1][next_k]);
                    if(dp[i][j][k] == 1 + dp[i + 1][j + 1][next_k])
                        parent[i][j][k] = {i + 1, j + 1, next_k , 1} ;
                }
            }
        }
    }
    
}

    string ans = "";

    int i = 0 , j = 0 , k = 0;
    while(i < n && j < m) {
        auto [ni, nj, nk , take] = parent[i][j][k];
        if(take) {
            ans += s[i];
        }

        i = ni;
        j = nj;
        k = nk;
    }

    if(ans.empty()) {
        return "0";
    }

    return ans;
} 


int main() {

    ios_base::sync_with_stdio(false), cin.tie(nullptr) , cout.tie(nullptr);

    string s,t,pattern;

    cin >> s >> t >> pattern;


    // KMP<string> kmp(pattern);
    auto text  = longestCommonSubsequenceWithNoPatternAsSubstring(s, t , pattern    );
  
    cout << text << "\n";

}
