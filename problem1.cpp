#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, l;
    cin >> n >> k >> l;

    vector<int> target(n + 2, 0);
    for (int i = 0; i < k; ++i) {
        int x;
        cin >> x;
        target[x] = 1;
    }

    vector<int> lengths(l);
    for (int &x : lengths) cin >> x;
    sort(lengths.begin(), lengths.end());
    lengths.erase(unique(lengths.begin(), lengths.end()), lengths.end());

    // A flip of [L, R] toggles exactly two difference-array positions:
    // L and R+1. The required positions are therefore the odd-degree
    // vertices of a minimum T-join in the graph of panel boundaries.
    vector<int> boundary;
    for (int p = 1; p <= n + 1; ++p) {
        if (target[p] != target[p - 1]) boundary.push_back(p);
    }

    const int m = (int)boundary.size();
    const int INF = 1e9;
    vector<vector<int>> dist(m, vector<int>(m, INF));

    // Vertices are boundaries 1..n+1. An operation of length a
    // connects boundary p with p+a.
    for (int s = 0; s < m; ++s) {
        vector<int> d(n + 2, -1);
        queue<int> q;
        d[boundary[s]] = 0;
        q.push(boundary[s]);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int a : lengths) {
                int v1 = u - a;
                int v2 = u + a;
                if (v1 >= 1 && d[v1] == -1) {
                    d[v1] = d[u] + 1;
                    q.push(v1);
                }
                if (v2 <= n + 1 && d[v2] == -1) {
                    d[v2] = d[u] + 1;
                    q.push(v2);
                }
            }
        }
        for (int t = 0; t < m; ++t) {
            if (d[boundary[t]] != -1) dist[s][t] = d[boundary[t]];
        }
    }

    // Pair all required odd-degree boundaries at minimum total distance.
    // There are at most 2*k <= 20 such boundaries.
    const int FULL = (1 << m) - 1;
    vector<int> memo(1 << m, -1);
    function<int(int)> solve = [&](int mask) -> int {
        if (mask == FULL) return 0;
        int &ans = memo[mask];
        if (ans != -1) return ans;
        ans = INF;

        int i = 0;
        while (mask & (1 << i)) ++i;
        int withI = mask | (1 << i);
        for (int j = i + 1; j < m; ++j) {
            if (mask & (1 << j)) continue;
            if (dist[i][j] == INF) continue;
            int rest = solve(withI | (1 << j));
            if (rest != INF) ans = min(ans, dist[i][j] + rest);
        }
        return ans;
    };

    int answer = solve(0);
    cout << (answer == INF ? -1 : answer) << '\n';
    return 0;
}
