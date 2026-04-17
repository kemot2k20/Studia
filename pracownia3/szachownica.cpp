#include <bits/stdc++.h>
using namespace std;

int n, p, m;
vector<int> valid[1<<10];
int wzorek[105];

void pre() {
    for (int mask = 0; mask < 1 << 15; mask ++) {
        bool ok = true;
        for (int i = 0; i < p; i ++) {
            if (wzorek[i] == (mask&((1<<9)-1)) || 
                wzorek[i] == ((mask>>3)&((1<<9)-1)) ||
                wzorek[i] == (mask>>6))
                {
                    ok = false;
                    break;
                }
        }
        if (ok) {
            int mask1 = 0, mask2 = 0;
            for (int b = 0; b < 5; b ++) {
                if (mask&(1<<(3*b))) mask1 |= (1<<b);
                if (mask&(1<<(3*b+1))) mask1 |= (1<<(b+5));
                if (mask&(1<<(3*b+2))) mask2 |= (1<<(b+5));
            }
            valid[mask1].push_back(mask2);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false); cout.tie(0); cin.tie(0);
    cin >> n >> p >> m;
    for (int i = 0; i < p; i ++) {
        for (int y = 0; y < 3; y ++) {
            for (int x = 0; x < 3; x ++) {
                char c;
                cin >> c;
                if (c=='x') wzorek[i] |= (1<<(y*3+x));
            }
        }
    }
    pre();
    vector<int> dp(1<<10, 1);
    for (int i = 3; i <= n; i ++) {
        vector<int> ndp(1<<10);
        for (int mask = 0; mask < 1<<10; mask ++) {
            for (int col : valid[mask]) {
                int nmask = (mask>>5) | col;
                ndp[nmask] += dp[mask];
                if (ndp[nmask]>=m) ndp[nmask]-=m;
            }
        }
        dp = ndp;
    }
    int ans = 0;
    for (int mask = 0; mask < 1<<10; mask ++) {
        ans += dp[mask];
        if (ans>=m) ans-=m;
    }
    cout << ans << '\n';
}