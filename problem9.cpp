#include <bits/stdc++.h>
using namespace std;

struct Query { int type, a, b; };
struct Change { int child, parent, oldSize; };

class RollbackDSU {
    vector<int> parent, sz;
    vector<Change> history;
public:
    explicit RollbackDSU(int n) : parent(n + 1), sz(n + 1, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) const {
        while (parent[x] != x) x = parent[x];
        return x;
    }

    void addEdge(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) {
            history.push_back({-1, -1, 0});
            return;
        }
        if (sz[u] > sz[v]) swap(u, v);
        history.push_back({u, v, sz[v]});
        parent[u] = v;
        sz[v] += sz[u];
    }

    int componentSize(int x) const { return sz[find(x)]; }
    size_t snapshot() const { return history.size(); }

    void rollback(size_t snap) {
        while (history.size() > snap) {
            Change c = history.back();
            history.pop_back();
            if (c.child != -1) {
                parent[c.child] = c.child;
                sz[c.parent] = c.oldSize;
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q, k;
    cin >> n >> q >> k;

    vector<Query> queries(q);
    vector<int> dayOfAdd(q, -1);
    vector<int> transitionToDay(q + 3, -1);

    int currentDay = 1;
    for (int i = 0; i < q; ++i) {
        int type;
        cin >> type;
        queries[i].type = type;
        if (type == 1) {
            int x, y;
            cin >> x >> y;
            queries[i].a = x;
            queries[i].b = y;
            dayOfAdd[i] = currentDay;
        } else if (type == 2) {
            int z;
            cin >> z;
            queries[i].a = z;
            queries[i].b = 0;
        } else {
            queries[i].a = queries[i].b = 0;
            ++currentDay;
            transitionToDay[currentDay] = i;
        }
    }

    int base = 1;
    while (base < q) base <<= 1;
    vector<vector<pair<int,int>>> seg(2 * base);

    auto addInterval = [&](int l, int r, int u, int v) {
        if (l > r) return;
        int L = l + base, R = r + 1 + base;
        while (L < R) {
            if (L & 1) seg[L++].push_back({u, v});
            if (R & 1) seg[--R].push_back({u, v});
            L >>= 1;
            R >>= 1;
        }
    };

    // An edge added on day d is active on days d ... d+k-1.
    // It disappears at the query that advances the clock to day d+k.
    for (int i = 0; i < q; ++i) {
        if (queries[i].type != 1) continue;
        int expireDay = dayOfAdd[i] + k;
        int expireAt = (expireDay < (int)transitionToDay.size())
                           ? transitionToDay[expireDay] : -1;
        int right = (expireAt == -1 ? q - 1 : expireAt - 1);
        addInterval(i, right, queries[i].a, queries[i].b);
    }

    RollbackDSU dsu(n);
    function<void(int)> dfs = [&](int node) {
        size_t snap = dsu.snapshot();
        for (auto [u, v] : seg[node]) dsu.addEdge(u, v);

        if (node >= base) {
            int idx = node - base;
            if (idx < q && queries[idx].type == 2) {
                cout << dsu.componentSize(queries[idx].a) << '\n';
            }
        } else {
            dfs(node * 2);
            dfs(node * 2 + 1);
        }
        dsu.rollback(snap);
    };
    dfs(1);
    return 0;
}
