#pragma once

/**
 * Catalan Numbers
 *
 * Computes C_n = binomial(2n, n) / (n + 1)  for arbitrary mod.
 *
 * Formula used:
 *   C_n = C(2n, n) * inv(n + 1)  (mod mod)
 *
 * Supports large n by precomputing factorials and inverse factorials.
 *
 * Reference: kirainmoe's catalan.tex
 */

#include <bits/stdc++.h>
using namespace std;

namespace Catalan {

static const int MAXN = 5000000;

long long mod;
vector<long long> fact(1, 1);
vector<long long> inv_fact(1, 1);
vector<long long> inv(1, 0);

/**
 * Fast exponentiation: a^e mod mod
 */
inline long long modpow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % mod;
        a = a * a % mod;
        e >>= 1;
    }
    return r;
}

/**
 * Precompute factorials, inverse factorials, and modular inverses up to n.
 */
void init(long long n_, long long mod_) {
    mod = mod_;
    int n = static_cast<int>(min<long long>(n_, MAXN));
    fact.resize(n + 1);
    inv_fact.resize(n + 1);
    inv.resize(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i % mod;
    inv_fact[n] = modpow(fact[n], mod - 2);
    for (int i = n; i >= 1; --i) inv_fact[i - 1] = inv_fact[i] * i % mod;
    inv[1] = 1;
    for (int i = 2; i <= n; ++i)
        inv[i] = mod - (mod / i) * inv[mod % i] % mod;
}

/**
 * Returns the binomial coefficient C(n, k) modulo mod.
 * Must call init() first.
 */
inline long long binom(int n, int k) {
    if (n < 0 || k < 0 || k > n) return 0;
    return fact[n] * inv_fact[k] % mod * inv_fact[n - k] % mod;
}

/**
 * Returns the n-th Catalan number modulo mod.
 * Uses: C_n = C(2n, n) / (n + 1)
 *
 * Requires init(2 * n, mod) to be called first.
 */
long long catalan(int n) {
    return binom(2 * n, n) * modpow(n + 1, mod - 2) % mod;
}

/**
 * Returns the n-th Catalan number modulo mod, auto-initializing if needed.
 */
long long compute(int n, long long mod_) {
    if (mod != mod_) init(2 * n, mod_);
    return catalan(n);
}

} // namespace Catalan
