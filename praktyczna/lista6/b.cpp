#include <bits/stdc++.h>
#define int long long
using namespace std;

const int N = 1 << 18;
int t[2*N];

void update(int v, int l, int r, int lq, int rq, int x) {
	if (lq > rq) return;
	if (l == lq && r == rq) {
		t[v] += x;
		return;
	}
	int m = (l+r)/2;
	update(v*2, l, m, lq, min(m, rq), x);
	update(v*2+1, m+1, r, max(lq, m+1), rq, x);
}

int query(int v) {
	if (v == 0) return 0;
	return t[v] + query(v/2);
}

void test_case() {
	int n,q;
	cin >> n >> q;
	for (int i = 0; i < n; i ++) cin >> t[i+N];
	while (q--) {
		int op;
		cin >> op;
		if (op == 1) {
			int l, r, v;
			cin >> l >> r >> v;
			l --, r --;
			update(1, 0, N-1, l, r, v);
		} else {
			int k;
			cin >> k;
			k --;
			cout << query(k+N) << '\n';
		}
	}
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);
	
	test_case();

	return 0;
}