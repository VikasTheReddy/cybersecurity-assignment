#include <bits/stdc++.h>
using namespace std;

static constexpr long long MOD = 1000000007LL;

long long modPow(long long a, long long e) {
    long long r = 1;
    while (e > 0) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

struct Edge {
    unsigned long long diff;
    int u, v;
    bool operator<(const Edge &other) const { return diff < other.diff; }
};

// DSU with lazy initialization: each XOR group touches only its own vertices.
struct SparseDSU {
    vector<int> parent, sz, stamp;
    int epoch = 0;

    explicit SparseDSU(int n) : parent(n), sz(n), stamp(n, -1) {}

    void beginGroup() { ++epoch; }

    void touch(int x) {
        if (stamp[x] != epoch) {
            stamp[x] = epoch;
            parent[x] = x;
            sz[x] = 1;
        }
    }

    int find(int x) {
        touch(x);
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<unsigned long long> c(n);
    for (auto &x : c) cin >> x;

    vector<Edge> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        edges.push_back({c[u] ^ c[v], u, v});
    }
    sort(edges.begin(), edges.end());

    const long long allX = modPow(2, k);
    const long long pow2n = modPow(2, n);
    long long answer = 0;

    SparseDSU dsu(n);
    int groups = 0;

    for (int i = 0; i < m; ) {
        int j = i;
        dsu.beginGroup();
        long long successfulUnions = 0;
        while (j < m && edges[j].diff == edges[i].diff) {
            if (dsu.unite(edges[j].u, edges[j].v)) ++successfulUnions;
            ++j;
        }
        ++groups;

        // For this x, endpoints of every edge with c[u]^c[v] = x
        // must either both be infected or both remain uninfected.
        // Thus every connected component is an independent binary choice.
        long long components = n - successfulUnions;
        answer += modPow(2, components);
        answer %= MOD;
        i = j;
    }

    // For every x not appearing as an edge XOR, there are no constraints
    // and all 2^n infection subsets are valid.
    long long absentX = (allX - groups) % MOD;
    if (absentX < 0) absentX += MOD;
    answer = (answer + absentX * pow2n) % MOD;

    cout << answer << '\n';
    return 0;
}
