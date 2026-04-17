#include <bits/stdc++.h>
#define int long long
#define all(x) x.begin(), x.end()
using namespace std;

const int N = 1 << 18;
int t[2*N];

void update(int v) {
	v += N;
	t[v] = 1;
	while (v > 1) {
		v/=2;
		t[v] ++;
	}
}

int query(int v, int l, int r, int lq, int rq) {
	if (lq > rq) return 0;
	if (l == lq && r == rq) return t[v];
	int m = (l+r)/2;
	return query(v*2, l, m, lq, min(m, rq)) + query(v*2+1, m+1, r, max(lq, m+1), rq);
}

void test_case() {
	int n;
	cin >> n;
	vector<pair<int,int>> a(n);
	for (int i = 0; i < n; i ++) {
		cin >> a[i].first;
		a[i].second = i;
	}
	sort(all(a));
	reverse(all(a));

	int ans = 0;
	for (auto it : a) {
		int idx = it.second;
		ans += query(1, 0, N-1, 0, idx);
		update(idx);
	}
	cout << ans << '\n';
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);

	test_case();

	return 0;
}