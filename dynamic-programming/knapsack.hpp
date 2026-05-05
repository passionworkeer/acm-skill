#pragma once

#include <bits/stdc++.h>
using namespace std;

namespace Knapsack {

// ------------------------------------------------------------
// Knapsack DP namespace
// All functions: O(capacity * n) or O(capacity) as noted.
// dp array convention: dp[c] = max value achievable with capacity c
// ------------------------------------------------------------

// ------------------------------------------------------------
// 0-1 Knapsack (01背包)
//
// Standard 0-1 knapsack: each item can be taken at most once.
// dp[c] = max(dp[c], dp[c-weight] + value)  for c from CAP down to w
// Time: O(CAP * n), Space: O(CAP)
//
// Parameters:
//   capacity : backpack capacity (int)
//   weight[] : array of item weights
//   value[]  : array of item values
//   n       : number of items
//
// Returns: max total value
// ------------------------------------------------------------
long long zeroOnePack(long long capacity,
                      const vector<long long>& weight,
                      const vector<long long>& value,
                      int n) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < n; ++i) {
        long long w = weight[i];
        long long v = value[i];
        for (long long c = capacity; c >= w; --c) {
            dp[c] = max(dp[c], dp[c - w] + v);
        }
    }
    return dp[capacity];
}

// overload with arrays
long long zeroOnePack(long long capacity,
                      const long long weight[],
                      const long long value[],
                      int n) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < n; ++i) {
        for (long long c = capacity; c >= weight[i]; --c) {
            dp[c] = max(dp[c], dp[c - weight[i]] + value[i]);
        }
    }
    return dp[capacity];
}

// 2D dp version (exact weight constraint)
long long zeroOnePackExact(long long capacity,
                           const vector<long long>& weight,
                           const vector<long long>& value,
                           int n) {
    vector<long long> dp(capacity + 1, -LLONG_MAX);
    dp[0] = 0;
    for (int i = 0; i < n; ++i) {
        for (long long c = capacity; c >= weight[i]; --c) {
            if (dp[c - weight[i]] != -LLONG_MAX) {
                dp[c] = max(dp[c], dp[c - weight[i]] + value[i]);
            }
        }
    }
    long long ans = 0;
    for (long long c = 0; c <= capacity; ++c) ans = max(ans, dp[c]);
    return ans;
}

// ------------------------------------------------------------
// Complete / Unlimited Knapsack (完全背包)
//
// Each item can be taken unlimited times.
// dp[c] = max(dp[c], dp[c-weight] + value)  for c from w to CAP
// Time: O(CAP * n), Space: O(CAP)
//
// Because we iterate forward, each item can be reused within
// the same iteration.
// ------------------------------------------------------------
long long completePack(long long capacity,
                       const vector<long long>& weight,
                       const vector<long long>& value,
                       int n) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < n; ++i) {
        long long w = weight[i];
        long long v = value[i];
        for (long long c = w; c <= capacity; ++c) {
            dp[c] = max(dp[c], dp[c - w] + v);
        }
    }
    return dp[capacity];
}

// with arrays
long long completePack(long long capacity,
                        const long long weight[],
                        const long long value[],
                        int n) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < n; ++i) {
        for (long long c = weight[i]; c <= capacity; ++c) {
            dp[c] = max(dp[c], dp[c - weight[i]] + value[i]);
        }
    }
    return dp[capacity];
}

// ------------------------------------------------------------
// Multiple Knapsack (多重背包)
//
// Item i has count[i] copies available.
// Simple version: expand into 01 items.
// Optimized O(CAP * n) using binary decomposition.
// ------------------------------------------------------------
long long multiPack(long long capacity,
                    const vector<long long>& weight,
                    const vector<long long>& value,
                    const vector<int>& count,
                    int n) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < n; ++i) {
        int cnt = count[i];
        long long w = weight[i];
        long long v = value[i];
        // Binary optimization: decompose cnt into 1,2,4,...,cnt-2^k+1
        int k = 1;
        while (cnt > 0) {
            int take = min(k, cnt);
            long long tw = take * w;
            long long tv = take * v;
            for (long long c = capacity; c >= tw; --c) {
                dp[c] = max(dp[c], dp[c - tw] + tv);
            }
            cnt -= take;
            k <<= 1;
        }
    }
    return dp[capacity];
}

// Monotone queue optimization for large counts (O(CAP) per item)
long long multiPackOptim(long long capacity,
                          const vector<long long>& weight,
                          const vector<long long>& value,
                          const vector<int>& count,
                          int n) {
    vector<long long> dp(capacity + 1, 0);
    for (int i = 0; i < n; ++i) {
        long long w = weight[i];
        long long v = value[i];
        int cnt = count[i];
        if (cnt == 0) continue;
        // Process each remainder class modulo w
        for (long long r = 0; r < w; ++r) {
            deque<pair<long long, long long>> dq;  // (capacity, dp value)
            for (long long c = r; c <= capacity; c += w) {
                long long cidx = c / w;
                // candidate value at this position
                long long val = dp[c] - cidx * v;
                // remove outdated entries
                while (!dq.empty() && dq.back().second <= val) dq.pop_back();
                dq.emplace_back(c, val);
                // remove entries out of window
                while (!dq.empty() && dq.front().first <= c - cnt * w) dq.pop_front();
                if (!dq.empty()) {
                    dp[c] = dq.front().second + cidx * v;
                }
            }
        }
    }
    return dp[capacity];
}

// ------------------------------------------------------------
// Group Knapsack (分组背包)
//
// n groups. From each group, choose at most 1 item.
// items[i] = vector of (weight, value) for group i
// dp[c] = max(dp[c], dp[c - w] + v) for each item in group
// ------------------------------------------------------------
long long groupPack(long long capacity,
                    const vector<vector<pair<long long,long long>>>& items) {
    vector<long long> dp(capacity + 1, 0);
    for (const auto& group : items) {
        for (long long c = capacity; c >= 0; --c) {
            for (const auto& [w, v] : group) {
                if (w <= c) {
                    dp[c] = max(dp[c], dp[c - w] + v);
                }
            }
        }
    }
    return dp[capacity];
}

// ------------------------------------------------------------
// Binary Optimization for 0-1 Knapsack (二进制优化)
//
// When many copies of the same item are available (zero-one case),
// decompose count into powers of 2. Converts to 0-1 knapsack.
// ------------------------------------------------------------
long long bitOptimZeroOne(long long capacity,
                          const vector<long long>& weight,
                          const vector<long long>& value,
                          const vector<int>& count,
                          int n) {
    vector<long long> nw, nv;
    for (int i = 0; i < n; ++i) {
        int c = count[i];
        long long w = weight[i], v = value[i];
        int k = 1;
        while (c > 0) {
            int take = min(k, c);
            nw.push_back(take * w);
            nv.push_back(take * v);
            c -= take;
            k <<= 1;
        }
    }
    vector<long long> dp(capacity + 1, 0);
    for (size_t i = 0; i < nw.size(); ++i) {
        for (long long c = capacity; c >= nw[i]; --c) {
            dp[c] = max(dp[c], dp[c - nw[i]] + nv[i]);
        }
    }
    return dp[capacity];
}

// ------------------------------------------------------------
// 0-1 knapsack with exact weight (bounded by capacity)
long long zeroOnePathCount(long long capacity,
                            const vector<long long>& weight,
                            const vector<long long>& value,
                            int n) {
    vector<long long> dp(capacity + 1, -LLONG_MAX);
    dp[0] = 0;
    for (int i = 0; i < n; ++i) {
        for (long long c = capacity; c >= weight[i]; --c) {
            if (dp[c - weight[i]] != -LLONG_MAX) {
                dp[c] = max(dp[c], dp[c - weight[i]] + value[i]);
            }
        }
    }
    return dp[capacity];
}

// ------------------------------------------------------------
// Complete knapsack: count number of ways to reach exact weight
long long completePackCountWays(long long capacity,
                                 const vector<long long>& weight,
                                 int n) {
    vector<long long> dp(capacity + 1, 0);
    dp[0] = 1;
    for (int i = 0; i < n; ++i) {
        for (long long c = weight[i]; c <= capacity; ++c) {
            dp[c] += dp[c - weight[i]];
        }
    }
    return dp[capacity];
}

// ------------------------------------------------------------
// Example / full test harness
// ------------------------------------------------------------
#ifdef KNAPSACK_MAIN
int main() {
    long long CAP = 10;
    // Items: weight, value pairs
    vector<long long> w = {3, 4, 5, 6};
    vector<long long> v = {2, 3, 4, 5};
    vector<int> cnt = {2, 2, 1, 3};

    cout << "0-1 knapsack max value: "
         << zeroOnePack(CAP, w, v, 4) << endl;

    cout << "Complete knapsack max value: "
         << completePack(CAP, w, v, 4) << endl;

    cout << "Multiple knapsack (binary) max value: "
         << multiPack(CAP, w, v, cnt, 4) << endl;

    cout << "Multiple knapsack (optim) max value: "
         << multiPackOptim(CAP, w, v, cnt, 4) << endl;

    cout << "Binary optimized 0-1 knapsack: "
         << bitOptimZeroOne(CAP, w, v, cnt, 4) << endl;

    // Group knapsack example:
    // Group 0: (weight=2, value=3), (3,4)
    // Group 1: (weight=4, value=5), (5,6)
    // Group 2: (weight=1, value=2)
    vector<vector<pair<long long,long long>>> groups = {
        {{2,3}, {3,4}},
        {{4,5}, {5,6}},
        {{1,2}}
    };
    cout << "Group knapsack max value: "
         << groupPack(CAP, groups) << endl;

    // Test count ways
    vector<long long> w2 = {1, 2, 3};
    cout << "Ways to fill capacity 5 (complete): "
         << completePackCountWays(5, w2, 3) << endl;

    return 0;
}
#endif
}  // namespace Knapsack
