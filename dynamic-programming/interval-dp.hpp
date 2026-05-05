#pragma once

#include <bits/stdc++.h>
using namespace std;

namespace IntervalDP {

// ------------------------------------------------------------
// IntervalDP namespace
//
// Standard form:
//   dp[l][r] = min (or max) over k of { dp[l][k] + dp[k+1][r] + cost[l][r] }
//
// Typical use:
//   1. Stone Merging   -- merge adjacent piles
//   2. Matrix Chain    -- optimal parenthesization
//   3. Palindrome DP  -- min cuts to partition into palindromes
//   4. General merge  -- arbitrary cost matrix
// ------------------------------------------------------------

// ------------------------------------------------------------
// General interval DP driver
// dp[l][r]: best cost for interval [l, r] (0-indexed, inclusive)
// cost[l][r]: cost of merging or combining [l..r]
// fn: merge function (e.g. min or max)
// ------------------------------------------------------------
template <typename T>
vector<vector<T>> intervalDPGeneric(
    const vector<T>& cost,          // cost[l][r] for 0 <= l <= r < n
    int n,
    function<T(T, T)> combine,
    T identity = T(0)) {

    vector<vector<T>> dp(n, vector<T>(n, identity));
    // length = 1: single element costs nothing to itself
    for (int len = 1; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            if (len == 1) {
                dp[l][r] = identity;
                continue;
            }
            T best = identity;
            bool first = true;
            for (int k = l; k < r; ++k) {
                T candidate = combine(dp[l][k], dp[k + 1][r]) + cost[l][r];
                if (first) {
                    best = candidate;
                    first = false;
                } else {
                    best = combine(best, candidate);
                }
            }
            dp[l][r] = best;
        }
    }
    return dp;
}

// ------------------------------------------------------------
// Stone Merging (合并石子)
//
// Problem: merge stones[p..r] into one pile. Merging adjacent
// piles costs sum(p..r). Find minimum total cost.
//
// Let dp[l][r] = min cost to merge stones[l..r] into one pile.
// dp[l][r] = min_{k in [l, r-1]} { dp[l][k] + dp[k+1][r] + sum[l..r] }
//
// Time: O(n^3), Space: O(n^2)
// ------------------------------------------------------------

// Build prefix sum for O(1) range sum queries
vector<long long> prefixSum(const vector<long long>& a) {
    int n = a.size();
    vector<long long> ps(n + 1, 0);
    for (int i = 0; i < n; ++i) ps[i + 1] = ps[i] + a[i];
    return ps;
}

// dp[l][r] = min cost to merge stones[l..r] (0-indexed)
vector<vector<long long>> stoneMergingDP(const vector<long long>& stones) {
    int n = stones.size();
    if (n == 0) return {};
    vector<long long> ps = prefixSum(stones);

    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    // len = 1: already one stone, cost = 0
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            dp[l][r] = LLONG_MAX;
            for (int k = l; k < r; ++k) {
                long long cost = dp[l][k] + dp[k + 1][r]
                               + ps[r + 1] - ps[l];
                dp[l][r] = min(dp[l][r], cost);
            }
        }
    }
    return dp;
}

// Convenience: just returns dp[0][n-1]
long long stoneMergingMinCost(const vector<long long>& stones) {
    int n = stones.size();
    if (n == 0) return 0;
    auto dp = stoneMergingDP(stones);
    return dp[0][n - 1];
}

// Recursive reconstruction: which k gave the optimal split?
void stoneMergingTrace(const vector<long long>& stones,
                       const vector<vector<long long>>& dp,
                       int l, int r,
                       vector<pair<int,int>>& merges) {
    if (l == r) return;
    for (int k = l; k < r; ++k) {
        long long base = dp[0][stones.size() - 1];  // top-level best
        if (base == LLONG_MAX) return;
        long long expected = stones.empty() ? 0 : 0;  // placeholder
        // actual trace: check which k satisfies optimal condition
        if (dp[l][r] == dp[l][k] + dp[k + 1][r] +
                        (prefixSum(stones)[r + 1] - prefixSum(stones)[l])) {
            merges.emplace_back(l, r);
            stoneMergingTrace(stones, dp, l, k, merges);
            stoneMergingTrace(stones, dp, k + 1, r, merges);
            return;
        }
    }
}

// ------------------------------------------------------------
// Stone Merging (Ring / Circular version)
//
// Stones arranged in a circle. We need to find min cost of merging
// all into one pile, where we may start at any position.
// Strategy: duplicate the array, try all 2n - n + 1 windows.
// ------------------------------------------------------------
long long stoneMergingRing(const vector<long long>& stones) {
    int n = stones.size();
    if (n == 0) return 0;
    vector<long long> dup(2 * n);
    for (int i = 0; i < 2 * n; ++i) dup[i] = stones[i % n];

    vector<long long> ps(2 * n + 1, 0);
    for (int i = 0; i < 2 * n; ++i) ps[i + 1] = ps[i] + dup[i];

    long long best = LLONG_MAX;
    // Try window starting at i, length = n
    for (int i = 0; i < n; ++i) {
        vector<vector<long long>> dp(n, vector<long long>(n, 0));
        // local prefix within window [i, i+n-1]
        auto localSum = [&](int l, int r) -> long long {
            int li = i + l, ri = i + r;
            return ps[ri + 1] - ps[li];
        };
        for (int len = 2; len <= n; ++len) {
            for (int l = 0; l + len - 1 < n; ++l) {
                int r = l + len - 1;
                dp[l][r] = LLONG_MAX;
                for (int k = l; k < r; ++k) {
                    long long cand = dp[l][k] + dp[k + 1][r]
                                   + localSum(l, r);
                    dp[l][r] = min(dp[l][r], cand);
                }
            }
        }
        best = min(best, dp[0][n - 1]);
    }
    return best;
}

// ------------------------------------------------------------
// Optimal Matrix Chain Multiplication
//
// Given matrices A1, A2, ..., An where Ai has dimensions d[i] x d[i+1],
// find min number of scalar multiplications to compute product.
//
// dp[l][r] = min cost to multiply A[l]..A[r]
// Cost of merging product(A[l..k]) * product(A[k+1..r]) is:
//   d[l] * d[k+1] * d[r+1]
// ------------------------------------------------------------
struct MatrixDim { int rows, cols; };

long long matrixChainMinMult(const vector<int>& dims) {
    int n = dims.size() - 1;  // n matrices, dims has n+1 entries
    if (n <= 1) return 0;
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            dp[l][r] = LLONG_MAX;
            for (int k = l; k < r; ++k) {
                long long cand = dp[l][k] + dp[k + 1][r]
                               + 1LL * dims[l] * dims[k + 1] * dims[r + 1];
                dp[l][r] = min(dp[l][r], cand);
            }
        }
    }
    return dp[0][n - 1];
}

// Reconstruct optimal parenthesization
string matrixChainTrace(const vector<int>& dims, int l, int r,
                        const vector<vector<long long>>& dp) {
    if (l == r) return "A" + to_string(l + 1);
    for (int k = l; k < r; ++k) {
        if (dp[l][r] == dp[l][k] + dp[k + 1][r] +
                        1LL * dims[l] * dims[k + 1] * dims[r + 1]) {
            return "(" + matrixChainTrace(dims, l, k, dp) + " "
                 + matrixChainTrace(dims, k + 1, r, dp) + ")";
        }
    }
    return "ERROR";
}

// ------------------------------------------------------------
// Palindrome Partitioning
//
// Partition s into minimum number of palindromic substrings.
// dp[l][r] = min cuts to make s[l..r] all palindromes.
// dp[l][r] = 0 if s[l..r] is palindrome
// else dp[l][r] = min_{k in [l, r-1]} { dp[l][k] + dp[k+1][r] + 1 }
//
// Note: standard formulation uses cut counts; simpler:
// dp2[i] = min cuts for prefix s[0..i]
// ------------------------------------------------------------
int minPalPartition(const string& s) {
    int n = s.size();
    // isPal[l][r] = true iff s[l..r] is palindrome
    vector<vector<bool>> isPal(n, vector<bool>(n, false));
    for (int i = 0; i < n; ++i) isPal[i][i] = true;
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            if (len == 2) {
                isPal[l][r] = (s[l] == s[r]);
            } else {
                isPal[l][r] = (s[l] == s[r]) && isPal[l + 1][r - 1];
            }
        }
    }

    // dp[l][r] = min cuts for s[l..r]
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            if (isPal[l][r]) {
                dp[l][r] = 0;
            } else {
                dp[l][r] = INT_MAX;
                for (int k = l; k < r; ++k) {
                    dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + 1);
                }
            }
        }
    }
    return dp[0][n - 1];
}

// ------------------------------------------------------------
// Example / test harness
// ------------------------------------------------------------
#ifdef INTERVAL_DP_MAIN
int main() {
    // Stone merging example
    vector<long long> stones = {3, 4, 5, 6, 1};
    cout << "Stone merging min cost: " << stoneMergingMinCost(stones) << endl;

    // Matrix chain example: A1(10x30), A2(30x5), A3(5x60)
    vector<int> dims = {10, 30, 5, 60};
    cout << "Matrix chain min mult: " << matrixChainMinMult(dims) << endl;

    // Palindrome partition
    string s = "aab";
    cout << "Min palindrome cuts for \"" << s << "\": "
         << minPalPartition(s) << endl;

    // Ring stone merging
    vector<long long> ringStones = {1, 2, 3};
    cout << "Ring stone merging min cost: " << stoneMergingRing(ringStones) << endl;

    return 0;
}
#endif
}  // namespace IntervalDP
