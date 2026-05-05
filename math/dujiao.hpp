#pragma once

/**
 * Dujiao Sieve (杜教筛)
 *
 * Computes prefix sums of multiplicative functions using Dirichlet convolution.
 *
 * Key identities used:
 *   (1)  mu * 1 = epsilon  =>  S_mu(n) = 1 - sum_{i=2}^{n} S_mu(floor(n/i))
 *   (2)  phi * 1 = id      =>  S_phi(n) = n*(n+1)/2 - sum_{i=2}^{n} S_phi(floor(n/i))
 *
 * Precompute mu[1..n^(2/3)] with linear sieve for the base case.
 * Use hashmap (unordered_map) for memoization of larger values.
 *
 * Reference: kirainmoe's du.tex
 */

#include <bits/stdc++.h>
using namespace std;

namespace Dujiao {

static const int MAXN = 5000000;  // precompute limit: n^(2/3)

bool is_composite[MAXN];
int primes[MAXN];
int pcnt = 0;
int mu[MAXN];        // Möbius function values
long long pre_mu[MAXN]; // prefix sum of mu

// memoization hashmap
unordered_map<long long, long long> memo_mu;
unordered_map<long long, long long> memo_phi;

/**
 * Linear sieve up to MAXN for mu and its prefix sum.
 */
void init_sieve(int limit) {
    mu[1] = 1;
    pre_mu[1] = 1;
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
        pre_mu[i] = pre_mu[i - 1] + mu[i];
    }
}

/**
 * S(n) = 1 - sum_{i=2}^{n} S(floor(n/i))
 * Recurrence derived from: sum_{d|n} mu(d) = [n==1]
 *
 * Time complexity: O(n^(2/3)) with memoization
 */
long long S_mu(long long n) {
    if (n <= MAXN) return pre_mu[n];

    auto it = memo_mu.find(n);
    if (it != memo_mu.end()) return it->second;

    long long res = 1;  // base case: n=1 contributes 1
    for (long long i = 2, j; i <= n; ) {
        long long q = n / i;
        j = n / q;
        res -= (j - i + 1) * S_mu(q);
        i = j + 1;
    }
    memo_mu[n] = res;
    return res;
}

/**
 * S_phi(n) = n*(n+1)/2 - sum_{i=2}^{n} S_phi(floor(n/i))
 * Recurrence derived from: phi * 1 = id
 * i.e., sum_{d|n} phi(d) = n
 *
 * Time complexity: O(n^(2/3)) with memoization
 */
long long S_phi(long long n) {
    if (n <= MAXN) {
        long long res = 0;
        for (int i = 1; i <= n; ++i) res += mu[i] * (n / i) * (n / i + 1) / 2;
        return res;
    }

    auto it = memo_phi.find(n);
    if (it != memo_phi.end()) return it->second;

    // S_phi(n) = 1 + sum_{i=2}^{n} phi(i)
    // Using identity sum_{d|n} phi(d) = n:
    //   n*(n+1)/2 = sum_{i=1}^{n} sum_{d|i} phi(d)
    //             = sum_{d=1}^{n} phi(d) * floor(n/d)
    //             = sum_{i=1}^{n} S_phi(floor(n/i))
    // So: S_phi(n) = n*(n+1)/2 - sum_{i=2}^{n} S_phi(floor(n/i))
    long long res = n * (n + 1) / 2;
    for (long long i = 2, j; i <= n; ) {
        long long q = n / i;
        j = n / q;
        res -= (j - i + 1) * S_phi(q);
        i = j + 1;
    }
    memo_phi[n] = res;
    return res;
}

/**
 * Initialize the sieve up to n^(2/3) for efficient queries.
 * Call this once before using S_mu or S_phi.
 *
 * @param n  The maximum value you will query
 */
void init(long long n) {
    int limit = static_cast<int>(pow(n, 2.0 / 3.0)) + 10;
    limit = min(limit, MAXN - 1);
    init_sieve(limit);
}

/**
 * Prefix sum of Möbius function: sum_{i=1}^{n} mu(i)
 */
inline long long sum_mu(long long n) { return S_mu(n); }

/**
 * Prefix sum of Euler's totient: sum_{i=1}^{n} phi(i)
 */
inline long long sum_phi(long long n) { return S_phi(n); }

} // namespace Dujiao
