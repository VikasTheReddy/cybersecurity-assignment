#include <bits/stdc++.h>
using namespace std;

// Interactive problem. The interactor provides one response for each query.
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;

    constexpr int BITS = 30;
    constexpr int FULL = (1u << BITS) - 1;

    // Exactly 15 one-bits and 15 zero-bits. The balanced split minimizes
    // the worst-case collision probability in the ambiguous case o == x.
    const int x = (1 << 15) - 1;
    const int outside = FULL ^ x;

    random_device rd;
    uint32_t seed = (uint32_t)chrono::steady_clock::now().time_since_epoch().count();
    seed ^= (uint32_t)rd();
    mt19937 rng(seed);

    while (t--) {
        cout << x << endl; // endl flushes the interactive output.

        int o;
        if (!(cin >> o) || o == -1) return 0;

        int d;
        if (o == x) {
            // In this case either operation may have produced the response.
            // Randomly choose a mask that has at least one bit on each side.
            // The interactor fixes v before this random choice, so the
            // probability of an accidental collision is at most 1/(2^15-1).
            do {
                d = (int)(rng() & FULL);
            } while ((d & x) == 0 || (d & outside) == 0);
        } else if ((o & outside) == 0) {
            // o is a strict subset of x, hence the hidden operation is AND.
            d = x & -x;
        } else {
            // o has a one outside x, hence the hidden operation is OR.
            d = outside & -outside;
        }

        // m0 = 0, m1 = d. The final response is v (if b=0)
        // or d XOR v (if b=1).
        cout << 0 << ' ' << d << endl;

        int r;
        if (!(cin >> r) || r == -1) return 0;

        // If b=0, r=v and it must satisfy the observed AND/OR relation.
        // The chosen d makes that test fail for b=1, except for the
        // deliberately randomized low-probability ambiguous case.
        bool consistentWithBZero = ((r & x) == o) || ((r | x) == o);
        cout << (consistentWithBZero ? 0 : 1) << endl;
    }
    return 0;
}
