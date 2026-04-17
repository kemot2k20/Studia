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
int ans[N], a[N], suf[N], pref[N];
int n;

struct query {
	int l, r, idx;
};

void solve(int l, int r, vector<query>& queries) {
	if (l > r || queries.empty()) return;
	int m = (l+r)/2;
	vector<query> L, R, M;
	for (auto q : queries) {
		if (q.r < m) L.pb(q);
		else if (q.l > m) R.pb(q);
		else M.pb(q);
	}

	suf[m] = a[m];
	for (int i = m-1; i >= l; i --) suf[i] = min(suf[i+1], a[i]);
	pref[m] = a[m];
	for (int i = m+1; i <= r; i ++) pref[i] = min(pref[i-1], a[i]);

	for (auto q : M) {
		ans[q.idx] = min(suf[q.l], pref[q.r]);
	}

	solve(l, m-1, L);
	solve(m+1, r, R);
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);

	int q;
	cin >> n >> q;
	for (int i = 0; i < n; i ++) cin >> a[i];
	vector<query> queries;
	for (int i = 0; i < q; i ++) {
		int l, r;
		cin >> l >> r;
		l--, r--;
		queries.pb({l, r, i});
	}
	for (int i = 0; i < q; i ++) ans[i] = 1e9;
	solve(0, n-1, queries);

	for (int i = 0; i < q; i ++) cout << ans[i] << '\n';

	return 0;
}