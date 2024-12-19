#include <bits/stdc++.h>
using namespace std;

int main () {
    int tr, tc, td, fr, fc, fd;
    // tr = Matches (True) in row
    // tc = Matches (True) in column
    // td = Matches (True) in diagonal
    // fr = Doesn't match (False) in row
    // fc = Doesn't match (False) in column
    // fd = Doesn't match (False) in diagonal
    string s1, s2;
    cin >> s1 >> s2;
    int n, m;
    n = s1.length();
    m = s2.length();
    cout << n << " " << m << endl;
    int table[n][m];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (s1[i] == s2[j]) {
                if (i == 0 && j == 0) {
                    table[i][j] = 0;
                    break;
                }
                if (j > 0) {
                    tr = table[i][j-1];
                } else {tr = INT_MAX; td = INT_MAX; }
                if (i > 0) {
                    tc = table[i-1][j];
                } else {tc = INT_MAX; td = INT_MAX; }
                if (i > 0 && j > 0) {
                    td = table[i-1][j-1];
                }
                table[i][j] = min({tr, tc, td});

            } else {
                if (i == 0 && j == 0) {
                    table[i][j] = 1;
                    break;
                }
                if (j > 0) {
                    fr = table[i][j-1] + 1;
                } else { fr = INT_MAX; fd = INT_MAX; }
                if (i > 0) {
                    fc = table[i-1][j] + 1;
                } else { fc = INT_MAX; fd = INT_MAX; }
                if (i > 0 && j > 0) {
                    fd = table[i-1][j-1] + 1;
                }
                table[i][j] = min({fr, fc, fd});
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << table[i][j] << " " ;
        }
        cout << endl;
    }

    return 0;
}