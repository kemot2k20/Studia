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

//#define int long long

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef set<int> si;
typedef multiset<int> msi;
typedef long double ld;
//typedef cc_hash_table<int, int, hash<int>> ht

const int N = 1e5 + 5;

int uf[N], cnt[N];

int Find(int x) {
	if (uf[x] == x) return x;
	uf[x] = Find(uf[x]);
	return uf[x];
}

void Union(int a, int b) {
	a = Find(a);
	b = Find(b);
	if (cnt[a] < cnt[b]) uf[a] = b, cnt[b] += cnt[a];
	else uf[b] = a, cnt[a] += cnt[b];
}

void test_case() {
	int n, m;
	cin >> n >> m;

	for (int i = 1; i <= n; i ++) uf[i] = i, cnt[i] = 1;

	vector<pii> v(m);
	vi a(m), b(m);
	for (int i = 0; i < m; i ++) {
		cin >> a[i] >> b[i];
		int c; cin >> c;
		v[i] = {c, i};
	}
	sort(all(v));

	int ans = 0;
	for (int i = 0; i < m; i ++) {
		int cost = v[i].first;
		int idx = v[i].second;
		if (Find(a[idx]) != Find(b[idx])) {
			ans += cost;
			Union(a[idx], b[idx]);
		}
	}

	cout << ans << '\n';
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);

	int t=1;
	//cin >> t;
	while(t--)test_case();

	return 0;
}