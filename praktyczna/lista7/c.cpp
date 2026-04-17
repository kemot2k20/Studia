#include <stdio.h>
#include <math.h>
using namespace std;

const int N = 1 << 16;
int t[N<<1], lazy[N<<1];

void propagate(int v) {
	lazy[v<<1] += lazy[v];
	lazy[(v<<1)|1] += lazy[v];
	t[v<<1] += lazy[v];
	t[(v<<1)|1] += lazy[v];
	lazy[v] = 0;
}

int query(int v, int l, int r, int lq, int rq) {
	if (lq > rq) return 0;
	if (l == lq && r == rq) return t[v];
	propagate(v);
	int m = (l+r)>>1;
	return max(query(v<<1, l, m, lq, min(m, rq)), query((v<<1)|1, m+1, r, max(m+1, lq), rq));
}

void update(int v, int l, int r, int lq, int rq, int x) {
	if (lq > rq) return;
	if (l == lq && r == rq) {
		lazy[v] += x;
		t[v] += x;
		return;
	}
	propagate(v);
	int m = (l+r)>>1;
	update(v<<1, l, m, lq, min(m, rq), x);
	update((v<<1)|1, m+1, r, max(m+1, lq), rq, x);
	t[v] = max(t[v<<1], t[(v<<1)|1]);
}

int main() {
	int n, m, q;
	scanf("%d%d%d", &n, &m, &q);

	while (q--) {
		int l, r, x;
		scanf("%d%d%d", &l, &r, &x);
		l--, r--;
		int mx = query(1, 0, N-1, l, r-1);
		if (mx + x <= m) {
			printf("T\n");
			update(1, 0, N-1, l, r-1, x);
		} else printf("N\n");
	}

	return 0;
}