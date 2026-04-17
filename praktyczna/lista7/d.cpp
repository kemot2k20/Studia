#include <stdio.h>
#include <math.h>
using namespace std;

const int N = 1 << 18;
long long t[2*N], lazy[2*N][2];

void build() {
	for (int i = N - 1; i >= 1; i --) t[i] = t[i<<1] + t[(i<<1)|1];
}

void propagate(int v, int len) {
	len >>= 1;
	if (lazy[v][1]) {
		lazy[v<<1][0] = lazy[(v<<1)|1][0] = 0;
		lazy[v<<1][1] = lazy[(v<<1)|1][1] = lazy[v][1];
		t[v<<1] = t[(v<<1)|1] = lazy[v][1] * len;
	}
	lazy[v<<1][0] += lazy[v][0];
	lazy[(v<<1)|1][0] += lazy[v][0];
	t[v<<1] += lazy[v][0] * len;
	t[(v<<1)|1] += lazy[v][0] * len;
	lazy[v][0] = lazy[v][1] = 0;
}

long long query(int v, int l, int r, int lq, int rq) {
	if (lq > rq) return 0;
	if (l == lq && r == rq) return t[v];
	propagate(v, r-l+1);
	int m = (l+r)>>1;
	return query(v<<1, l, m, lq, min(m, rq)) + query((v<<1)|1, m+1, r, max(m+1, lq), rq);
}

void update_add(int v, int l, int r, int lq, int rq, int x) {
	if (lq > rq) return;
	if (l == lq && r == rq) {
		lazy[v][0] += x;
		t[v] += 1LL * x * (r-l+1);
		return;
	}
	propagate(v, r-l+1);
	int m = (l+r)>>1;
	update_add(v<<1, l, m, lq, min(m, rq), x);
	update_add((v<<1)|1, m+1, r, max(m+1, lq), rq, x);
	t[v] = t[v<<1] + t[(v<<1)|1];
}

void update_set(int v, int l, int r, int lq, int rq, int x) {
	if (lq > rq) return;
	if (l == lq && r == rq) {
		lazy[v][1] = x;
		lazy[v][0] = 0;
		t[v] = 1LL * x * (r-l+1);
		return;
	}
	propagate(v, r-l+1);
	int m = (l+r)>>1;
	update_set(v<<1, l, m, lq, min(m, rq), x);
	update_set((v<<1)|1, m+1, r, max(m+1, lq), rq, x);
	t[v] = t[v<<1] + t[(v<<1)|1];
}

int main() {
	int n, q;
	scanf("%d%d", &n, &q);

	for (int i = 0; i < n; i ++) scanf("%d", &t[i + N]);
	build();

	while (q--) {
		int op;
		scanf("%d", &op);
		if (op == 1) {
			int l, r, x;
			scanf("%d%d%d", &l, &r, &x);
			l--, r--;
			update_add(1, 0, N-1, l, r, x);
		} else if (op == 2) {
			int l, r, x;
			scanf("%d%d%d", &l, &r, &x);
			l--, r--;
			update_set(1, 0, N-1, l, r, x);
		} else {
			int l, r;
			scanf("%d%d", &l, &r);
			l--, r--;
			printf("%lld\n", query(1, 0, N-1, l, r));
		}
	}

	return 0;
}