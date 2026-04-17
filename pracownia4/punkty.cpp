#include <bits/stdc++.h>
using namespace std;

const int N = 1e6;

int n;
int x[N + 5], y[N + 5], where[N + 5];
pair<int,int> X[N + 5], Y[N + 5];

double dist(int i, int j) {
    return sqrt((x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]));
}

pair<int,int> brut(vector<int>& v) {
    int m = v.size();
    double mini = 1e9;
    pair<int,int> closest = {-1,-1};
    for (int i = 0; i < m; i ++) {
        for (int j = i + 1; j < m; j ++) {
            double d = dist(v[i], v[j]);
            if (d < mini) {
                mini = d;
                closest = {v[i], v[j]};
            }
        }
    }
    return closest;
}

pair<int,int> minimum_distance(int indL, int indR) {
    int len = indR - indL + 1;
    if (len <= 1000) {
        vector<int> v;
        for (int i = indL; i <= indR; i ++) v.push_back(X[i].second);
        return brut(v);
    }    

    int mid = (indL+indR)/2;
    vector<int> L, R;
    for(int i = indL; i <= indR; i ++) {
        if (i <= mid) L.push_back(X[i].second);
        else R.push_back(X[i].second);
    }

    pair<int,int> ansL = minimum_distance(indL, mid);
    pair<int,int> ansR = minimum_distance(mid+1, indR);

    double d = dist(ansL.first, ansL.second);
    pair<int,int> closest = ansL;
    double d2 = dist(ansR.first, ansR.second);
    if (d2 < d) {
        d = d2;
        closest = ansR;
    }

    int midx = X[mid].first;
    vector<int> Y2; // punkty ktore sa oddalone od midx o conajwyzej d, posortowane po y.
    for (int i = 0; i < n; i ++) {
        int idx = Y[i].second;
        if (abs(x[idx] - midx) <= d && where[idx] >= where[X[indL].second] && where[idx] <= where[X[indR].second]) Y2.push_back(idx);
    }

    int m = Y2.size();
    for (int i = 0; i < m; i ++) {
        for (int j = i + 1; j < min(m, i + 8); j ++) {
            double dm = dist(Y2[i], Y2[j]);
            if (dm < d) {
                d = dm;
                closest = {Y2[i], Y2[j]};
            }
        }
    }

    return closest;
}

int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    cin.tie(0);

    cin >> n;
    for (int i = 0; i < n; i ++) {
        cin >> x[i] >> y[i];
        X[i] = {x[i], i};
        Y[i] = {y[i], i};
    }

    sort(X, X + n);
    sort(Y, Y + n);
    for (int i = 0; i < n; i ++) where[X[i].second] = i;
    pair<int,int> closest = minimum_distance(0, n-1);
    cout << x[closest.first] << ' ' << y[closest.first] << '\n' << x[closest.second] << ' ' << y[closest.second] << '\n';
    return 0;
}