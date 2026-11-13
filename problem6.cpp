#include <bits/stdc++.h>
using namespace std;

static constexpr long long MOD = 1000000007LL;
static constexpr int MAX_LEN = 100;
static constexpr int MAX_SUM = 25 * MAX_LEN;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ways[len][sum] = number of length-len words whose letter-index sum
    // is sum, where a=0, b=1, ..., z=25.
    // Adjacent operations transfer one unit between neighboring positions,
    // so every bounded composition with the same total sum is reachable.
    static long long ways[MAX_LEN + 1][MAX_SUM + 1]{};
    ways[0][0] = 1;

    for (int len = 1; len <= MAX_LEN; ++len) {
        long long window = 0;
        int previousMax = 25 * (len - 1);
        for (int sum = 0; sum <= 25 * len; ++sum) {
            if (sum <= previousMax) window += ways[len - 1][sum];
            if (sum - 26 >= 0) window -= ways[len - 1][sum - 26];
            window %= MOD;
            if (window < 0) window += MOD;
            ways[len][sum] = window;
        }
    }

    int t;
    cin >> t;
    while (t--) {
        string s;
        cin >> s;
        int sum = 0;
        for (char c : s) sum += c - 'a';
        cout << (ways[s.size()][sum] - 1 + MOD) % MOD << '\n';
    }
    return 0;
}
