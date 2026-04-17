#include <bits/stdc++.h>
using namespace std;

#define int long long

void test_case() {
	int n;
	cin >> n;

	vector<int> x(n), m(n);
	for (int i = 0; i < n; i ++) {
		cin >> m[i] >> x[i];
	}

	int ans = x[0], lcm = m[0];
	for (int i = 1; i < n; i ++) {
		while (ans % m[i] != x[i]) {
			ans += lcm;
		}
		lcm *= m[i];
	}

	cout << ans << '\n';
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);

	int t=1;
	cin >> t;
	while(t--)test_case();

	return 0;
}