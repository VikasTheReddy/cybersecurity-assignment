#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    int next[26];
    int wordIndex;
    TrieNode() : wordIndex(-1) { fill(next, next + 26, -1); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string text;
    cin >> n >> text;

    int m;
    cin >> m;
    vector<string> words(m);

    vector<TrieNode> trie;
    trie.reserve(1000001);
    trie.emplace_back();

    int maxLen = 0;
    for (int id = 0; id < m; ++id) {
        cin >> words[id];
        maxLen = max(maxLen, (int)words[id].size());
        int node = 0;
        // A reversed dictionary word matches the encoded text left to right.
        for (int j = (int)words[id].size() - 1; j >= 0; --j) {
            int c = tolower((unsigned char)words[id][j]) - 'a';
            if (trie[node].next[c] == -1) {
                trie[node].next[c] = (int)trie.size();
                trie.emplace_back();
            }
            node = trie[node].next[c];
        }
        if (trie[node].wordIndex == -1) trie[node].wordIndex = id;
    }

    vector<int> previous(n + 1, -1), chosenWord(n + 1, -1);
    vector<char> reachable(n + 1, false);
    reachable[0] = true;

    for (int i = 0; i < n; ++i) {
        if (!reachable[i]) continue;
        int node = 0;
        int limit = min(n, i + maxLen);
        for (int j = i; j < limit; ++j) {
            int c = text[j] - 'a';
            node = trie[node].next[c];
            if (node == -1) break;

            if (trie[node].wordIndex != -1 && !reachable[j + 1]) {
                reachable[j + 1] = true;
                previous[j + 1] = i;
                chosenWord[j + 1] = trie[node].wordIndex;
            }
        }
    }

    // The statement guarantees that at least one decomposition exists.
    vector<int> answer;
    for (int pos = n; pos > 0; pos = previous[pos]) {
        answer.push_back(chosenWord[pos]);
    }
    reverse(answer.begin(), answer.end());

    for (int i = 0; i < (int)answer.size(); ++i) {
        if (i) cout << ' ';
        cout << words[answer[i]];
    }
    cout << '\n';
    return 0;
}
