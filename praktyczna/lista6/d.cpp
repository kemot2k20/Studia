#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Node {
	int sum, max_pref, max_suf, max_seg;
};

const int N = 1 << 18;
Node t[2*N];

Node merge(Node& a, Node& b) {
	Node c;
	c.sum = a.sum + b.sum;
	c.max_pref = max(a.max_pref, a.sum + b.max_pref);
	c.max_suf = max(b.max_suf, b.sum + a.max_suf);
	c.max_seg = max({a.max_seg, b.max_seg, a.max_suf + b.max_pref});
	return c;
}

void upd(int v, int x) {
	v += N;
	t[v].sum = x;
	t[v].max_pref = max(0LL, x);
	t[v].max_suf = max(0LL, x);
	t[v].max_seg = max(0LL, x);
	while (v > 1) {
		v /= 2;
		t[v] = merge(t[v*2], t[v*2+1]);
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
		int v, x;
		cin >> v >> x;
		v--;
		upd(v, x);
		vec.clear();
		query(1, 0, N-1, 0, n-1);
		Node res = vec[0];
		for (int i = 1; i < (int)vec.size(); i ++) res = merge(res, vec[i]);
		cout << res.max_seg << '\n';
	}
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);

	test_case();

	return 0;
}