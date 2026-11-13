#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> adj;
vector<int> parentFromRoot, subtreeSize;
vector<long long> memo;

// Maximum sum obtainable by assigning consecutive smallest labels to the
// edges of a simple path whose endpoints are u and v. The path can be
// extended at either endpoint; the recurrence is evaluated by removing
// one outer edge at a time.
long long solve(int u, int v) {
    if (u == v) return 0;
    size_t id = (size_t)u * n + v;
    long long &result = memo[id];
    if (result != -1) return result;

    int nextU = parentFromRoot[(size_t)v * n + u]; // first step u -> v
    int nextV = parentFromRoot[(size_t)u * n + v]; // first step v -> u

    // Number of unordered vertex pairs whose path contains the whole
    // u-v path: choose one endpoint in each outer component.
    long long ways = 1LL * subtreeSize[(size_t)v * n + u]
                         * subtreeSize[(size_t)u * n + v];

    result = ways + max(solve(nextU, v), solve(u, nextV));
    memo[(size_t)v * n + u] = result; // the state is symmetric
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    adj.assign(n, {});
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    const size_t total = (size_t)n * n;
    parentFromRoot.assign(total, -1);
    subtreeSize.assign(total, 1);
    memo.assign(total, -1);

    // For every possible root, compute parent and subtree-size tables.
    vector<int> order;
    order.reserve(n);
    for (int root = 0; root < n; ++root) {
        size_t row = (size_t)root * n;
        order.clear();
        order.push_back(root);
        parentFromRoot[row + root] = -1;

        for (size_t i = 0; i < order.size(); ++i) {
            int u = order[i];
            for (int v : adj[u]) {
                if (v == parentFromRoot[row + u]) continue;
                parentFromRoot[row + v] = u;
                order.push_back(v);
            }
        }

        for (int i = n - 1; i > 0; --i) {
            int u = order[i];
            int p = parentFromRoot[row + u];
            subtreeSize[row + p] += subtreeSize[row + u];
        }
    }

    long long answer = 0;
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            answer = max(answer, solve(u, v));
        }
    }

    cout << answer << '\n';
    return 0;
}
