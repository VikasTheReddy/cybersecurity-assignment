#include <bits/stdc++.h>
using namespace std;

struct Query { int u, v; };
struct Change { int child, parent, oldSize, oldBad; };

class RollbackParityDSU {
    vector<int> parent, sz, parity;
    vector<Change> history;
public:
    int bad = 0;
    explicit RollbackParityDSU(int n)
        : parent(n + 1), sz(n + 1, 1), parity(n + 1, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    pair<int,int> find(int x) const {
        int p = 0;
        while (parent[x] != x) {
            p ^= parity[x];
            x = parent[x];
        }
        return {x, p};
    }

    void addEdge(int u, int v) {
        auto [ru, pu] = find(u);
        auto [rv, pv] = find(v);
        int oldBad = bad;
        if (ru == rv) {
            if ((pu ^ pv) != 1) ++bad;
            history.push_back({-1, -1, 0, oldBad});
            return;
        }
        if (sz[ru] > sz[rv]) swap(ru, rv);
        history.push_back({ru, rv, sz[rv], oldBad});
        parent[ru] = rv;
        parity[ru] = pu ^ pv ^ 1;
        sz[rv] += sz[ru];
    }

    size_t snapshot() const { return history.size(); }
    void rollback(size_t snap) {
        while (history.size() > snap) {
            Change c = history.back();
            history.pop_back();
            bad = c.oldBad;
            if (c.child != -1) {
                parent[c.child] = c.child;
                parity[c.child] = 0;
                sz[c.parent] = c.oldSize;
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<Query> queries(q);
    unordered_map<unsigned long long, int> opened;
    opened.reserve(2 * q + 1);

    int base = 1;
    while (base < q) base <<= 1;
    vector<vector<pair<int,int>>> seg(2 * base);

    auto keyOf = [](int u, int v) -> unsigned long long {
        if (u > v) swap(u, v);
        return (static_cast<unsigned long long>(u) << 32) | (unsigned int)v;
    };
    auto addInterval = [&](int l, int r, int u, int v) {
        // Inclusive time interval [l, r], stored on an iterative segment tree.
        int L = l + base, R = r + 1 + base;
        while (L < R) {
            if (L & 1) seg[L++].push_back({u, v});
            if (R & 1) seg[--R].push_back({u, v});
            L >>= 1;
            R >>= 1;
        }
    };

    for (int i = 0; i < q; ++i) {
        int u, v;
        cin >> u >> v;
        queries[i] = {u, v};
        if (u > v) swap(u, v);
        auto key = keyOf(u, v);
        auto it = opened.find(key);
        if (it == opened.end()) {
            opened[key] = i;
        } else {
            addInterval(it->second, i - 1, u, v);
            opened.erase(it);
        }
    }
    for (const auto &[key, start] : opened) {
        int u = (int)(key >> 32);
        int v = (int)(key & 0xffffffffu);
        addInterval(start, q - 1, u, v);
    }

    RollbackParityDSU dsu(n);
    function<void(int)> dfs = [&](int node) {
        size_t snap = dsu.snapshot();
        for (auto [u, v] : seg[node]) dsu.addEdge(u, v);

        if (node >= base) {
            int idx = node - base;
            if (idx < q) cout << (dsu.bad == 0 ? "YES" : "NO") << '\n';
        } else {
            dfs(node * 2);
            dfs(node * 2 + 1);
        }
        dsu.rollback(snap);
    };
    dfs(1);
    return 0;
}
