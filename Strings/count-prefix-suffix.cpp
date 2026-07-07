#include <bits/stdc++.h>

using  namespace std;



vector<int> occ_dp(string s) {
    int n = s.size();
    KMP kmp(s);
    auto& lcp = kmp.lps;

    auto occ = vector<int>(n + 1, 0);
    for(int i = 0; i < n; i++) {
        occ[lcp[i]]++;
    }

    for(int i = n-1; i > 0; i--) {
        occ[lcp[i - 1]] += occ[i];
    }
    for(int i = 0; i <= n; i++) {
        occ[i]++;
    }

    return occ;
}

vector<int>valid_lengths(string s) {
    int n = s.size();
    KMP kmp(s);
    auto& lcp = kmp.lps;
    vector<int>res;

    int curr_len = n;

    while(curr_len> 0) {
        res.push_back(curr_len);
        curr_len = lcp[curr_len - 1];
    }

    reverse(res.begin(), res.end());

    return res;
}
int main() {

    ios_base::sync_with_stdio(false), cin.tie(nullptr);

    string s;
    cin >> s;

    auto occ = occ_dp(s);
    auto valid = valid_lengths(s);

    cout << valid.size() << "\n";
    for(int i = 0; i < valid.size(); i++) {
        cout << valid[i] << " " << occ[valid[i]] << "\n";
    }



}
