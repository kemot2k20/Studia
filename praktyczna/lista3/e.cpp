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
const int B = 350;

int x[N];

struct query {
	int l, r, idx;
};

bool cmp(query& X, query& y) {
	if (X.l / B != y.l / B) return X.l / B < y.l / B;
	return ((X.l / B)&1 ? X.r > y.r : X.r < y.r);
}

int cnt[N];
int Ans = 0;

void add(int ind) {
	if (cnt[x[ind]] == x[ind]) Ans --;
	cnt[x[ind]] ++;
	if (cnt[x[ind]] == x[ind]) Ans ++;
}

void remove(int ind) {
	if (cnt[x[ind]] == x[ind]) Ans --;
	cnt[x[ind]] --;
	if (cnt[x[ind]] == x[ind]) Ans ++;
}

int32_t main() {
	ios_base::sync_with_stdio(false);
	cout.tie(0);
	cin.tie(0);

	int n, Q;
	cin >> n >> Q;

	for (int i = 0; i < n; i ++) {
		cin >> x[i];
		x[i] = min(x[i], N-1);
	}

	vector<query> q(Q);
	for (int i = 0; i < Q; i ++) {
		int l, r;
		cin >> l >> r;
		l--, r--;

		q[i] = {l, r, i};
	}

	sort(all(q), cmp);
	
	int cur_l = 0, cur_r = -1;

	vector<int> ans(Q);
	for (int i = 0; i < Q; i ++) {
		int l = q[i].l, r = q[i].r, idx = q[i].idx;

		while (cur_r < r) {
			cur_r ++;
			add(cur_r);
		}
		while (cur_r > r) {
			remove(cur_r);
			cur_r --;
		}
		while (cur_l < l) {
			remove(cur_l);
			cur_l ++;
		}
		while (cur_l > l) {
			cur_l --;
			add(cur_l);
		}

		ans[idx] = Ans;
	}

	for (int i = 0; i < Q; i ++) cout << ans[i] << '\n';

	return 0;
}