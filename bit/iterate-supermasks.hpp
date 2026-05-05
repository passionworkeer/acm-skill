#pragma once

/**
 * Supermask Iteration Utilities
 *
 * Iterate all supermasks of a mask within a given limit:
 *
 *   for (int sup = s; sup < (1<<n); sup = (sup+1) | s)
 *
 * This is the standard technique to iterate all supersets of s
 * where sup is bounded by some limit.
 *
 * Usage:
 *   int n = 5; int mask = 1;  // supermasks of {0} within 5 bits
 *   for (int sup = mask; sup < (1<<n); sup = (sup+1) | mask) {
 *       // sup ranges over all 2^(n-1) supersets of mask
 *   }
 */

#include <bits/stdc++.h>
using namespace std;

// Iterate all supermasks of `mask` within `limit` (exclusive upper bound)
#define FOR_SUPERMASKS(mask, limit) \
    for (int sup = mask; sup < limit; sup = (sup + 1) | (mask))

// Count number of supermasks of mask within limit (limit must be power of 2)
inline int countSupermasks(int mask, int limit) {
    int bits = 0;
    for (int i = 0; i < 31; i++) {
        if (limit & (1 << i)) bits = i + 1;
    }
    int freeBits = bits - __builtin_popcount(mask);
    return 1 << freeBits;
}

// Collect all supermasks of mask within [mask, limit)
inline vector<int> getAllSupermasks(int mask, int limit) {
    vector<int> res;
    FOR_SUPERMASKS(mask, limit) {
        res.push_back(sup);
    }
    return res;
}

// Find the minimum supermask of mask with at most K bits set
inline int minSupermaskWithKBits(int mask, int K) {
    int result = mask;
    int extra = K - __builtin_popcount(mask);
    for (int i = 0; i < 31 && extra > 0; i++) {
        if (!(mask & (1 << i))) {
            result |= (1 << i);
            extra--;
        }
    }
    return result;
}

// Check if a is a supermask of b
inline bool isSupermask(int a, int b) {
    return (a | b) == a && (a & b) == b;
}