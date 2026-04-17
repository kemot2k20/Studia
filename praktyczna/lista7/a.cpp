#include <stdio.h>
using namespace std;

const int N = 1 << 18;
int a[200000], t[N<<1];

void build() {
	for (int i = N - 1; i >= 1; i --) t[i] = t[i<<1] + t[(i<<1)|1]; 
}

int query(int v, int k) {
	if (v >= N) return v;
	if (t[v<<1] >= k) return query(v<<1, k);
	return query((v<<1)|1, k - t[v<<1]);
}

void update(int v, int x) {
	t[v] = x;
	while (v > 1) {
		v>>=1;
		t[v] = t[v<<1] + t[(v<<1)|1];
	}
}

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i ++) {
		scanf("%d", &a[i]);
		update(i+N, 1);
	}

	for (int i = 0; i < n; i ++) {
		int k;
		scanf("%d", &k);
		int ind = query(1, k);
		printf("%d ", a[ind-N]);
		update(ind, 0);
	}
	printf("\n");

	return 0;
}