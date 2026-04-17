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
int a[N];

void merge(int l, int r) {
	int mid = (l + r) / 2;

	int len1 = mid - l + 1;
	int len2 = r - mid;

	vi L(len1), R(len2);
	for (int i = 0; i < len1; i ++) {
		L[i] = a[l + i];
	}
	for (int i = 0; i < len2; i ++) {
		R[i] = a[mid + 1 + i];
	}

	int i = 0, j = 0, k = l;
	while (k <= r) {
		if (i < len1 && (j >= len2 || L[i] <= R[j])) {
			a[k] = L[i];
			i ++;
			k ++;
		} else {
			a[k] = R[j];
			j ++;
			k ++;
		}
	}
}

void mergesort(int l, int r) {
	if (l >= r) {
		return;
	}

	int mid = (l + r) / 2;
	mergesort(l, mid);
	mergesort(mid+1, r);
	merge(l, r);
}

void test_case() {
	int n;
	cin >> n;
	for (int i = 1; i <= n; i ++) {
		cin >> a[i];
	}

	mergesort(1, n);
	for (int i = 1; i <= n; i ++) {
		cout << a[i] << ' ';
	}
	cout << '\n';
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