#include <bits/stdc++.h>
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>
#define INF 1e9
#define INFl 1e18
#define all(x) x.begin(), x.end()
#define sajz(x) (int)x.size()
#define pb push_back
#define s second
#define f first
#define yes puts("YES")
#define no puts("NO")
#define erase_duplicates(x) {sort(all(x));(x).resize(distance((x).begin(), unique(all(x))));}
using namespace std;
//using namespace __gnu_pbds;
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}

template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif

#define int long long

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef set<int> si;
typedef multiset<int> msi;
typedef long double ld;
//typedef cc_hash_table<int, int, hash<int>> ht

const int N = 2e5 + 5;
const int B = 450;

int a[N], b[N/B + 1];

int get(int l, int r) {
	int sum = 0;
	int kl = l / B, kr = r / B;
	if (kl == kr) {
		for (int i = l; i <= r; i ++) sum += a[i];
	} else {
		for (int i = kl + 1; i <= kr - 1; i ++) sum += b[i];
		for (int i = l; i < (kl + 1) * B; i ++) sum += a[i];
		for (int i = kr * B; i <= r; i ++) sum += a[i];
	}
	return sum;
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);

	int n, q;
	cin >> n >> q;

	for (int i = 0; i < n; i ++) {
		cin >> a[i];
		b[i / B] += a[i];
	}

	while (q--) {
		int op;
		cin >> op;
		if (op == 1) {
			int k, u;
			cin >> k >> u;
			k --;
			b[k / B] += u - a[k];
			a[k] = u;
		} else {
			int aa, bb;
			cin >> aa >> bb;
			aa--, bb--;

			cout << get(aa, bb) << '\n';
		}
	}

	return 0;
}