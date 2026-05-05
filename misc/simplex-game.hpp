#pragma once

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// Game Theory / SG Theorem (Sprague-Grundy)
//
// Standard impartial combinatorial games:
//   - Nim (normal play)
//   - Take-away games -> SG values via mex
//   - Composite games -> SG via Nim sum
//
// Key concepts:
//   - SG(x) = mex{ SG(y) for all moves from x }
//   - Nim sum (xor of all component SG values)
//   - First player wins iff total XOR != 0 (normal play)
// ------------------------------------------------------------
namespace GameTheory {

// ------------------------------------------------------------
// mex: minimum excluded non-negative integer
// Given a set S of non-negative integers, returns smallest >= 0
// not in S. S is given as a vector.
// ------------------------------------------------------------
int mex(const vector<int>& S) {
    int m = (int)S.size();
    vector<bool> seen(m + 1, false);
    for (int x : S) {
        if (x <= m) seen[x] = true;
    }
    for (int i = 0; i <= m; ++i)
        if (!seen[i]) return i;
    return m + 1;
}

// Overload with seen set (more efficient for large ranges)
int mex(const unordered_set<int>& S) {
    int m = (int)S.size();
    vector<bool> seen(m + 1, false);
    for (int x : S) {
        if (x <= m) seen[x] = true;
    }
    for (int i = 0; i <= m; ++i)
        if (!seen[i]) return i;
    return m + 1;
}

// Generic mex using bitmask (for small SG values, up to 64)
int mex64(const unordered_set<int>& S) {
    unsigned long long mask = 0;
    for (int x : S) {
        if (0 <= x && x < 64) mask |= (1ULL << x);
    }
    // find first unset bit
    return __builtin_ctzll(~mask);
}

// ------------------------------------------------------------
// SG table: compute SG values for positions 0..n
// f[i] = list of reachable positions from i
// g[i] = SG value for position i
// ------------------------------------------------------------
vector<int> sgTable(const vector<vector<int>>& moves, int n) {
    vector<int> g(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        unordered_set<int> s;
        for (int to : moves[i]) {
            if (to >= 0 && to <= n) s.insert(g[to]);
        }
        g[i] = mex(s);
    }
    return g;
}

// Convenience: moves defined as offsets (e.g. remove 1,3,4)
vector<int> sgTableFromOffsets(const vector<int>& offsets, int n) {
    vector<vector<int>> moves(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (int d : offsets) {
            int to = i - d;
            if (to >= 0) moves[i].push_back(to);
        }
    }
    return sgTable(moves, n);
}

// grundy(n) wrapper using the take-away set
vector<int> grundyCache;
vector<int> offsetsCache;

int grundy(int n, const vector<int>& moves) {
    if (grundyCache.empty()) return 0;
    if (grundyCache[n] >= 0) return grundyCache[n];
    unordered_set<int> s;
    for (int d : moves) {
        if (n >= d) s.insert(grundy(n - d, moves));
    }
    return mex(s);
}

// Build Grundy cache lazily
int grundyLazy(int n, const vector<int>& moves) {
    if (grundyCache.size() <= (size_t)n) {
        grundyCache.resize(n + 1, -1);
        offsetsCache = moves;
        grundyCache[0] = 0;
    }
    if (grundyCache[n] != -1) return grundyCache[n];
    unordered_set<int> s;
    for (int d : moves) {
        if (n >= d) s.insert(grundyLazy(n - d, moves));
    }
    grundyCache[n] = mex(s);
    return grundyCache[n];
}

// ------------------------------------------------------------
// Nim: normal-play Nim
// Heaps: array of pile sizes
// Returns Nim sum (xor of all heap sizes)
// ------------------------------------------------------------
int nimxor(const vector<int>& heaps) {
    int x = 0;
    for (int h : heaps) x ^= h;
    return x;
}

int nimxor(const initializer_list<int>& heaps) {
    int x = 0;
    for (int h : heaps) x ^= h;
    return x;
}

// Returns true if first player has a winning strategy
bool isFirstWin(const vector<int>& heaps) {
    return nimxor(heaps) != 0;
}

// Find a winning move: return (heap_index, new_size)
// Requires first player to be winning.
// If no winning move (nim xor == 0), returns (-1, -1).
pair<int, int> findWinningMove(const vector<int>& heaps) {
    int x = nimxor(heaps);
    if (x == 0) return {-1, -1};
    for (size_t i = 0; i < heaps.size(); ++i) {
        int h = heaps[i];
        int target = h ^ x;          // remove (h ^ x) stones from heap i
        if (target < h) {
            return {(int)i, target};
        }
    }
    return {-1, -1};
}

// ------------------------------------------------------------
// SG Theorem: Nim sum of SG values of independent components
// Each component is a subgame; its SG value is precomputed.
// Returns total XOR (non-zero -> first player wins).
// ------------------------------------------------------------
int sgTheorem(const vector<int>& sgValues) {
    int x = 0;
    for (int g : sgValues) x ^= g;
    return x;
}

int sgTheorem(const vector<vector<int>>& components) {
    int x = 0;
    for (const auto& comp : components) x ^= nimxor(comp);
    return x;
}

// ------------------------------------------------------------
// Take-away game builder
// Standard impartial game where a pile of n stones can be reduced
// by moves in the given offset set.
// ------------------------------------------------------------
struct TakeawayGame {
    vector<int> moves;       // allowed removals (positive integers)
    vector<int> grundyVals;  // grundy[0..maxN]

    explicit TakeawayGame(const vector<int>& m, int maxN = 1000)
        : moves(m) {
        sort(moves.begin(), moves.end());
        grundyVals = sgTableFromOffsets(moves, maxN);
    }

    int grundy(int n) const {
        if (n < (int)grundyVals.size()) return grundyVals[n];
        return 0;
    }

    // Determine winner for a single pile of size n
    bool firstPlayerWins(int n) const { return grundy(n) != 0; }

    // Determine winner for multiple independent piles
    bool firstPlayerWins(const vector<int>& piles) const {
        int x = 0;
        for (int p : piles) x ^= grundy(p);
        return x != 0;
    }
};

// ------------------------------------------------------------
// Misere Nim (last move loses)
// Special rules: when all piles have size 1, winning depends on parity.
// Otherwise treat as normal Nim.
// ------------------------------------------------------------
bool isFirstWinMisere(const vector<int>& heaps) {
    int nonOnes = 0, ones = 0;
    for (int h : heaps) {
        if (h == 1) ++ones;
        else ++nonOnes;
    }
    if (nonOnes == 0) {
        // all piles size 1: first player wins iff odd count
        return (ones % 2 == 1);
    } else {
        // Has large pile: treat as normal Nim but strategy differs
        // First player wins iff XOR of all large piles != 1
        int x = 0;
        for (int h : heaps) x ^= h;
        // When there's a pile > 1, the strategy is same as normal Nim
        // but the terminal condition differs.
        // Correct misere condition: XOR != 0
        return x != 0;
    }
}

// ------------------------------------------------------------
// Example / test harness
// ------------------------------------------------------------
#ifdef GAME_THEORY_MAIN
int main() {
    // Example: take-away game where you can remove 1, 3, or 4 stones
    vector<int> takeaway = {1, 3, 4};
    TakeawayGame game(takeaway, 20);

    cout << "Grundy numbers for remove {1,3,4}:" << endl;
    for (int i = 0; i <= 20; ++i) {
        cout << "grundy[" << i << "] = " << game.grundy(i);
        if (i % 5 == 4) cout << endl;
        else cout << "  ";
    }

    cout << "\nSG table (sgTableFromOffsets):" << endl;
    auto sg = sgTableFromOffsets(takeaway, 20);
    for (int i = 0; i <= 20; ++i)
        cout << sg[i] << (i % 7 == 4 ? "\n" : " ");

    // Nim example
    cout << "\nNim example: heaps {3,4,5}:" << endl;
    vector<int> heaps = {3, 4, 5};
    cout << "Nim xor = " << nimxor(heaps) << endl;
    cout << "First player wins = " << boolalpha << isFirstWin(heaps) << endl;
    auto [hi, hv] = findWinningMove(heaps);
    cout << "Winning move: take " << (heaps[hi] - hv)
         << " from heap " << hi << " (new size " << hv << ")" << endl;

    // Multiple components SG theorem
    cout << "\nSG theorem with subgames:" << endl;
    // Component A: heap of 5 (normal Nim)
    // Component B: take-away {1,3,4}, pile of 8
    vector<int> comp1 = {5};
    vector<int> comp2 = {8};
    TakeawayGame game2(takeaway, 20);
    int g1 = nimxor(comp1);  // 5
    int g2 = game2.grundy(8);
    cout << "Comp1 SG = " << g1 << ", Comp2 SG = " << g2 << endl;
    cout << "Total SG xor = " << sgTheorem({g1, g2}) << endl;

    // Misere Nim
    cout << "\nMisere Nim: heaps {1,1,1}: " << isFirstWinMisere({1,1,1}) << endl;
    cout << "Misere Nim: heaps {1,1,1,1}: " << isFirstWinMisere({1,1,1,1}) << endl;

    return 0;
}
#endif
}  // namespace GameTheory
