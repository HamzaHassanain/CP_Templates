# Missing templates

Gap list from an audit of the 74 existing templates, ordered roughly by how
often each shows up in an ICPC regional. Write new files in compact print style
(K&R braces, short header comment with complexity + usage) to keep the notebook
page count down.

## Contest infrastructure — nothing here yet

- [ ] `Misc/template.cpp` — fast I/O, multi-test skeleton, debug macros
- [ ] `Misc/CustomHash.cpp` — splitmix64 hash for `unordered_map`/`gp_hash_table`
      (plain `unordered_map` is routinely hacked)
- [ ] `Misc/Random.cpp` — `mt19937_64` seeded from `chrono`
- [ ] `Misc/OrderedSet.cpp` — `__gnu_pbds` order-statistics tree
- [ ] `Misc/FastInput.cpp` — getchar-based reader
- [ ] `Misc/Int128.cpp` — `__int128` read/print helpers
- [ ] `Misc/TernarySearch.cpp` — integer and real variants
- [ ] `Misc/CoordinateCompression.cpp`

## Graph — biggest hole

- [ ] `Graph/MST-Kruskal.cpp` — five DSU variants and no Kruskal
- [ ] `Graph/MST-Prim.cpp`
- [ ] `Graph/BridgesArticulationPoints.cpp` — Tarjan; only SCC exists today
- [ ] `Graph/BridgeTree.cpp` / `Graph/BiconnectedComponents.cpp`
- [ ] `Graph/TwoSAT.cpp` — ~30 lines on top of the existing SCC
- [ ] `Graph/BellmanFord.cpp` — negative cycle detection
- [ ] `Graph/FloydWarshall.cpp`
- [ ] `Graph/BFS01.cpp`
- [ ] `Graph/TopologicalSort.cpp` — plus DAG longest path
- [ ] `Graph/EulerPath.cpp` — Hierholzer, directed + undirected
- [ ] `Graph/Hungarian.cpp` — assignment problem
- [ ] `Graph/MinCutRecovery.cpp` — recover the cut, and König vertex cover from
      the existing bipartite matching
- [ ] `Graph/SecondBestMST.cpp`
- [ ] `Graph/DirectedMST.cpp` — Chu-Liu/Edmonds (rarer)

## Math

- [ ] `Math/FFT.cpp` and `Math/NTT.cpp` — no polynomial multiplication anywhere
- [ ] `Math/CRT.cpp` — coprime and general moduli
- [ ] `Math/BSGS.cpp` — discrete log
- [ ] `Math/TonelliShanks.cpp` — modular square root
- [ ] `Math/Lucas.cpp` — nCr mod small prime
- [ ] `Math/GaussianElimination.cpp` — double / mod p / xor variants
      (only `XorBasis.cpp` exists)
- [ ] `Math/PrimitiveRoot.cpp`
- [ ] Fix `Math/MODs.cpp`: no `__int128` mulmod, overflows for `MOD > 2^31`

## Geometry — only two files, both `complex<double>` macro-based

The `#define point complex<double>` style can't hold integer coordinates and is
fragile under `-O2`. Consider a proper `struct pt` with an integer template
parameter, then:

- [ ] point in polygon (+ on-boundary)
- [ ] polygon area, centroid, convexity test
- [ ] closest pair of points
- [ ] rotating calipers (diameter, width)
- [ ] half-plane intersection
- [ ] minimum enclosing circle (Welzl)
- [ ] circle-line and circle-circle intersection, tangents

## Strings

- [ ] `Strings/AhoCorasick.cpp` — missing entirely (Trie + KMP automaton exist,
      but not multi-pattern matching)
- [ ] `Strings/Eertree.cpp` — palindromic tree
- [ ] `Strings/Duval.cpp` — Lyndon factorization + minimal rotation

## DP

- [ ] `DP/DivideAndConquerOpt.cpp`
- [ ] `DP/KnuthOpt.cpp`
- [ ] `DP/AliensTrick.cpp`
- [ ] `DP/DigitDP.cpp`
- [ ] `DP/BitmaskTSP.cpp`
- [ ] `DP/LIS.cpp` — O(n log n), with reconstruction
- [ ] `DP/Knapsack.cpp` — 0/1, bounded, unbounded

## Data structures

- [ ] `Range-Queries/Treap.cpp` — implicit treap, split/merge, lazy reversal
- [ ] `Range-Queries/MoWithUpdates.cpp` — 3D Mo
- [ ] `Range-Queries/SegmentTreeBeats.cpp`
- [ ] `Range-Queries/FenwickKth.cpp` — binary lifting on the BIT

## Cheat sheets — put these in `Notes/*.tex`

`MakePDFS.py` `\input`s every `.tex` in `Notes/` at the front of the notebook.

- [ ] Game theory — Nim, Sprague-Grundy, misère, staircase Nim, Wythoff
- [ ] Combinatorics identities — Catalan, Stirling 1st/2nd, Bell, derangements,
      stars and bars, Burnside/Pólya, inclusion-exclusion
- [ ] Number theory — divisor/totient sums, Möbius inversion forms, max divisor
      counts up to 1e18, primes near powers of 10
- [ ] Series and closed forms — power sums, geometric, harmonic bounds, Pick's
      theorem
- [ ] Geometry formulas — triangle centers, tangents, spherical distance
- [ ] Bit tricks — `__builtin_*`, subset enumeration, Gray code
- [ ] Complexity budget — n vs. feasible algorithm at 1e8 ops/s, overflow limits
- [ ] C++/STL gotchas — `set` vs `priority_queue` erase, iterator invalidation,
      printing doubles, recursion depth
- [ ] Debug checklist — long long, reset globals between tests, re-read limits

## Misc

- [ ] `Range-Queries/SegemntTree.cpp` is misspelled (Segemnt → Segment)
