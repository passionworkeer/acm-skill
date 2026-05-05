#pragma once

#include <bits/stdc++.h>
using namespace std;

namespace ArbitraryNTT {

// Three moduli and their primitive roots
const long long MODS[3] = {998244353, 1004535809, 469762049};
const long long G = 3;  // primitive root for all three moduli
const int NUM_MODS = 3;

// ------------------------------------------------------------
// Precomputed roots of unity for each modulus
// ------------------------------------------------------------
long long mod_pow(long long a, long long e, long long mod) {
    long long r = 1;
    while (e) {
        if (e & 1) r = (__int128)r * a % mod;
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return r;
}

// ------------------------------------------------------------
// Iterative NTT: operates on vector<long long> interpreted
// under a specific mod (passed as parameter).
// invert = false -> forward NTT, true -> inverse NTT
// ------------------------------------------------------------
void ntt(vector<long long>& a, size_t n, bool invert, long long mod) {
    // bit-reversal permutation
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (size_t len = 2; len <= n; len <<= 1) {
        long long wlen = mod_pow(G, (mod - 1) / len, mod);
        if (invert) wlen = mod_pow(wlen, mod - 2, mod);
        for (size_t i = 0; i < n; i += len) {
            long long w = 1;
            for (size_t j = 0; j < len / 2; ++j) {
                long long u = a[i + j];
                long long v = (__int128)a[i + j + len / 2] * w % mod;
                a[i + j] = (u + v) % mod;
                a[i + j + len / 2] = (u - v + mod) % mod;
                w = (__int128)w * wlen % mod;
            }
        }
    }

    if (invert) {
        long long inv_n = mod_pow(n, mod - 2, mod);
        for (auto& x : a) x = (__int128)x * inv_n % mod;
    }
}

// ------------------------------------------------------------
// Multiply polynomials a (size na) and b (size nb) under one
// modulus. Returns vector of coefficients modulo mod.
// ------------------------------------------------------------
vector<long long> multiply_mod(const vector<long long>& a,
                                const vector<long long>& b,
                                long long mod) {
    size_t na = a.size(), nb = b.size();
    if (!na || !nb) return {};

    size_t result_size = na + nb - 1;
    size_t n = 1;
    while (n < result_size) n <<= 1;

    vector<long long> fa(n, 0), fb(n, 0);
    for (size_t i = 0; i < na; ++i) fa[i] = a[i] % mod;
    for (size_t i = 0; i < nb; ++i) fb[i] = b[i] % mod;

    ntt(fa, n, false, mod);
    ntt(fb, n, false, mod);
    for (size_t i = 0; i < n; ++i) fa[i] = (__int128)fa[i] * fb[i] % mod;
    ntt(fa, n, true, mod);

    vector<long long> res(result_size);
    for (size_t i = 0; i < result_size; ++i) res[i] = fa[i] % mod;
    return res;
}

// ------------------------------------------------------------
// CRT: merge (x1, m1) and (x2, m2) -> (x, lcm(m1,m2))
// m1 and m2 must be coprime.
// x = x1 + m1 * ((x2 - x1) * inv(m1) mod m2)
// result modulo = m1 * m2
// ------------------------------------------------------------
long long crt_merge(long long x1, long long m1, long long x2, long long m2) {
    long long d = std::gcd(m1, m2);
    long long m1d = m1 / d;
    long long x = ( (__int128)((x2 - x1) / d % m2 + m2) % m2 )
                * mod_pow(m1d % m2, m2 - 2, m2) % m2;
    x = (x1 + (__int128)m1 * x) % (m1 / d * m2);
    if (x < 0) x += m1 / d * m2;
    return x;
}

// ------------------------------------------------------------
// Garner-like CRT for three coprime moduli (CRT chain)
// Returns combined value modulo M = m1*m2*m3
// ------------------------------------------------------------
long long crt_merge3(long long x1, long long m1,
                     long long x2, long long m2,
                     long long x3, long long m3) {
    long long x12 = crt_merge(x1, m1, x2, m2);
    long long m12 = m1 / std::gcd(m1, m2) * m2;
    return crt_merge(x12, m12, x3, m3);
}

// ------------------------------------------------------------
// Multiply polynomials a and b under arbitrary modulus P.
// P can be any integer up to 1e9 (does not need to be prime).
// Strategy: compute result modulo each of the three moduli,
//          then reconstruct via CRT.
// ------------------------------------------------------------
vector<long long> multiply(const vector<long long>& a, size_t na,
                           const vector<long long>& b, size_t nb,
                           long long P) {
    // Results under each modulus
    vector<long long> r1 = multiply_mod(a, b, MODS[0]);
    vector<long long> r2 = multiply_mod(a, b, MODS[1]);
    vector<long long> r3 = multiply_mod(a, b, MODS[2]);

    size_t result_size = na + nb - 1;
    vector<long long> result(result_size);
    long long M12 = MODS[0] * MODS[1];  // ~1e18, fits in unsigned __int128

    for (size_t i = 0; i < result_size; ++i) {
        // Combine mod 1 and mod 2 first
        long long x12 = crt_merge(r1[i], MODS[0], r2[i], MODS[1]);
        // Combine with mod 3
        long long x123 = crt_merge(x12, M12, r3[i], MODS[2]);
        // Reduce to target modulus P
        result[i] = x123 % P;
    }
    return result;
}

// ------------------------------------------------------------
// Convenience overload: input as vectors, result as vector
// ------------------------------------------------------------
vector<long long> multiply(const vector<long long>& a,
                           const vector<long long>& b,
                           long long P) {
    return multiply(a, a.size(), b, b.size(), P);
}

// ------------------------------------------------------------
// Example / test harness
// ------------------------------------------------------------
#ifdef ARBITRARYNTT_MAIN
#include <cstdio>
int main() {
    // Example: (1 + 2x) * (3 + 4x) = 3 + 10x + 8x^2
    vector<long long> A = {1, 2};
    vector<long long> B = {3, 4};

    // Test with several arbitrary moduli
    long long test_mods[] = {100, 997, 1'000'000'007, 1'234'567'890};

    for (long long mod : test_mods) {
        vector<long long> C = multiply(A, B, mod);
        cout << "mod=" << mod << " => ";
        for (size_t i = 0; i < C.size(); ++i) {
            cout << C[i] << (i + 1 == C.size() ? "\n" : " ");
        }
    }
    // Expected for mod=1e9+7: C = [3, 10, 8]
    // Expected for mod=1: all zeros
    return 0;
}
#endif
}  // namespace ArbitraryNTT
