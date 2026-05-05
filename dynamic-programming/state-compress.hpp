/**
 * State Compression Dynamic Programming (状压 DP)
 *
 * State compression DP applies when the number of states per position is
 * small (typically n <= 20), allowing us to represent a set of positions
 * as a bitmask of length n.
 *
 * Bit operations:
 *   - getBit(n, k)   : (n >> k) & 1
 *   - setBit(n, k)   : n | (1 << k)
 *   - clearBit(n, k) : n & ~(1 << k)
 *   - countOnes(n)  : __builtin_popcount (or loop)
 *
 * Submask iteration:
 *   foreachSubmask(mask, fn): enumerates all submasks of mask
 *     Time: O(3^n) if called for all masks; O(2^{popcount}) per call
 *
 * Subset checking:
 *   isSubset(mask, sub): true iff (mask & sub) == sub
 *
 * TSP DP (Hamiltonian cycle):
 *   dp[mask][i] = minimum cost of a path that visits exactly the vertices
 *                 in `mask` and ends at vertex i.
 *
 *   Transition:
 *     For every j in mask that is not i:
 *       dp[mask][i] = min(dp[mask ^ (1<<i)][j] + dist[j][i])
 *
 *   Answer: min_{i} dp[fullMask][i] + dist[i][0]  (return to start)
 *
 * Complexity: O(n^2 * 2^n) time, O(n * 2^n) space
 *
 * Reference: cp-algorithms.com - Hamiltonian Path in O(n * 2^n)
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

namespace StateCompress {

// ---------------------------------------------------------------------------
// Section 1: Bit Operations
// ---------------------------------------------------------------------------

/**
 * Get the k-th bit (0-indexed) of n.
 * Returns 0 or 1.
 */
inline int getBit(int n, int k) {
    return (n >> k) & 1;
}

/**
 * Set the k-th bit of n to 1 (leave other bits unchanged).
 */
inline int setBit(int n, int k) {
    return n | (1 << k);
}

/**
 * Clear the k-th bit of n to 0 (leave other bits unchanged).
 */
inline int clearBit(int n, int k) {
    return n & ~(1 << k);
}

/**
 * Count the number of 1-bits in n.
 * Uses compiler builtin for speed; falls back to loop on non-GCC.
 */
inline int countOnes(int n) {
#ifdef __GNUC__
    return __builtin_popcount(n);
#else
    int cnt = 0;
    while (n) {
        n &= n - 1;  // clear lowest set bit
        ++cnt;
    }
    return cnt;
#endif
}

/**
 * Count 1-bits in a 64-bit integer.
 */
inline int countOnesLL(long long n) {
#ifdef __GNUC__
    return __builtin_popcountll(n);
#else
    int cnt = 0;
    while (n) {
        n &= n - 1;
        ++cnt;
    }
    return cnt;
#endif
}

/**
 * Check if bit k is set in n.
 */
inline bool hasBit(int n, int k) {
    return (n >> k) & 1;
}

// ---------------------------------------------------------------------------
// Section 2: Submask Iteration
// ---------------------------------------------------------------------------

/**
 * Iterate over all submasks of `mask` (including 0 and `mask` itself).
 *
 * A submask is a mask `sub` where (mask & sub) == sub.
 * Every mask has exactly 2^{popcount(mask)} submasks.
 *
 * @param mask  the mask to enumerate submasks of
 * @param fn   callable (int submask) -> void
 *
 * Example:
 *   foreachSubmask(6 (110), fn): fn(0), fn(2), fn(4), fn(6)
 *   foreachSubmask(3 (011), fn): fn(0), fn(1), fn(2), fn(3)
 */
template <typename F>
void foreachSubmask(int mask, F&& fn) {
    // Standard enumeration: iterate submask = mask; submask; submask = (submask-1) & mask
    // This visits each submask exactly once (in decreasing order).
    for (int sub = mask; ; sub = (sub - 1) & mask) {
        fn(sub);
        if (sub == 0) break;
    }
}

/**
 * Iterate over all non-empty submasks.
 */
template <typename F>
void foreachNonEmptySubmask(int mask, F&& fn) {
    for (int sub = mask; sub; sub = (sub - 1) & mask) {
        fn(sub);
    }
}

/**
 * Iterate over all proper submasks (strict subsets).
 * Equivalent to: foreachSubmask(mask, fn) but skips `mask` itself.
 */
template <typename F>
void foreachProperSubmask(int mask, F&& fn) {
    for (int sub = mask; sub; sub = (sub - 1) & mask) {
        fn(sub);
    }
}

/**
 * Iterate over all submasks with exactly k bits set.
 * Useful for DP by number of elements.
 */
template <typename F>
void foreachKSubmask(int mask, int k, F&& fn) {
    foreachSubmask(mask, [&](int sub) {
        if (countOnes(sub) == k) fn(sub);
    });
}

// ---------------------------------------------------------------------------
// Section 3: Subset Operations
// ---------------------------------------------------------------------------

/**
 * Check if `sub` is a subset of `mask`.
 * Returns true iff all bits set in sub are also set in mask.
 */
inline bool isSubset(int mask, int sub) {
    return (mask & sub) == sub;
}

/**
 * Check if two masks are disjoint (no common bits).
 */
inline bool isDisjoint(int mask1, int mask2) {
    return (mask1 & mask2) == 0;
}

/**
 * Get the complement of mask within a universe of `n` bits.
 */
inline int complement(int mask, int n) {
    return ((1 << n) - 1) ^ mask;
}

/**
 * Get the number of submasks of mask.
 * This is always 2^{popcount(mask)}.
 */
inline int submaskCount(int mask) {
    return 1 << countOnes(mask);
}

// ---------------------------------------------------------------------------
// Section 4: TSP (Hamiltonian Cycle) DP
// ---------------------------------------------------------------------------

/**
 * Solve the Traveling Salesman Problem (TSP) on a complete graph
 * with n vertices (0..n-1) using state compression DP.
 *
 * dp[mask][i] = minimum cost of a path that visits exactly the vertices
 *               in `mask` and ends at vertex i.
 *
 * Initialization:
 *   dp[1 << 0][0] = 0  (start at vertex 0 with only vertex 0 visited)
 *   dp[1 << i][i] = dist[0][i]  (direct path from 0 to i)
 *
 * Transition:
 *   For each state (mask, i):
 *     For each j in mask that is not i:
 *       dp[mask][i] = min(dp[mask][i], dp[mask ^ (1<<i)][j] + dist[j][i])
 *
 * Answer:
 *   min_{i} dp[fullMask][i] + dist[i][0]   (return to start)
 *   For path (not cycle): min_{i} dp[fullMask][i]
 *
 * @param n    number of vertices (n <= 20)
 * @param dist n x n distance matrix (dist[i][j] >= 0, symmetric for undirected)
 * @return     minimum Hamiltonian cycle cost
 *
 * Complexity: O(n^2 * 2^n) time, O(n * 2^n) space
 */
ll solveTSP(int n, const vector<vector<ll>>& dist) {
    if (n == 0) return 0;
    if (n == 1) return 0;  // trivial: start == end, no travel needed

    int fullMask = (1 << n) - 1;
    int size = 1 << n;

    // dp[mask][i] = min cost visiting `mask`, ending at i
    // Use a flat array to avoid large stack allocation.
    vector<vector<ll>> dp(size, vector<ll>(n, LLONG_MAX / 4));

    // Base case: start from vertex 0
    dp[1 << 0][0] = 0;

    // For masks with exactly 1 bit and that bit != 0: direct path from 0
    for (int i = 1; i < n; ++i) {
        dp[1 << i][i] = dist[0][i];
    }

    // Iterate over all masks
    for (int mask = 1; mask <= fullMask; ++mask) {
        // Skip masks with less than 2 bits
        if (countOnes(mask) < 2) continue;

        for (int i = 0; i < n; ++i) {
            // i must be in the mask and be the ending vertex
            if (!((mask >> i) & 1)) continue;
            if (dp[mask][i] == LLONG_MAX / 4) continue;

            int prevMask = mask ^ (1 << i);  // mask without i

            // Try extending the path by adding j before i (j is the new end)
            for (int j = 0; j < n; ++j) {
                if ((prevMask >> j) & 1) {
                    ll candidate = dp[prevMask][j] + dist[j][i];
                    if (candidate < dp[mask][i]) {
                        dp[mask][i] = candidate;
                    }
                }
            }
        }
    }

    // Complete the cycle by returning to vertex 0
    ll ans = LLONG_MAX;
    for (int i = 1; i < n; ++i) {
        if (dp[fullMask][i] != LLONG_MAX / 4 && dist[i][0] != LLONG_MAX / 4) {
            ans = min(ans, dp[fullMask][i] + dist[i][0]);
        }
    }

    return ans;
}

/**
 * TSP with path (open walk, no need to return to start).
 * Returns min_{i} dp[fullMask][i].
 */
ll solveTSPPath(int n, const vector<vector<ll>>& dist) {
    if (n == 0) return 0;
    if (n == 1) return 0;

    int fullMask = (1 << n) - 1;
    vector<vector<ll>> dp(1 << n, vector<ll>(n, LLONG_MAX / 4));

    dp[1 << 0][0] = 0;
    for (int i = 1; i < n; ++i) {
        dp[1 << i][i] = dist[0][i];
    }

    for (int mask = 1; mask <= fullMask; ++mask) {
        if (countOnes(mask) < 2) continue;
        for (int i = 0; i < n; ++i) {
            if (!((mask >> i) & 1)) continue;
            if (dp[mask][i] == LLONG_MAX / 4) continue;
            int prevMask = mask ^ (1 << i);
            for (int j = 0; j < n; ++j) {
                if ((prevMask >> j) & 1) {
                    dp[mask][i] = min(dp[mask][i], dp[prevMask][j] + dist[j][i]);
                }
            }
        }
    }

    ll ans = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        ans = min(ans, dp[fullMask][i]);
    }
    return ans;
}

// ---------------------------------------------------------------------------
// Section 5: Standard DP Transfer Pattern
// ---------------------------------------------------------------------------

/**
 * Standard DP transition for state compression.
 *
 * Given dp[mask][i] (current state) and a new position j not in mask:
 *   nxtMask = mask | (1 << j)
 *   dp[nxtMask][j] = min(dp[nxtMask][j], dp[mask][i] + dist[i][j])
 *
 * This is the core update pattern used in TSP, Hamiltonian path,
 * and many other combinatorial optimizations.
 *
 * @param dp        dp array of size [2^n][n]
 * @param n         number of vertices
 * @param mask      current visited set
 * @param i         current ending vertex
 * @param j         vertex to add
 * @param dist      distance matrix
 * @param newMask   (output) mask | (1<<j)
 * @return          updated dp value, or LLONG_MAX if no improvement
 */
ll transfer(int** dp, int n, int mask, int i, int j,
            const vector<vector<ll>>& dist, int* newMask) {
    *newMask = mask | (1 << j);
    ll candidate = dp[mask][i] + dist[i][j];
    if (candidate < dp[*newMask][j]) {
        dp[*newMask][j] = candidate;
        return candidate;
    }
    return dp[*newMask][j];
}

// ---------------------------------------------------------------------------
// Section 6: Utility Functions
// ---------------------------------------------------------------------------

/**
 * Generate all masks with exactly k bits set, for 0 <= k <= n.
 * Returns a vector of masks.
 */
vector<int> masksOfSize(int n, int k) {
    vector<int> result;
    int full = (1 << n) - 1;
    for (int mask = 0; mask <= full; ++mask) {
        if (countOnes(mask) == k) result.push_back(mask);
    }
    return result;
}

/**
 * Get the next mask with the same number of bits (lexicographic successor).
 */
int nextSamePopcount(int mask) {
    int c = mask & -mask;
    int r = mask + c;
    return (((r ^ mask) >> 2) / c) | r;
}

}  // namespace StateCompress

// -------------------------------------------------------------------------
// Example: TSP on a small graph
// -------------------------------------------------------------------------
#ifdef SC_MAIN

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace StateCompress;

    // Example: 4 cities with given distances
    //   0 -- 2 -- 3
    //   |         |
    //   1 --------+
    // Complete graph distances:
    vector<vector<ll>> dist = {
        {0, 1, 2, 3},
        {1, 0, 4, 2},
        {2, 4, 0, 1},
        {3, 2, 1, 0}
    };

    int n = 4;
    cout << "Bit operations:\n";
    cout << "  getBit(6, 1)  = " << getBit(6, 1) << "  (expected 1)\n";
    cout << "  setBit(4, 2)  = " << setBit(4, 2) << "  (expected 6)\n";
    cout << "  clearBit(7,2)= " << clearBit(7, 2) << "  (expected 3)\n";
    cout << "  countOnes(14) = " << countOnes(14) << "  (expected 3)\n";
    cout << "  isSubset(7,3) = " << isSubset(7, 3) << "  (expected 1)\n";

    cout << "\nSubmask iteration for mask=6 (110):\n";
    foreachSubmask(6, [](int sub) {
        cout << "  " << sub << " (";
        for (int i = 0; i < 3; ++i) cout << getBit(sub, i);
        cout << ")\n";
    });

    cout << "\nTSP (Hamiltonian cycle) on 4 cities:\n";
    ll ans = solveTSP(n, dist);
    cout << "  Minimum cycle cost: " << ans << "\n";
    // Optimal: 0 -> 1 -> 3 -> 2 -> 0 = 1 + 2 + 1 + 2 = 6

    cout << "\nTSP path (open walk):\n";
    ll pathAns = solveTSPPath(n, dist);
    cout << "  Minimum path cost: " << pathAns << "\n";

    return 0;
}

#endif