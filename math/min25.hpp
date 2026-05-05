#pragma once

/**
 * Min25 Sieve
 *
 * Computes prefix sums of multiplicative functions in O(N^(2/3)).
 *
 * Key idea:
 *   Discretize the set { n/i | 1 <= i <= n }. There are O(sqrt(n)) distinct values.
 *   Define g(k) = sum_{i=1}^{k} i * [i is prime]  (weighted prime indicator)
 *   Using the recurrence g(n) = g(n/p) + ... we can compute g values.
 *   Then S(n) = g(n) - 1  (subtract 1 to exclude the prime "1")
 *
 * Reference: kirainmoe's min25.tex
 */

#include <bits/stdc++.h>
using namespace std;

namespace Min25 {

static const int MAXN = 5000000;  // adjust based on problem constraints

bool is_composite[MAXN];
int primes[MAXN];
int pcnt = 0;

// discretization: distinct values of n / i
long long disc[MAXN];
int dcnt = 0;

// g[i] = sum_{j=1}^{i} j * [disc[j] is prime]
// we store g values for each discretized value
long long g[MAXN];

// mu[i] up to n^(2/3) for Dujiao helper
int mu[MAXN];

// helper: get index of v in disc[] (monotonic mapping)
inline int idx(long long v) {
    return lower_bound(disc, disc + dcnt, v) - disc;
}

/**
 * Preprocess primes and g values for all n / i <= n.
 * Call this once before using S(n).
 */
void init(long long n) {
    int limit = static_cast<int>(min<long long>(MAXN - 1, n));
    int sq = static_cast<int>(sqrt(n));

    // Sieve for mu and primes up to limit
    mu[1] = 1;
    for (int i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[pcnt++] = i;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt && 1LL * i * primes[j] <= limit; ++j) {
            long long p = 1LL * i * primes[j];
            if (p > limit) break;
            is_composite[p] = true;
            if (i % primes[j] == 0) {
                mu[p] = 0;
                break;
            } else {
                mu[p] = -mu[i];
            }
        }
    }

    // Discretize n / i for i = 1..n
    dcnt = 0;
    for (long long i = 1; i <= n; ) {
        long long v = n / i;
        disc[dcnt++] = v;
        i = n / v + 1;
    }

    // g[i] = sum_{j=1}^{i} j   (i.e., identity function prefix)
    // initially g[i] = disc[i] * (disc[i] + 1) / 2
    for (int i = 0; i < dcnt; ++i) {
        long long v = disc[i];
        g[i] = v * (v + 1) / 2;
    }

    // Sieve contribution: for each prime p, subtract p * (1 + sum of terms)
    for (int i = 0; i < pcnt && 1LL * primes[i] * primes[i] <= n; ++i) {
        int p = primes[i];
        long long pp = 1LL * p * p;

        // for each discretized value v, v >= pp
        for (int j = 0; j < dcnt && disc[j] >= pp; ++j) {
            long long v = disc[j];
            int k = idx(v / p);
            // g[j] = g[j] - p * (g[k] - sum_{q < p} q)  (standard Min25 recurrence)
            // simplified: subtract p times the contribution from the smaller bucket
            g[j] -= p * (g[k] - 1LL * p * (p - 1) / 2);
        }
    }

    // Subtract 1: remove the "prime 1" placeholder
    for (int i = 0; i < dcnt; ++i) {
        g[i] -= 1;
    }
}

/**
 * Returns the sum of all primes <= n.
 * Call init(n) first for the maximum n you will query.
 * After init, S(x) for any x <= original_n is available.
 *
 * Time: O(N^(2/3)) preprocessing, O(sqrt(n)) per query
 */
long long S(long long n) {
    if (n < MAXN && !is_composite[(int)min<long long>(n, MAXN - 1)]) {
        // small fast path
    }
    int i = idx(n);
    return g[i];
}

/**
 * Returns S(n) for arbitrary n (not requiring prior init up to n).
 * Calls init(n) internally if needed.
 */
long long sum_primes(long long n) {
    init(n);
    return S(n);
}

} // namespace Min25
