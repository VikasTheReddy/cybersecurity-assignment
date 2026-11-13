#include <bits/stdc++.h>
using namespace std;

// Interactive oracle: ask(c, r) returns f applied c times to r.
int ask(int c, int r) {
    cout << "? " << c << ' ' << r << endl;
    int response;
    if (!(cin >> response) || response == -1) exit(0);
    return response;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    int block = (int)ceil(sqrt((double)n));

    // n applications of f from any start must land on a directed cycle.
    int y = ask(n, 1);

    // First inspect up to 'block' consecutive points on the cycle.
    unordered_map<int, int> firstPosition;
    firstPosition.reserve(2 * block + 5);
    firstPosition[y] = 0;

    int cycleLength = -1;
    int current = y;
    for (int i = 1; i < block; ++i) {
        current = ask(1, current);
        if (current == y) {
            cycleLength = i;
            break;
        }
        firstPosition[current] = i;
    }

    // If the cycle is longer than block, jump in blocks until a jump
    // lands on one of the first block vertices. The difference is one
    // complete cycle length.
    if (cycleLength == -1) {
        int z = y;
        for (int jumps = 1; ; ++jumps) {
            z = ask(block, z);
            auto it = firstPosition.find(z);
            if (it != firstPosition.end()) {
                cycleLength = block * jumps - it->second;
                break;
            }
        }
    }

    // Choose c=y and r=f^shift(y), where shift+c is divisible by the
    // cycle length. Therefore f^c(r)=y=c.
    int shift = (cycleLength - (y % cycleLength)) % cycleLength;
    int r = (shift == 0 ? y : ask(shift, y));

    cout << "! " << y << ' ' << r << endl;
    return 0;
}
