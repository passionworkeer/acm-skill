#pragma once

/**
 * Stirling Numbers
 *
 * Precomputes Stirling numbers of the first and second kind
 * using O(n^2) dynamic programming.
 *
 * First kind (unsigned): s(n,k) = s(n-1,k-1) + (n-1) * s(n-1,k)
 *   Interpretation: number of permutations of n elements with exactly k cycles
 *
 * Second kind: S(n,k) = k * S(n-1,k) + S(n-1,k-1)
 *   Interpretation: number of ways to partition n elements into k non-empty subsets
 *
 * Reference: kirainmoe's stirling.tex
 */

#include <bits/stdc++.h>
using namespace std;

namespace Stirling {

static const int MAXN = 5000;

long long s1[MAXN + 1][MAXN + 1];  // first kind: unsigned Stirling numbers
long long s2[MAXN + 1][MAXN + 1];  // second kind
int initialized_n = 0;

/**
 * Precompute Stirling numbers up to n (inclusive).
 * Time: O(n^2), Memory: O(n^2)
 */
void init(int n) {
    n = min(n, MAXN);
    if (initialized_n >= n) return;

    // s(n,0) = 0 for n > 0; s(0,0) = 1
    // s(n,k) = 0 when k > n or k < 0
    for (int i = initialized_n + 1; i <= n; ++i) {
        s1[i][0] = 0;
        s1[i][i] = 1;  // s(i,i) = 1 for all i
        for (int k = 1; k < i; ++k) {
            s1[i][k] = (s1[i - 1][k - 1] + (1LL * (i - 1) * s1[i - 1][k]) % 1000000007) % 1000000007;
        }
    }

    // S(n,0) = 0 for n > 0; S(0,0) = 1
    // S(n,k) = 0 when k > n or k < 0
    s2[0][0] = 1;
    for (int i = initialized_n + 1; i <= n; ++i) {
        s2[i][0] = 0;
        s2[i][1] = 1;  // S(n,1) = 1
        for (int k = 2; k <= i; ++k) {
            s2[i][k] = (s2[i - 1][k - 1] + (1LL * k * s2[i - 1][k]) % 1000000007) % 1000000007;
        }
    }

    initialized_n = n;
}

/**
 * Returns the unsigned Stirling number of the first kind s(n, k).
 * s(n,k) counts permutations of {1..n} with exactly k cycles.
 */
inline long long first_kind(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (initialized_n < n) init(n);
    return s1[n][k];
}

/**
 * Returns the Stirling number of the second kind S(n, k).
 * S(n,k) counts ways to partition n elements into k non-empty subsets.
 */
inline long long second_kind(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (initialized_n < n) init(n);
    return s2[n][k];
}

/**
 * Bell number B(n): total number of partitions of n elements.
 * B(n) = sum_{k=0}^{n} S(n,k)
 */
inline long long bell(int n) {
    if (initialized_n < n) init(n);
    long long res = 0;
    for (int k = 1; k <= n; ++k) res = (res + s2[n][k]) % 1000000007;
    return res;
}

} // namespace Stirling
