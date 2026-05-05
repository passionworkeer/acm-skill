/**
 * Slope Optimization / Convex Hull Trick (CHT)
 *
 * Optimizes DP transitions of the form:
 *   dp[i] = min_{j < i} { dp[j] + cost(j, i) }
 * where cost(j, i) can be expressed as:
 *   cost(j, i) = m_j * X_i + b_j
 *
 * Here, each decision point j defines a line:
 *   f_j(x) = m_j * x + b_j
 * The query at x = X_i asks for the minimum (or maximum) value among all lines.
 *
 * This header supports two modes:
 *   MIN: lower hull, query returns minimum
 *   MAX: upper hull, query returns maximum
 *
 * It handles two scenarios:
 *   1. Slopes added in monotonic order, queries in any order
 *      -> Pointer method (prune dominated lines, binary search)
 *   2. Queries (x) in monotonic order (increasing), slopes arbitrary
 *      -> Deque method (amortized O(1) per operation)
 *
 * The classic "Land Acquisition" (IOI 2002 / POJ 3661) problem:
 *   Given n rectangles, choose a subsequence such that each selected
 *   rectangle can contain the next (w_i >= w_{i+1} and h_i >= h_{i+1}).
 *   Minimize total area sum.
 *   Sorting by w ascending (and h descending for equal w), then applying
 *   slope DP gives O(n log n) solution.
 *
 * Reference: cp-algorithms.com - Convex Hull Trick
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// ---------------------------------------------------------------------------
// Section 1: Core Line and Hull Structures
// ---------------------------------------------------------------------------

/**
 * A line represented as y = m * x + b.
 *
 * The intersection point of two lines (l1, l2) is at:
 *   x = (b2 - b1) / (m1 - m2)
 *
 * We store lines with ll coefficients; the query x is also ll.
 * For division comparisons, we use cross-multiplication to avoid
 * floating-point errors.
 */
struct Line {
    ll m;  // slope
    ll b;  // intercept

    /** Evaluate this line at x. */
    ll value(ll x) const { return m * x + b; }
};

/**
 * Determine whether line l3 makes the middle line l2 obsolete when added
 * to the lower hull, given that lines are added in order of decreasing slope.
 *
 * l2 is obsolete if the intersection of (l1, l3) occurs to the left of
 * or at the intersection of (l1, l2).
 *
 * In other words, using cross-multiplication:
 *   (b3 - b1) / (m1 - m3) <= (b2 - b1) / (m1 - m2)
 *   => (b3 - b1) * (m1 - m2) <= (b2 - b1) * (m1 - m3)
 *
 * We return true when l2 is unnecessary.
 */
inline bool bad(const Line& l1, const Line& l2, const Line& l3) {
    // (b3 - b1) / (m1 - m3) <= (b2 - b1) / (m1 - m2)
    // Cross-multiply carefully: avoid overflow in 64-bit by checking signs.
    // Since slopes are usually monotonic in practice, we use __int128 for safety.
    __int128 left  = (__int128)(l3.b - l1.b) * (l1.m - l2.m);
    __int128 right = (__int128)(l2.b - l1.b) * (l1.m - l3.m);
    return left <= right;
}

namespace SlopeDP {

// ---------------------------------------------------------------------------
// Section 2: Convex Hull with Pointer (slopes monotonic, x arbitrary)
// ---------------------------------------------------------------------------

/**
 * Convex Hull for the case where lines are added in order of decreasing slope
 * and queries have arbitrary x values.
 *
 * This uses a deque of lines and binary search on the hull.
 * The pointer method: maintain an index `ptr` into the deque such that
 * hull[ptr] gives the minimum (or maximum) at the current x.
 * When x is non-decreasing, ptr only moves forward (amortized O(1)).
 *
 * Template parameter MODE:
 *   MIN_MODE = 0 : minimize (lower hull)
 *   MAX_MODE = 1 : maximize (upper hull)
 */
template <int MODE>
struct HullPointer {
    deque<Line> hull;  // lines in hull order
    int ptr = 0;        // query pointer (for monotonic x queries)

    /** Add a new line (slope, intercept) to the hull. */
    void add(ll m, ll b) {
        Line newLine{m, b};

        // Remove last line while it becomes obsolete by the new one
        while (hull.size() >= 2 && bad(hull[hull.size() - 2], hull[hull.size() - 1], newLine)) {
            hull.pop_back();
        }
        hull.push_back(newLine);
    }

    /** Add a line from a DP state j (given its slope coefficient and intercept). */
    void addLine(ll m, ll b) { add(m, b); }

    /**
     * Query the minimum (or maximum) value at x.
     * Uses binary search to find the best line in O(log n).
     */
    ll query(ll x) const {
        if (hull.empty()) return MODE ? LLONG_MIN : LLONG_MAX;

        int lo = 0, hi = (int)hull.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (MODE == 0) {
                // MIN: choose the smaller value at mid
                if (hull[mid].value(x) <= hull[mid + 1].value(x))
                    hi = mid;
                else
                    lo = mid + 1;
            } else {
                // MAX: choose the larger value at mid
                if (hull[mid].value(x) >= hull[mid + 1].value(x))
                    hi = mid;
                else
                    lo = mid + 1;
            }
        }
        return hull[lo].value(x);
    }

    /**
     * Query with a monotonic x pointer.
     * Assumes x is non-decreasing across calls.  Advances ptr forward
     * until hull[ptr] is no longer worse than hull[ptr+1].
     * Amortized O(1) per query.
     */
    ll queryMonotonic(ll x) {
        if (hull.empty()) return MODE ? LLONG_MIN : LLONG_MAX;
        while (ptr + 1 < (int)hull.size()) {
            if (MODE == 0) {
                // MIN: if next line is better, advance pointer
                if (hull[ptr].value(x) >= hull[ptr + 1].value(x))
                    ++ptr;
                else
                    break;
            } else {
                if (hull[ptr].value(x) <= hull[ptr + 1].value(x))
                    ++ptr;
                else
                    break;
            }
        }
        return hull[ptr].value(x);
    }

    void clear() {
        hull.clear();
        ptr = 0;
    }
};

// ---------------------------------------------------------------------------
// Section 3: Convex Hull with Deque (x monotonic, slopes arbitrary)
// ---------------------------------------------------------------------------

/**
 * Convex Hull for the case where x values in queries are strictly increasing
 * (or decreasing) and lines can be added in any order.
 *
 * Each line in the deque is active in an interval of x.
 * Since x is monotonic, we only need to check the front of the deque.
 *
 * When adding a new line, we remove from the back while the last line
 * is made obsolete.  When querying, we pop from the front while the
 * next line gives a better value.
 */
template <int MODE>
struct HullDeque {
    deque<Line> hull;

    void add(ll m, ll b) {
        Line newLine{m, b};

        // Remove lines from back while they are dominated
        while (hull.size() >= 2 && bad(hull[hull.size() - 2], hull[hull.size() - 1], newLine)) {
            hull.pop_back();
        }
        hull.push_back(newLine);
    }

    /**
     * Query at x, assuming x is non-decreasing across calls.
     * Pops from front while the next line gives a better result.
     * Amortized O(1) per query.
     */
    ll query(ll x) {
        if (hull.empty()) return MODE ? LLONG_MIN : LLONG_MAX;

        if (MODE == 0) {
            // MIN: while next line is better, pop front
            while (hull.size() >= 2 && hull[0].value(x) >= hull[1].value(x)) {
                hull.pop_front();
            }
        } else {
            while (hull.size() >= 2 && hull[0].value(x) <= hull[1].value(x)) {
                hull.pop_front();
            }
        }
        return hull[0].value(x);
    }

    /** Query for arbitrary x (O(log n) via binary search). */
    ll queryAll(ll x) const {
        if (hull.empty()) return MODE ? LLONG_MIN : LLONG_MAX;
        int lo = 0, hi = (int)hull.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (MODE == 0) {
                if (hull[mid].value(x) <= hull[mid + 1].value(x)) hi = mid;
                else lo = mid + 1;
            } else {
                if (hull[mid].value(x) >= hull[mid + 1].value(x)) hi = mid;
                else lo = mid + 1;
            }
        }
        return hull[lo].value(x);
    }

    void clear() { hull.clear(); }
};

// ---------------------------------------------------------------------------
// Section 4: Slope Calculation Utilities
// ---------------------------------------------------------------------------

/**
 * Compute the slope of the line between two DP states:
 *   slope(i, j) = (dp[j] - dp[i]) / (x[j] - x[i])
 *
 * Used when building DP transitions where the cost function involves
 * products of indices.  Returns as a double (use with caution).
 *
 * Note: x values must be distinct and in increasing order.
 */
double slope(int i, int j, const vector<ll>& dp, const vector<ll>& x) {
    if (x[j] == x[i]) return 0;  // undefined; handle separately
    return double(dp[j] - dp[i]) / double(x[j] - x[i]);
}

/**
 * Compute the slope as a rational (m, b) for exact line representation.
 * The line passing through (x[i], dp[i]) and (x[j], dp[j]) is:
 *   y = m * t + b,  where m = (dp[j]-dp[i])/(x[j]-x[i])
 *   b = dp[i] - m * x[i]
 *
 * Returns {m, b} pair.  b may be fractional; for exact arithmetic
 * store as a pair of numerator/denominator if needed.
 */
pair<ll, ll> lineFromPoints(int i, int j, const vector<ll>& dp, const vector<ll>& x) {
    ll dx = x[j] - x[i];
    ll dy = dp[j] - dp[i];
    ll m = dy / dx;        // integer division; may lose precision
    ll b = dp[i] - m * x[i];
    return {m, b};
}

// ---------------------------------------------------------------------------
// Section 5: Common DP Patterns
// ---------------------------------------------------------------------------

/**
 * Standard DP with slope optimization:
 *   dp[i] = min_{j < i} { dp[j] + cost(j, i) }
 * where cost(j, i) = m_j * X_i + b_j for precomputed m_j, b_j.
 *
 * The caller provides:
 *   - dp[0] = 0 (base case)
 *   - For each j: lines of the form y = m_j * x + b_j
 *   - For each i: query at X_i
 *
 * @param n          number of states
 * @param xs         array of query x values (X_1..X_n), strictly increasing
 * @param slopes     m_j for each j (slopes of lines from j)
 * @param intercepts b_j for each j (intercepts of lines from j)
 * @param MODE       0 = MIN, 1 = MAX
 * @return           dp array
 */
template <int MODE>
vector<ll> slopeDP(int n, const vector<ll>& xs,
                   const vector<ll>& slopes,
                   const vector<ll>& intercepts) {
    vector<ll> dp(n + 1, 0);
    dp[0] = 0;

    HullDeque<MODE> hull;
    // Initialize with j=0
    hull.add(slopes[0], intercepts[0]);

    for (int i = 1; i <= n; ++i) {
        ll x = xs[i - 1];
        dp[i] = hull.query(x);
        // Add line for state i (will be queried by future states)
        hull.add(slopes[i], intercepts[i]);
    }
    return dp;
}

// ---------------------------------------------------------------------------
// Section 6: Land Acquisition (IOI 2002 / POJ 3661)
// ---------------------------------------------------------------------------

/**
 * Solve the "Land Acquisition" problem using slope optimization.
 *
 * Problem: Given n rectangles with width w_i and height h_i.
 *   Select a subsequence such that each rectangle can contain the next:
 *     w_i >= w_j and h_i >= h_j whenever i appears before j in the subsequence.
 *   Minimize the total area sum of selected rectangles.
 *
 * Key insight:
 *   Sort rectangles by w ascending; for equal w, sort by h descending.
 *   After sorting, only heights matter (widths are non-decreasing).
 *   The DP becomes:
 *     dp[i] = min_{j < i, h_j >= h_i} { dp[j] + w_i * h_j }
 *   This is of the form: dp[i] = min_j { dp[j] + w_i * h_j }
 *   where w_i is the query x and h_j is the slope.
 *
 *   Sort rectangles by w ascending and also remove dominated ones
 *   (where a later rectangle has both >= w and >= h).
 *
 * @param w  widths (sorted ascending, deduped)
 * @param h  heights (corresponding to sorted order)
 * @return   minimum total area
 */
ll landAcquisition(const vector<ll>& w, const vector<ll>& h) {
    int n = (int)w.size();
    if (n == 0) return 0;

    // Build convex hull for minimizing with queries in increasing w
    HullDeque<0> hull;  // MIN mode

    vector<ll> dp(n + 1, 0);
    dp[0] = 0;

    // Add the first line: y = h[0] * x + dp[0]
    hull.add(h[0], dp[0]);

    for (int i = 1; i < n; ++i) {
        // Query at x = w[i] (non-decreasing because w is sorted ascending)
        dp[i] = hull.query(w[i]);

        // Add line for this state: y = h[i] * x + dp[i]
        // (will be queried by future states with larger x)
        hull.add(h[i], dp[i]);
    }

    // Note: total area = sum of w_i * h_{selected} where selection follows
    // the non-dominated property. After removing dominated rectangles,
    // the DP computes the optimal selection.
    return dp[n - 1];
}

/**
 * Remove dominated rectangles from a set sorted by w ascending.
 * A rectangle i dominates j if w_i >= w_j and h_i >= h_j.
 * After sorting by w asc, h desc, we keep only rectangles with strictly
 * increasing h (these are the "upper convex hull" of valid states).
 */
vector<pair<ll,ll>> removeDominated(const vector<pair<ll,ll>>& rects) {
    vector<pair<ll,ll>> filtered;
    for (auto& r : rects) {
        // Sort by w asc; for same w keep largest h
        while (!filtered.empty() && filtered.back().second <= r.second) {
            filtered.pop_back();
        }
        filtered.push_back(r);
    }
    return filtered;
}

}  // namespace SlopeDP

// -------------------------------------------------------------------------
// Example: Land Acquisition
// -------------------------------------------------------------------------
#ifdef SLOPE_MAIN

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example: 4 rectangles (w, h)
    // Sort by w ascending, h descending: (2,4), (3,6), (5,3), (8,5)
    // After removing dominated (keep increasing h): (2,4), (3,6), (8,5)
    // DP:
    //   dp[0] = 0
    //   dp[1] = dp[0] + w1*h0 = 0 + 2*4 = 8
    //   dp[2] = min(dp[1]+w2*h1, dp[0]+w2*h0) = min(8+3*6, 0+3*4) = min(26,12) = 12
    //   dp[3] = min(dp[2]+w3*h2, dp[1]+w3*h1, dp[0]+w3*h0)
    //         = min(12+8*5, 8+8*6, 0+8*4) = min(52, 56, 32) = 32

    using SlopeDP::HullDeque;

    // Demonstrate the HullDeque with MIN mode
    HullDeque<0> hull;  // MIN
    hull.add(4, 0);     // line: y = 4*x + 0
    hull.add(6, 0);     // line: y = 6*x + 0  (dominates for large x)
    hull.add(5, 12);    // line: y = 5*x + 12

    cout << "Hull query at x=2: " << hull.query(2) << "\n";   // min(4*2,6*2,5*2+12)=min(8,12,22)=8
    cout << "Hull query at x=5: " << hull.query(5) << "\n";   // min(4*5,6*5,5*5+12)=min(20,30,37)=20
    cout << "Hull query at x=10: " << hull.query(10) << "\n"; // min(4*10,6*10,5*10+12)=min(40,60,62)=40

    // Demonstrate Land Acquisition
    vector<pair<ll,ll>> rects = {{2,4}, {3,6}, {5,3}, {8,5}};
    auto filtered = SlopeDP::removeDominated(rects);
    cout << "\nFiltered rectangles:\n";
    for (auto& r : filtered) cout << "  (" << r.first << ", " << r.second << ")\n";

    vector<ll> w, h;
    for (auto& r : filtered) { w.push_back(r.first); h.push_back(r.second); }
    cout << "Min area: " << SlopeDP::landAcquisition(w, h) << "\n";

    return 0;
}

#endif