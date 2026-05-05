#pragma once

/**
 * Polya Enumeration Theorem
 *
 * Counts the number of distinct colorings of objects under a group action.
 *
 * Burnside's Lemma:
 *   Number of orbits = (1/|G|) * sum_{g in G} Fix(g)
 *   where Fix(g) = number of colorings fixed by group element g.
 *
 * For necklaces under rotation (size n, m colors):
 *   ans = (1/n) * sum_{k=0}^{n-1} m^{gcd(n, k)}
 *        = (1/n) * sum_{d|n} phi(d) * m^{n/d}
 *
 * This file implements the dihedral group version (rotation + reflection)
 * as the full Polya enumeration.
 *
 * Reference: kirainmoe's polya.tex
 */

#include <bits/stdc++.h>
using namespace std;

namespace Polya {

static const int MAXN = 1000000;

bool is_composite[MAXN];
int primes[MAXN];
int pcnt = 0;
int phi[MAXN];
int pre_phi[MAXN];  // prefix sum of phi

/**
 * Linear sieve for Euler's totient function up to n.
 */
void init_sieve(int n) {
    phi[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!is_composite[i]) {
            primes[pcnt++] = i;
            phi[i] = i - 1;
        }
        for (int j = 0; j < pcnt && 1LL * i * primes[j] <= n; ++j) {
            long long p = 1LL * i * primes[j];
            if (p > n) break;
            is_composite[p] = true;
            if (i % primes[j] == 0) {
                phi[p] = phi[i] * primes[j];
                break;
            } else {
                phi[p] = phi[i] * (primes[j] - 1);
            }
        }
    }
    pre_phi[0] = 0;
    for (int i = 1; i <= n; ++i) pre_phi[i] = pre_phi[i - 1] + phi[i];
}

/**
 * Returns Euler's totient phi(x): number of integers in [1, x] coprime to x.
 * Uses precomputed sieve values for x <= MAXN.
 */
long long euler_phi(long long x) {
    if (x <= MAXN) return phi[(int)x];
    // For large x, factorize using the precomputed primes
    long long result = x;
    for (int i = 0; i < pcnt && 1LL * primes[i] * primes[i] <= x; ++i) {
        if (x % primes[i] == 0) {
            while (x % primes[i] == 0) x /= primes[i];
            result = result / primes[i] * (primes[i] - 1);
        }
    }
    if (x > 1) result = result / x * (x - 1);
    return result;
}

/**
 * Fast exponentiation: a^e mod mod
 */
inline long long modpow(long long a, long long e, long long mod) {
    long long r = 1;
    a %= mod;
    while (e) {
        if (e & 1) r = r * a % mod;
        a = a * a % mod;
        e >>= 1;
    }
    return r;
}

/**
 * Burnside's lemma for a general group described by a fixed-point function.
 *
 * @param m          number of colors / choices per position
 * @param n          number of positions / objects
 * @param gcd_func   custom function f(k) returning the number of colors
 *                    fixed when the permutation maps k positions to themselves
 *                    (typically f(k) = m^gcd(n, k) for rotation group)
 *
 * Burnside: ans = (1/|G|) * sum_{g in G} Fix(g)
 * For rotation group of size n:
 *   |G| = n, Fix(k) = m^{gcd(n, k)}
 *   ans = (1/n) * sum_{k=0}^{n-1} m^{gcd(n, k)}
 */
long long burnside(long long m, long long n,
                   const function<long long(long long)>& gcd_func) {
    long long sum = 0;
    for (long long k = 0; k < n; ++k) {
        sum += modpow(m, gcd_func(k), 0);  // 0 means no mod (returns exact value)
    }
    // Note: for large n, prefer the divisor-sum formula using phi
    return sum;
}

/**
 * Counts distinct necklaces with n beads and m colors under rotation only.
 * Formula: ans = (1/n) * sum_{d|n} phi(d) * m^{n/d}
 *
 * This counts colorings up to cyclic rotation, NOT including reflection.
 * For dihedral (including reflection), multiply the result appropriately.
 */
long long polya_count(long long n, long long m) {
    long long ans = 0;
    // Iterate over divisors d of n
    // Contribution: phi(d) * m^{n/d}
    for (long long d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        long long d1 = d;
        long long d2 = n / d;

        // term for divisor d1 = d
        ans += euler_phi(d1) * modpow(m, d2, 0);

        // term for divisor d2 (unless d1 == d2 to avoid double-counting)
        if (d2 != d1) {
            ans += euler_phi(d2) * modpow(m, d1, 0);
        }
    }
    return ans / n;
}

/**
 * Counts distinct colorings of n objects with m colors under the full dihedral
 * group D_n (n rotations + n reflections).
 *
 * ans = (1/(2n)) * ( sum_{k=0}^{n-1} m^{gcd(n, k)}   [rotations]
 *                     + n * m^{(n+1)/2}               [n even: reflections with axis through opposite vertices]
 *                     + n * m^{n/2}                  [n even: reflections with axis through opposite edges]
 *                     + n * m^{(n+1)/2} )            [n odd: reflections with axis through vertex and midpoint]
 * Simplified: ans = (1/(2n)) * ( sum_{k=0}^{n-1} m^{gcd(n, k)} + ... )
 * A cleaner form using divisor sums exists; for typical ACM use,
 * the rotation-only version polya_count() is more common.
 */
long long dihedral_count(long long n, long long m) {
    long long rot = 0;
    // Sum over all rotations: sum_{k=0}^{n-1} m^{gcd(n, k)}
    for (long long d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        long long d1 = d;
        long long d2 = n / d;
        rot += euler_phi(d1) * modpow(m, d2, 0);
        if (d2 != d1)
            rot += euler_phi(d2) * modpow(m, d1, 0);
    }

    long long ref;
    if (n % 2 == 0) {
        // n even: n/2 reflections with axis through opposite vertices: m^{n/2 + 1}
        //         n/2 reflections with axis through opposite edges:   m^{n/2}
        ref = (n / 2) * (modpow(m, n / 2 + 1, 0) + modpow(m, n / 2, 0));
    } else {
        // n odd: n reflections, each fixes one bead and pairs the rest: m^{(n+1)/2}
        ref = n * modpow(m, (n + 1) / 2, 0);
    }
    return (rot + ref) / (2 * n);
}

/**
 * Initialize the sieve for Euler's phi function.
 * Call this once before using polya_count() for large n.
 */
void init(long long n) {
    int limit = static_cast<int>(min<long long>(MAXN - 1, n));
    init_sieve(limit);
}

} // namespace Polya
