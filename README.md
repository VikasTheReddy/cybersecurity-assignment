# MA429 Cybersecurity Assignments — C++ Solutions

This repository contains one standalone C++17 solution for each of the 10 problems in the **MA429 Cybersecurity Assignments** PDF.

The file layout follows the supplied demonstration repository:

```text
MA429_Cybersecurity_Solutions/
├── README.md
├── .gitignore
├── problem1.cpp
├── problem2.cpp
├── problem3.cpp
├── problem4.cpp
├── problem5.cpp
├── problem6.cpp
├── problem7.cpp
├── problem8.cpp
├── problem9.cpp
└── problem10.cpp
```

## Solutions overview

| File | Main idea | Time complexity |
|---|---|---|
| `problem1.cpp` | Difference-array boundaries, BFS shortest paths, minimum-weight perfect matching with bitmask DP | `O(B · n · L + 2^B · B^2)`, where `B ≤ 20` boundaries and `L ≤ 100` distinct lengths |
| `problem2.cpp` | Randomized bitmask strategy for the AND/OR interactive protocol | `O(t)` expected time; constant work per test case |
| `problem3.cpp` | Directed de Bruijn graph and Hierholzer's Euler-trail algorithm | `O(n + 62^2)` |
| `problem4.cpp` | Group edges by XOR value and count connected components with a sparse DSU | `O(m log m + m α(n) + U log n)`, where `U ≤ m` is the number of distinct edge-XOR values |
| `problem5.cpp` | Trie of reversed dictionary words and dynamic programming over ciphertext positions | `O(S + n·W)`, where `S` is total dictionary length and `W` is the maximum word length |
| `problem6.cpp` | Bounded-composition DP for strings with a fixed sum of letter indices | `O(100 · 2500 + total input length)` |
| `problem7.cpp` | Tree-path DP with all-roots parent and subtree-size tables | `O(n^2)` time and memory |
| `problem8.cpp` | Offline edge-active intervals, segment tree over time, rollback parity DSU | `O(q log q log n)` |
| `problem9.cpp` | Convert each contact into a `k`-day active interval; segment tree over time and rollback DSU | `O(q log q log n)` |
| `problem10.cpp` | Find a functional-graph cycle length using square-root jumps | `O(√n)` oracle queries |

`B` is the number of required difference-array boundaries, `L` is the number of distinct permitted interval lengths, `S` is the sum of dictionary-word lengths, `W` is the maximum dictionary-word length, and `U` is the number of distinct XOR values among graph edges.

## Compile

Compile any ordinary solution independently:

```bash
g++ -std=c++17 -O2 -pipe problem1.cpp -o problem1
./problem1 < input.txt
```

Replace `problem1.cpp` with the desired file. Each source file has its own `main()` and is intended to be compiled separately.

## Interactive problems

**Problems 2 and 10 are interactive.** They cannot be validated by supplying a normal sample input file because they must exchange messages with an interactor.

- `problem2.cpp` flushes after every protocol message. It uses randomization in the ambiguous `o == x` case, as required by the probabilistic nature of the problem.
- `problem10.cpp` flushes after every query and after the final answer. It checks for `-1` and exits immediately. Its square-root decomposition stays below the 1000-query limit for `n ≤ 200000`.

For local testing, connect either executable to a mock interactor that follows the protocol in the assignment statement. Do not submit a mock interactor as the solution.

## Verification performed

- All 10 files compile with `g++ -std=c++17 -O2`.
- The non-interactive sample cases used during validation were checked.
- Problems 1, 4, 7, 8, and 9 were additionally checked against brute-force or direct simulations on small random instances.
- Problems 2 and 10 were tested with mock interactors, including random hidden values and functional graphs.

The interactive judge remains the authoritative check for Problems 2 and 10.
