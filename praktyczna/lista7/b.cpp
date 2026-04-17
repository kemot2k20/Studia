#include <stdio.h>
#include <math.h>
using namespace std;

const int N = 1 << 18;
int t[N<<1];

void build() {
	for (int i = N - 1; i >= 1; i --) t[i] = max(t[i<<1], t[(i<<1)|1]); 
}

int query(int v, int l, int r, int x) {
	if (l == r) return v;
	int m = (l+r)>>1;
	if (t[v<<1] >= x) return query(v<<1, l, m, x);
	return query((v<<1)|1, m+1, r, x);
}

void update(int v, int x) {
	t[v] -= x;
	while (v > 1) {
		v>>=1;
		t[v] = max(t[v<<1], t[(v<<1)|1]);
	}
}

int main() {
	int n, q;
	scanf("%d%d", &n, &q);
	for (int i = 0; i < n; i ++) scanf("%d", &t[i + N]);
	build();

	while (q--) {
		int x;
		scanf("%d", &x);
		if (t[1] < x) printf("0 ");
		else {
			int ind = query(1, 0, N-1, x);
			printf("%d ", ind - N + 1);
			update(ind, x);
		}
	}
	printf("\n");

	return 0;
}