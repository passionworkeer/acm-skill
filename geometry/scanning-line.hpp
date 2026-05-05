#pragma once

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// Scanning Line / Line Sweep (扫描线)
// Used for counting union area of axis-aligned rectangles
// and other 2D geometry problems.
//
// Strategy:
//   1. Each rectangle generates two events: entering (add +1)
//      and leaving (add -1) at its x-coordinate.
//   2. Sort events by x-coordinate.
//   3. Sweep a vertical line left-to-right, maintaining a
//      segment tree over the y-coordinate interval tree.
//   4. When x jumps, add (delta_x * covered_y_length) to area.
//   5. Support lazy propagation for range updates on y.
//
// Features:
//   - Works for integer and floating-point coordinates
//   - Configurable coordinate type via template parameter T
//   - Segment tree with lazy propagation on y-axis
//   - Handles up to ~100,000 events efficiently O(N log M)
// ------------------------------------------------------------
namespace ScanningLine {

// ------------------------------------------------------------
// Axis-aligned rectangle
// (x1, y1) = lower-left,  (x2, y2) = upper-right
// Convention: x1 < x2, y1 < y2
// ------------------------------------------------------------
template <typename T = long long>
struct Rect {
    T x1, y1, x2, y2;
    Rect() : x1(0), y1(0), x2(0), y2(0) {}
    Rect(T _x1, T _y1, T _x2, T _y2)
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

    T width()  const { return x2 - x1; }
    T height() const { return y2 - y1; }
    T area()   const { return width() * height(); }
    bool valid() const { return x1 < x2 && y1 < y2; }
};

// ------------------------------------------------------------
// Event for sweep line
// at x = x
//   y-range [y1, y2) gets += coverDelta (+1 for enter, -1 for exit)
// ------------------------------------------------------------
template <typename T = long long>
struct Event {
    T x;
    T y1, y2;
    int coverDelta;  // +1 = rectangle entering, -1 = rectangle leaving

    Event() : x(0), y1(0), y2(0), coverDelta(0) {}

    Event(T _x, T _y1, T _y2, int _delta)
        : x(_x), y1(_y1), y2(_y2), coverDelta(_delta) {}

    bool operator<(const Event& other) const {
        return x < other.x;
    }
};

// ------------------------------------------------------------
// Segment Tree on y-coordinates
// Each node represents a y-interval [l, r) in terms of indices
// into the discretized y-coordinates array.
//
// Fields per node:
//   cnt  : how many y-intervals are fully covered (>0 means area contributes)
//   len  : total y-length where cnt > 0 in this node's interval
//   lazy : pending cover increment to propagate to children
//
// Coordinate compression is used: we collect all unique y-coordinates,
// sort them, and use their indices as the segment tree leaves.
// ------------------------------------------------------------
template <typename T = long long>
class SegTree {
public:
    static const int MAXN = 200000;  // max number of y-points

    struct Node {
        int cnt;      // cover count
        T len;        // covered length (total y-length with cnt > 0)
        T lazy;       // lazy propagation value
        Node() : cnt(0), len(0), lazy(0) {}
    };

    vector<Node> tree;
    vector<T> ys;       // discrete y-coordinates
    int n;             // number of leaves = ys.size() - 1

    explicit SegTree(const vector<T>& _ys) {
        ys = _ys;
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        n = (int)ys.size() - 1;
        if (n <= 0) n = 1;
        tree.assign(4 * max(1, n) + 10, Node());
    }

    // Helper: total length of interval [l, r) in y-coord array
    T intervalLen(int l, int r) const {
        if (l >= r || l >= (int)ys.size() || r >= (int)ys.size()) return T(0);
        return ys[r] - ys[l];
    }

    // Push lazy value to children
    void push(int node) {
        if (tree[node].lazy != 0) {
            T delta = tree[node].lazy;
            tree[node << 1].cnt += delta;
            tree[node << 1 | 1].cnt += delta;
            apply(node << 1, delta);
            apply(node << 1 | 1, delta);
            tree[node].lazy = 0;
        }
    }

    // Apply a delta to node's count, recalculate len
    void apply(int node, int delta) {
        // nothing needed beyond cnt; len recalculated in pull
    }

    void pull(int node, int l, int r) {
        if (tree[node].cnt > 0) {
            // fully covered
            tree[node].len = intervalLen(l, r);
        } else if (l + 1 == r) {
            // leaf, uncovered
            tree[node].len = 0;
        } else {
            // partially covered via children
            tree[node].len = tree[node << 1].len + tree[node << 1 | 1].len;
        }
    }

    // Range update: add delta to cover count on interval [L, R)
    void update(int node, int l, int r, int L, int R, int delta) {
        if (L >= r || R <= l) return;  // no overlap
        if (L <= l && r <= R) {
            tree[node].cnt += delta;
            pull(node, l, r);
            return;
        }
        push(node);
        int mid = (l + r) >> 1;
        update(node << 1, l, mid, L, R, delta);
        update(node << 1 | 1, mid, r, L, R, delta);
        pull(node, l, r);
    }

    // Public wrapper
    void update(int L, int R, int delta) {
        if (n <= 0 || L >= R) return;
        update(1, 0, n, L, R, delta);
    }

    T coveredLength() const {
        if (tree.empty()) return T(0);
        return tree[1].len;
    }

    // Query total covered length (same as coveredLength)
    T query() const { return coveredLength(); }
};

// ------------------------------------------------------------
// Core scanline algorithm
// rects[]: input axis-aligned rectangles
// Returns the total union area of all rectangles.
// Handles overlapping and partially overlapping rectangles.
//
// Steps:
//   1. Build events from each rectangle:
//      - Event at x1: y1..y2, delta = +1 (rectangle starts)
//      - Event at x2: y1..y2, delta = -1 (rectangle ends)
//   2. Collect and discretize all y-coordinates
//   3. Sort events by x
//   4. Sweep: for each consecutive x-pair, add
//      (x[i+1] - x[i]) * segTree.coveredLength()
//
// Time: O((N + M) log M) where N = number of rectangles
//       and M = number of unique y-coordinates
// Space: O(N + M)
// ------------------------------------------------------------
template <typename T = long long>
T scanlineCount(vector<Rect<T>> rects) {
    if (rects.empty()) return T(0);

    vector<Event<T>> events;
    vector<T> ys;

    for (const auto& r : rects) {
        if (!r.valid()) continue;
        events.emplace_back(r.x1, r.y1, r.y2, +1);
        events.emplace_back(r.x2, r.y1, r.y2, -1);
        ys.push_back(r.y1);
        ys.push_back(r.y2);
    }

    if (events.empty()) return T(0);

    sort(events.begin(), events.end());

    // Discretize y-coordinates
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    // Build segment tree on y-intervals
    SegTree<T> seg(ys);

    T area = T(0);
    T prevX = events[0].x;

    for (const auto& ev : events) {
        // Before processing event at x=ev.x, the sweep line is at prevX
        T deltaX = ev.x - prevX;
        if (deltaX > 0) {
            area += deltaX * seg.coveredLength();
        }
        prevX = ev.x;

        // Find y-indices for this event's y-range
        int y1i = (int)(lower_bound(ys.begin(), ys.end(), ev.y1) - ys.begin());
        int y2i = (int)(lower_bound(ys.begin(), ys.end(), ev.y2) - ys.begin());
        if (y1i < y2i) {
            seg.update(y1i, y2i, ev.coverDelta);
        }
    }

    return area;
}

// ------------------------------------------------------------
// Overload: raw event-based scanline (more flexible)
// Use this when you need to pre-process events yourself
// or when rectangles come from a different source.
// ------------------------------------------------------------
template <typename T = long long>
T scanlineEvents(vector<Event<T>> events, const vector<T>& ys) {
    if (events.empty()) return T(0);
    sort(events.begin(), events.end());
    SegTree<T> seg(ys);
    T area = T(0);
    T prevX = events[0].x;
    for (const auto& ev : events) {
        T deltaX = ev.x - prevX;
        if (deltaX > 0) area += deltaX * seg.coveredLength();
        prevX = ev.x;
        int y1i = (int)(lower_bound(ys.begin(), ys.end(), ev.y1) - ys.begin());
        int y2i = (int)(lower_bound(ys.begin(), ys.end(), ev.y2) - ys.begin());
        if (y1i < y2i) seg.update(y1i, y2i, ev.coverDelta);
    }
    return area;
}

// ------------------------------------------------------------
// Counting number of intersecting rectangle pairs
// (using the same sweep line infrastructure)
// For each event, maintain active rectangle count.
// ------------------------------------------------------------
template <typename T = long long>
int countIntersections(const vector<Rect<T>>& rects) {
    if (rects.empty()) return 0;
    struct XEvent {
        T x;
        int type;   // +1 = enter, -1 = leave
        int y1, y2;
    };
    vector<XEvent> events;
    vector<T> ys;
    for (const auto& r : rects) {
        if (!r.valid()) continue;
        ys.push_back(r.y1);
        ys.push_back(r.y2);
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    for (const auto& r : rects) {
        if (!r.valid()) continue;
        int y1i = lower_bound(ys.begin(), ys.end(), r.y1) - ys.begin();
        int y2i = lower_bound(ys.begin(), ys.end(), r.y2) - ys.begin();
        events.push_back({r.x1, +1, y1i, y2i});
        events.push_back({r.x2, -1, y1i, y2i});
    }
    sort(events.begin(), events.end(),
         [](const XEvent& a, const XEvent& b) {
             if (a.x != b.x) return a.x < b.x;
             return a.type > b.type;  // -1 before +1 at same x (enter first)
         });
    SegTree<T> seg(ys);
    long long intersections = 0;
    T prevX = events[0].x;
    for (const auto& ev : events) {
        // At current x, before processing ev:
        // intersections += C(active, 2)
        long long active = seg.tree[1].cnt;
        if (active >= 2) intersections += active * (active - 1) / 2;
        int y1i = ev.y1, y2i = ev.y2;
        if (y1i < y2i) seg.update(y1i, y2i, ev.type);
        prevX = ev.x;
    }
    return (int)intersections;
}

// ------------------------------------------------------------
// Example / test harness
// ------------------------------------------------------------
#ifdef SCANNING_LINE_MAIN
int main() {
    // Example 1: union area of 3 rectangles
    vector<Rect<int>> rects = {
        {0, 0, 4, 4},    // [0,4] x [0,4]
        {2, 2, 6, 6},    // overlaps first rect
        {5, 1, 9, 3}    // separate-ish
    };
    cout << "Union area of 3 rectangles: " << scanlineCount(rects) << endl;
    // Expected: rect1=16, rect2=16, rect3=8; overlap=4 -> 16+16+8-4=36

    // Example 2: floating-point rectangles
    vector<Rect<double>> rectsF = {
        {0.0, 0.0, 1.5, 2.0},
        {1.0, 1.0, 3.0, 3.0}
    };
    // Note: for double use appropriate T
    // (scanlineCount works for any comparable T)
    cout << "Union area (float): " << scanlineCount(rectsF) << endl;

    // Example 3: count intersections
    vector<Rect<int>> rects2 = {
        {0,0,5,5}, {2,2,7,7}, {1,3,6,8}, {3,0,8,4}
    };
    cout << "Intersecting pairs: " << countIntersections(rects2) << endl;

    // Example 4: large test with random rectangles
    cout << "\nStress test (1000 random rectangles)..." << endl;
    vector<Rect<int>> big;
    srand(42);
    for (int i = 0; i < 1000; ++i) {
        int x1 = rand() % 10000;
        int y1 = rand() % 10000;
        int x2 = x1 + rand() % 100 + 1;
        int y2 = y1 + rand() % 100 + 1;
        big.emplace_back(x1, y1, x2, y2);
    }
    clock_t start = clock();
    long long area = scanlineCount(big);
    clock_t end = clock();
    cout << "Union area: " << area << endl;
    cout << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}
#endif
}  // namespace ScanningLine
