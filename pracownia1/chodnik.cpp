#include <bits/stdc++.h>

using namespace std;

const int N = 200005;
const int M = 10005;

vector<int> parent, mid;
vector<int> L[M], R[M];
vector<pair<int,int>> bloczek;
int found = -1;

void bfs(int st) {
    queue<int> q;
    q.push(st);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        int r = bloczek[v].second;
        if (r == 0) {
            found = v;
            return;
        }

        for (auto u : L[r]) {
            if (parent[u] == -1) {
                parent[u] = v;
                q.push(u);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    cin.tie(0);

    int n;
    cin >> n;

    parent.resize(n, -1);
    mid.resize(n);
    bloczek.resize(n);

    for (int i = 0; i < n; i ++) {
        int l, m, r;
        cin >> l >> m >> r;
        L[l].push_back(i);
        R[r].push_back(i);
        bloczek[i] = {l, r};
        mid[i] = m;
    }

    vector<int> ans;
    for (int i = 0; i < n; i ++) {
        if (parent[i] == -1 && bloczek[i].first == 0) {
            parent[i] = i;
            bfs(i);
            int v = found;
            if (v != -1) {
                while (v != i) {
                    ans.push_back(v);
                    assert(parent[v] != -1);
                    v = parent[v];
                }
                ans.push_back(i);
                break; // znaleziony chodnik
            }
        }
    }

    if ((int)ans.size() > 0) {
        cout << (int)ans.size() << '\n';
        reverse(ans.begin(), ans.end());
        for (auto it : ans) {
            cout << bloczek[it].first << " " << mid[it] << " " << bloczek[it].second << "\n";
        }
    } else {
        cout << "BRAK\n";
    }

    return 0;
}