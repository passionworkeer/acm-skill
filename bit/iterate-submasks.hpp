#pragma once

/**
 * Submask Iteration Utilities
 *
 * for (int sub = mask; sub; sub = (sub-1) & mask)  // all non-zero submasks
 * for (int sub = mask; ; sub = (sub-1) & mask)    // includes 0
 *
 * Also includes supermask iteration:
 * for (int sup = mask; sup < (1<<n); sup = (sup+1) | mask)
 */

#include <bits/stdc++.h>
using namespace std;

// Count submasks of a given mask: O(1)
inline int countSubmasks(int mask) {
    return 1 << __builtin_popcount(mask);
}

// Iterate all submasks (including 0)
#define FOR_SUBSETS(mask) \
    for (int sub = mask; ; sub = (sub - 1) & mask)

// Iterate all non-zero submasks (common pattern)
#define FOR_NONZERO_SUBSETS(mask) \
    for (int sub = mask; sub; sub = (sub - 1) & mask)

// Iterate all submasks of sup that contain mask (submask of sup that is superset of mask)
inline void iterateSupersets(int mask, int sup, function<void(int)> cb) {
    int sub = sup;
    while (true) {
        if ((sub & mask) == mask) cb(sub);
        if (sub == 0) break;
        sub = (sub - 1) & sup;
    }
}

// Iterate all supermasks of s within [0, limit]
inline void iterateSupermasks(int s, int limit, function<void(int)> cb) {
    for (int sup = s; sup <= limit; sup = (sup + 1) | s) {
        cb(sup);
    }
}

// Non-recursive: enumerate all submasks of mask
inline vector<int> getAllSubmasks(int mask) {
    vector<int> res;
    for (int sub = mask; ; sub = (sub - 1) & mask) {
        res.push_back(sub);
        if (sub == 0) break;
    }
    return res;
}

// Check if a is a submask of b
inline bool isSubmask(int a, int b) {
    return (a & b) == a;
}

// Lowest set bit
inline int lowbit(int x) {
    return x & (-x);
}

// Next submask with same popcount (lexicographic next combination)
inline int nextSamePopcount(int x) {
    int t = x | (x - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(x) + 1));
}

// Check if adding bit i to mask keeps it a submask of sup
inline bool canAddBit(int mask, int i, int sup) {
    return ((mask | (1 << i)) & ~sup) == 0;
}

// ====================== 额外工具函数 ======================

/*
 * iterateSubmasks(mask, f)
 *
 * 遍历 mask 的所有非空子集，对每个 sub 调用 f(sub)。
 * 等价于 FOR_NONZERO_SUBSETS 宏的函数化版本。
 */
inline void iterateSubmasks(int mask, const function<void(int)>& f) {
    for (int sub = mask; sub; sub = (sub - 1) & mask) {
        f(sub);
    }
}

/*
 * iterateSubmasksWithZero(mask, f)
 *
 * 遍历 mask 的所有子集（包括空集 0）。
 */
inline void iterateSubmasksWithZero(int mask, const function<void(int)>& f) {
    for (int sub = mask; ; sub = (sub - 1) & mask) {
        f(sub);
        if (sub == 0) break;
    }
}

/*
 * iterateSubmasksDetailed(mask, f)
 *
 * 遍历子集并提供去低位后的值（sub & (sub-1)）。
 * 方便需要同时处理 sub 和 sub_without_lowbit 的场景。
 */
inline void iterateSubmasksDetailed(int mask, const function<void(int, int)>& f) {
    for (int sub = mask; sub; sub = (sub - 1) & mask) {
        f(sub, sub & (sub - 1));
    }
}

/*
 * sosDPAdd(a, dp)
 *
 * SOS DP（快速子集变换）：dp[mask] = Σ_{sub 是 mask 的子集} a[sub]
 *
 * 示例：
 *   dp = a;  // 初始化
 *   for (int i = 0; i < n; ++i)
 *       for (int mask = 0; mask < (1<<n); ++mask)
 *           if (mask & (1<<i)) dp[mask] += dp[mask ^ (1<<i)];
 *
 * 时间复杂度: O(n·2^n)
 */
inline void sosDPAdd(const vector<int>& a, vector<int>& dp) {
    dp = a;
    int n = dp.size();
    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < n; ++mask) {
            if (mask & (1 << i)) {
                dp[mask] += dp[mask ^ (1 << i)];
            }
        }
    }
}