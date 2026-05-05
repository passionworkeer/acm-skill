#pragma once

#include <bits/stdc++.h>
using namespace std;

namespace DigitDP {

// ------------------------------------------------------------
// DigitDP<RT>: digit DP framework
//
// Template parameter RT  : return type (e.g. long long, int)
// The framework digitizes a number and runs DFS with memoization.
//
// Usage: subclass or pass a transition via lambda.
//
// Key methods:
//   - DigitDP(x)       : constructor, builds a[] from x
//   - solve(x)         : returns answer for range [0, x]
//   - dfs(pos,state,lead,limit) -> RT
//
// State encoding is problem-specific; subclasses define it.
// lead = true  : still all-leading-zero prefix
// limit = true : previous digits exactly matched bound, current
//               position is the tight upper bound
// ------------------------------------------------------------
template <typename RT = long long>
class DigitDP {
public:
    static const int MAXLEN = 25;   // up to 10^24 fits in 64-bit
    static const int MAXDIG = 12;  // max digits for dp dimension

    long long bound;          // upper bound being queried
    long long a[MAXLEN];      // digitized digits, a[0] = most significant
    int len;

    // dp[pos][state][lead][limit] -- stored as -1 for uncomputed
    // pos in [0, len], state is problem-specific
    mutable RT dp[MAXLEN][64][2][2];

    DigitDP() : bound(0), len(0) {
        memset(dp, -1, sizeof(dp));
        memset(a, 0, sizeof(a));
    }

    explicit DigitDP(long long x) { init(x); }

    // Build digit array from x. Digits stored MSB-first at index 0.
    void init(long long x) {
        bound = x;
        if (x < 0) {
            len = 1; a[0] = 0; return;
        }
        long long tmp = x;
        len = 0;
        while (tmp > 0) {
            a[len++] = tmp % 10;
            tmp /= 10;
        }
        if (len == 0) { len = 1; a[0] = 0; }
        // reverse so a[0] = most significant
        for (int i = 0; i < len / 2; ++i) swap(a[i], a[len - 1 - i]);
        memset(dp, -1, sizeof(dp));
    }

    // --------------------------------------------------------
    // dfs: base method -- override addTransition() for logic
    // pos  : current digit index (0 = most significant)
    // state: problem-specific state (e.g. mod sum)
    // lead : true if all previous digits are 0
    // limit: true if previous digits exactly follow bound
    // --------------------------------------------------------
    virtual RT dfs(int pos, int state, bool lead, bool limit) const {
        if (pos == len) {
            // Override in subclass for terminal condition
            return RT(0);
        }
        RT& memo = dp[pos][state][lead][limit];
        if (memo != RT(-1)) return memo;
        memo = RT(0);
        int up = limit ? a[pos] : 9;
        for (int d = 0; d <= up; ++d) {
            bool nlead = lead && (d == 0);
            bool nlimit = limit && (d == up);
            int nstate = addTransition(state, d, nlead);
            memo += dfs(pos + 1, nstate, nlead, nlimit);
        }
        return memo;
    }

    // --------------------------------------------------------
    // addTransition: update state after choosing digit d
    // Override this for problem-specific logic.
    // Default: state is just the count of non-leading digits.
    // --------------------------------------------------------
    virtual int addTransition(int state, int digit, bool lead) const {
        if (lead) return state;
        return state + 1;
    }

    // --------------------------------------------------------
    // solve: answer for range [0, bound] (inclusive)
    // To query [l, r]: solve(r) - solve(l-1)
    // --------------------------------------------------------
    RT solve() const { return dfs(0, 0, true, true); }

    RT solve(long long x) {
        init(x);
        return solve();
    }

    // --------------------------------------------------------
    // Range query: [L, R] inclusive.  Pass R as bound.
    // (L defaults to 0.)
    // --------------------------------------------------------
    RT rangeSolve(long long L, long long R) {
        if (L <= 0) return solve(R);
        // solve(R) - solve(L-1)
        long long old_bound = bound;
        init(R);
        RT resR = solve();
        init(L - 1);
        RT resL = solve();
        bound = old_bound;
        return resR - resL;
    }
};

// ------------------------------------------------------------
// Example 1: Count numbers in [0, x] that do NOT contain digit 4
// ------------------------------------------------------------
class NoFourDP : public DigitDP<long long> {
public:
    NoFourDP(long long x) : DigitDP<long long>(x) {}

    long long dfs(int pos, int state, bool lead, bool limit) const override {
        if (pos == len) return 1LL;  // valid number constructed

        long long key = (long long)pos * 1000 + state * 100 + (lead ? 10 : 0) + (limit ? 1 : 0);
        // Use address as memo key since dp is large
        // Actually use the full tuple lookup
        long long& memo = dp[pos][state][lead][limit];
        if (memo != -1) return memo;
        memo = 0;

        int up = limit ? a[pos] : 9;
        for (int d = 0; d <= up; ++d) {
            if (d == 4) continue;
            bool nlead = lead && (d == 0);
            bool nlimit = limit && (d == up);
            memo += dfs(pos + 1, state, nlead, nlimit);
        }
        return memo;
    }
};

inline long long countNoFour(long long x) {
    NoFourDP dp(x);
    return dp.solve();
}

// ------------------------------------------------------------
// Example 2: Count numbers in [0, x] where sum of digits % 7 == 0
// ------------------------------------------------------------
class DigitSumMod7DP : public DigitDP<long long> {
public:
    DigitSumMod7DP(long long x) : DigitDP<long long>(x) {}

    int addTransition(int state, int digit, bool lead) const override {
        if (lead) return digit % 7;   // leading zeros contribute 0 to sum
        return (state + digit) % 7;
    }

    long long dfs(int pos, int state, bool lead, bool limit) const override {
        long long& memo = dp[pos][state][lead][limit];
        if (memo != -1) return memo;
        memo = 0;

        if (pos == len) {
            return memo = (state == 0) ? 1LL : 0LL;
        }

        int up = limit ? a[pos] : 9;
        for (int d = 0; d <= up; ++d) {
            bool nlead = lead && (d == 0);
            bool nlimit = limit && (d == up);
            int nstate = addTransition(state, d, nlead);
            memo += dfs(pos + 1, nstate, nlead, nlimit);
        }
        return memo;
    }
};

inline long long countDigitSumMod7(long long x) {
    DigitSumMod7DP dp(x);
    return dp.solve();
}

// ------------------------------------------------------------
// Example 3: Count numbers in [0, x] with at most k non-zero digits
// ------------------------------------------------------------
class BoundedDigitCountDP : public DigitDP<long long> {
public:
    int K;
    BoundedDigitCountDP(long long x, int k) : DigitDP<long long>(x), K(k) {}

    int addTransition(int state, int digit, bool lead) const override {
        if (lead) return state;
        return state + (digit == 0 ? 0 : 1);
    }

    long long dfs(int pos, int state, bool lead, bool limit) const override {
        long long& memo = dp[pos][state][lead][limit];
        if (memo != -1) return memo;
        memo = 0;

        if (pos == len) {
            return memo = (state <= K) ? 1LL : 0LL;
        }

        int up = limit ? a[pos] : 9;
        for (int d = 0; d <= up; ++d) {
            bool nlead = lead && (d == 0);
            bool nlimit = limit && (d == up);
            int nstate = addTransition(state, d, nlead);
            if (nstate > K) continue;
            memo += dfs(pos + 1, nstate, nlead, nlimit);
        }
        return memo;
    }
};

inline long long countBoundedDigit(long long x, int k) {
    BoundedDigitCountDP dp(x, k);
    return dp.solve();
}

// ------------------------------------------------------------
// Example 4: Count numbers in [L, R] divisible by m
// state = current remainder, lead = leading zeros
// ------------------------------------------------------------
class DivisibleByMDP : public DigitDP<long long> {
public:
    long long M;
    DivisibleByMDP(long long x, long long m) : DigitDP<long long>(x), M(m) {}

    int addTransition(int state, int digit, bool lead) const override {
        if (lead) return digit % M;
        return (state * 10 + digit) % M;
    }

    long long dfs(int pos, int state, bool lead, bool limit) const override {
        long long& memo = dp[pos][state][lead][limit];
        if (memo != -1) return memo;
        memo = 0;

        if (pos == len) {
            return memo = (state == 0) ? 1LL : 0LL;
        }

        int up = limit ? a[pos] : 9;
        for (int d = 0; d <= up; ++d) {
            bool nlead = lead && (d == 0);
            bool nlimit = limit && (d == up);
            int nstate = addTransition(state, d, nlead);
            memo += dfs(pos + 1, nstate, nlead, nlimit);
        }
        return memo;
    }
};

inline long long countDivisibleBy(long long x, long long m) {
    DivisibleByMDP dp(x, m);
    return dp.solve();
}

// ------------------------------------------------------------
// Example usage in main()
// ------------------------------------------------------------
#ifdef DIGIT_DP_MAIN
int main() {
    long long test = 12345;

    cout << "Test bound: " << test << endl;
    cout << "No digit 4 in [0," << test << "]: "
         << countNoFour(test) << endl;
    cout << "Digit sum % 7 == 0 in [0," << test << "]: "
         << countDigitSumMod7(test) << endl;
    cout << "At most 3 non-zero digits in [0," << test << "]: "
         << countBoundedDigit(test, 3) << endl;
    cout << "Divisible by 7 in [0," << test << "]: "
         << countDivisibleBy(test, 7) << endl;

    // Range query: [100, 1000] with no digit 4
    long long ans_range = countNoFour(1000) - countNoFour(99);
    cout << "No digit 4 in [100,1000]: " << ans_range << endl;

    return 0;
}
#endif
}  // namespace DigitDP
