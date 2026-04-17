#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Node {
	int sum, max_pref;
};

const int N = 1 << 18;
Node t[2*N];

void upd(int v, int x) {
	v += N;
	t[v].sum = x;
	t[v].max_pref = max(0LL, x);
	while (v > 1) {
		v /= 2;
		t[v].sum = t[v*2].sum + t[v*2+1].sum;
		t[v].max_pref = max(t[v*2].max_pref, t[v*2].sum + t[v*2+1].max_pref);
	}
}

vector<Node> vec;
void query(int v, int l, int r, int lq, int rq) {
	if (lq > rq) return;
	if (l == lq && r == rq) {
		vec.push_back(t[v]);
		return;
	}
	int m = (l+r)/2;
	query(v*2, l, m, lq, min(m, rq));
	query(v*2+1, m+1, r, max(lq, m+1), rq);
}

void test_case() {
	int n, q;
	cin >> n >> q;
	for (int i = 0; i < n; i ++) {
		int x;
		cin >> x;
		upd(i, x);
	}
	while (q--) {
		int op;
		cin >> op;
		if (op == 1) {
			int v, x;
			cin >> v >> x;
			v--;
			upd(v, x);
		} else {
			int l, r;
			cin >> l >> r;
			l--, r--;
			vec.clear();
			query(1, 0, N-1, l, r);
			int res = 0, S = 0;
			for (auto it : vec) {
				res = max(res, S + it.max_pref);
				S += it.sum;
			}
			cout << res << '\n';
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