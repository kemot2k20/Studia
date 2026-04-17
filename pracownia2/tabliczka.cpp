#include<iostream>
#include<utility>
using namespace std;

const int MAXM = 1e6;

int M, k;
pair<long long, int> heap[MAXM+1];

void init() {
    for (int i = M; i >= 1; i --) {
        heap[M-i+1] = {1LL * i * M, i};
    }
}

pair<long long, int> get_max() {
    return heap[1];
}

void move_down(int i) {
    int j = i;
    do {
        j = i;
        if (2*j <= M && heap[2*j].first > heap[i].first) i = 2*j;
        if (2*j+1 <= M && heap[2*j+1].first > heap[i].first) i = 2*j+1;
        swap(heap[i], heap[j]);
    } while (j != i);
}

void insert(long long x, int col) {
    heap[1] = {x, col};
    move_down(1);
}

int main() {
    ios_base::sync_with_stdio(0);
    cout.tie(0);
    cin.tie(0);

    cin >> M >> k;

    init();
    long long last = 0;
    int cnt = 0;

    while (cnt < k) {
        pair<long long, int> maks = get_max();
        if (maks.first != last) {
            cout << maks.first << '\n';
            last = maks.first;
            cnt ++;
        }

        long long val = maks.first;
        int col = maks.second;
        if (val / col - 1 >= col) insert(1LL * col * (val / col - 1), col);
        else insert(-1, col);
    }

    return 0;
}