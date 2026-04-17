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

const int N = 32000;
int p[N];

vi pre() {
	for (int i = 2; i < N; i ++) p[i] = i;
	for (int i = 2; i * i < N; i ++) {
		if (p[i] == i) {
			for (int j = i * 2; j < N; j += i) {
				if (p[j] == j) p[j] = i;
			} 
		}
	}
	vi Primes;
	Primes.pb(1);
	for (int i = 2; i < N; i ++) {
		if (p[i] == i) Primes.pb(i);
	}
	return Primes;
}

void test_case() {
	int n;
	cin >> n;
	vi a(n+2), b(n+2);
	for (int i = 1; i <= n; i ++) cin >> a[i];
	for (int i = 1; i <= n; i ++) cin >> b[i];

	vi Primes = pre();
	vector<vi> dp(n+1, vi(30)), new_a(n+1, vi(30));
	for (int i = 1; i <= n; i ++) {
		for (int j = 0; j < 30; j ++) {
			int g = (i > 1 ? __gcd(a[i], a[i-1]) : 1);
			int g2 = (i < n ? __gcd(a[i], a[i+1]) : 1);
			ll lcm = 1LL*g*g2/__gcd(g, g2) * Primes[j];
			
			int mx = -1;
			bool flag = (lcm != a[i] && lcm <= b[i]);
			new_a[i][j] = (flag ? lcm : a[i]);
			for (int k = 0; k < 30; k ++) {
				if (i == 1 || __gcd(new_a[i-1][k], new_a[i][j]) == g) {
					mx = max(mx, dp[i-1][k]);
				}
			}
			if (mx == -1) dp[i][j] = 0;
			else dp[i][j] = mx + flag;
		}
	}
	int res = 0;
	for (int j = 0; j < 30; j ++) res = max(res, dp[n][j]);
	cout << res << '\n';
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