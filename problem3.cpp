#include <bits/stdc++.h>
using namespace std;

int codeOf(char c) {
    if ('a' <= c && c <= 'z') return c - 'a';
    if ('A' <= c && c <= 'Z') return 26 + c - 'A';
    return 52 + c - '0';
}

char charOf(int x) {
    if (x < 26) return char('a' + x);
    if (x < 52) return char('A' + x - 26);
    return char('0' + x - 52);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    constexpr int ALPHABET = 62;
    constexpr int V = ALPHABET * ALPHABET;
    vector<vector<int>> adj(V);
    vector<int> indeg(V, 0), outdeg(V, 0);

    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        int a = codeOf(s[0]);
        int b = codeOf(s[1]);
        int c = codeOf(s[2]);
        int u = a * ALPHABET + b;
        int v = b * ALPHABET + c;
        adj[u].push_back(v);
        ++outdeg[u];
        ++indeg[v];
    }

    int start = -1;
    int plusOne = 0, minusOne = 0;
    for (int v = 0; v < V; ++v) {
        int diff = outdeg[v] - indeg[v];
        if (diff == 1) {
            ++plusOne;
            start = v;
        } else if (diff == -1) {
            ++minusOne;
        } else if (diff != 0) {
            cout << "NO\n";
            return 0;
        }
    }

    if (!((plusOne == 1 && minusOne == 1) || (plusOne == 0 && minusOne == 0))) {
        cout << "NO\n";
        return 0;
    }
    if (start == -1) {
        for (int v = 0; v < V; ++v) {
            if (outdeg[v]) {
                start = v;
                break;
            }
        }
    }

    if (start == -1) { // n >= 1, included for completeness.
        cout << "NO\n";
        return 0;
    }

    // Hierholzer's algorithm for a directed Euler trail. Every input
    // trigram is one directed edge from its first two chars to last two.
    vector<size_t> ptr(V, 0);
    vector<int> st, trail;
    st.reserve(n + 1);
    trail.reserve(n + 1);
    st.push_back(start);

    while (!st.empty()) {
        int u = st.back();
        if (ptr[u] < adj[u].size()) {
            int v = adj[u][ptr[u]++];
            st.push_back(v);
        } else {
            trail.push_back(u);
            st.pop_back();
        }
    }

    if ((int)trail.size() != n + 1) {
        cout << "NO\n";
        return 0;
    }
    reverse(trail.begin(), trail.end());

    string answer;
    answer.reserve(n + 2);
    answer.push_back(charOf(trail[0] / ALPHABET));
    answer.push_back(charOf(trail[0] % ALPHABET));
    for (int i = 1; i < (int)trail.size(); ++i) {
        answer.push_back(charOf(trail[i] % ALPHABET));
    }

    cout << "YES\n" << answer << '\n';
    return 0;
}
