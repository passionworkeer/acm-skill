/**
 * Half-Plane Intersection
 * Computes the intersection of a set of half-planes, each defined as the
 * set of points to the left of a directed line.
 *
 * Based on the O(n log n) sorting-by-angle algorithm using a deque.
 * Handles:
 *   - Parallel lines
 *   - Empty intersection
 *   - Convex degenerate cases (collinear edges, single point)
 *
 * Namespace: HalfPlane
 *
 * Key functions:
 *   getIntersection(a, b)  - compute intersection point of two lines
 *   onLeft(l, p)           - check if point p is strictly on the left of line l
 *   halfPlaneIntersection  - main routine, returns resulting polygon
 *
 * Usage:
 *   vector<Line> L = { ... };
 *   vector<Point> poly = HalfPlane::halfPlaneIntersection(L);
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace HalfPlane {

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}

    Point operator + (const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    Point operator - (const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    Point operator * (double k) const {
        return Point(x * k, y * k);
    }
    double dot(const Point& other) const {
        return x * other.x + y * other.y;
    }
    double cross(const Point& other) const {
        return x * other.y - y * other.x;
    }
    bool operator == (const Point& other) const {
        return fabs(x - other.x) < 1e-9 && fabs(y - other.y) < 1e-9;
    }
};

struct Line {
    Point p;    // a point on the line
    Point s;    // direction vector (from p)
    Line() {}
    Line(const Point& _p, const Point& _s) : p(_p), s(_s) {}

    // implicit normal (pointing to the left of s)
    Point normal() const {
        return Point(s.y, -s.x);
    }

    // Check if point q is on the left side of the directed line (p, p+s)
    // Returns true if strictly left, false if on or right.
    static bool onLeft(const Line& l, const Point& q) {
        return l.s.cross(q - l.p) > 1e-9;
    }
};

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------

// Compute intersection point of two lines a and b.
// Precondition: lines are not parallel.
inline Point getIntersection(const Line& a, const Line& b) {
    // Solve: a.p + t*a.s = b.p + u*b.s
    // (a.p - b.p) = u*b.s - t*a.s
    // Use cross product to solve for t and u.
    // Let d = a.s cross b.s  (non-zero when not parallel)
    double d = a.s.cross(b.s);
    // If d == 0, lines are parallel or coincident; caller must handle.
    // Use formula: p = a.p + ((b.p - a.p).cross(b.s) / d) * a.s
    double t = (b.p - a.p).cross(b.s) / d;
    return a.p + a.s * t;
}

// ---------------------------------------------------------------------------
// Main Algorithm
// ---------------------------------------------------------------------------

// Compare function for sorting lines by direction angle.
// Lines are sorted by the angle of their direction vector s.
// Primary key: atan2(s.y, s.x)  (range [-pi, pi])
// Secondary key: make left-of comparison consistent when angles equal.
struct LineCmp {
    bool operator()(const Line& a, const Line& b) const {
        double angA = atan2(a.s.y, a.s.x);
        double angB = atan2(b.s.y, b.s.x);
        if (fabs(angA - angB) > 1e-9)
            return angA < angB;
        // Same direction: the line with the more "left" point should come first.
        // That is, keep the more restrictive half-plane (p more in the left direction).
        return a.s.cross(b.p - a.p) > 1e-9;
    }
};

// Check if two lines are effectively parallel (direction vectors collinear)
inline bool parallel(const Line& a, const Line& b) {
    return fabs(a.s.cross(b.s)) < 1e-9;
}

// ---------------------------------------------------------------------------
// Half-Plane Intersection
// ---------------------------------------------------------------------------

// Returns the intersection polygon of all half-planes in L.
// Each half-plane is defined as the set of points strictly to the left of
// the directed line (p, p+s).
//
// Algorithm (O(n log n)):
//   1. Sort lines by direction angle.
//   2. Remove redundant lines (lines with same direction as the previous one).
//   3. Use a deque to maintain candidate lines:
//      - Add lines in sorted order.
//      - Maintain the front intersection point.
//      - If the last two lines and the new line make the front infeasible,
//        pop the front line.
//      - Similar logic for the back.
//   4. After processing all lines, the deque contains the lines forming the
//      intersection boundary.
//
// Returns an empty vector if the intersection is empty.
inline vector<Point> halfPlaneIntersection(vector<Line> L) {
    int n = (int)L.size();
    if (n == 0) return {};

    // 1. Sort by angle
    sort(L.begin(), L.end(), LineCmp());

    // 2. Remove duplicate-angle lines (keep only the most restrictive)
    vector<Line> filtered;
    for (int i = 0; i < n; ++i) {
        if (i + 1 < n && parallel(L[i], L[i + 1])) {
            // keep the one that cuts more: the one where the other line's point
            // lies on the right side of this line
            if (Line::onLeft(L[i], L[i + 1].p)) {
                // L[i] is more restrictive, discard L[i+1]
                continue;
            } else {
                // L[i+1] is more restrictive; replace L[i] with L[i+1]
                // (handled by discarding L[i] below)
            }
        }
        // For exact duplicates, skip second
        if (i > 0 && parallel(L[i], L[i - 1]) &&
            L[i].s.cross(L[i-1].p - L[i].p) < 1e-9) {
            // L[i] is covered by L[i-1]; skip
            continue;
        }
        filtered.push_back(L[i]);
    }
    L.swap(filtered);
    n = (int)L.size();
    if (n == 0) return {};

    // 3. Build deque of lines
    deque<Line> dq;
    deque<Point> pts;   // intersection points of adjacent lines in dq

    for (int i = 0; i < n; ++i) {
        // skip parallel consecutive lines (should not happen after filtering,
        // but guard anyway)
        if (i + 1 < n && parallel(L[i], L[i + 1])) continue;

        // Remove lines from the back that make the new line redundant
        while ((int)dq.size() >= 2) {
            Point inter = getIntersection(dq[dq.size() - 2], dq[dq.size() - 1]);
            if (Line::onLeft(L[i], inter)) {
                dq.pop_back();
                if (!pts.empty()) pts.pop_back();
            } else {
                break;
            }
        }

        // Remove lines from the front similarly
        while ((int)dq.size() >= 2) {
            Point inter = getIntersection(dq[0], dq[1]);
            if (Line::onLeft(L[i], inter)) {
                dq.pop_front();
                if (!pts.empty()) pts.pop_front();
            } else {
                break;
            }
        }

        if (!dq.empty()) {
            pts.push_back(getIntersection(dq.back(), L[i]));
        }
        dq.push_back(L[i]);
    }

    // 4. After all lines added, prune front and back again
    while ((int)dq.size() >= 3) {
        Point inter = getIntersection(dq[dq.size() - 2], dq[dq.size() - 1]);
        if (Line::onLeft(dq.back(), inter)) {
            dq.pop_back();
            pts.pop_back();
        } else {
            break;
        }
    }
    while ((int)dq.size() >= 3) {
        Point inter = getIntersection(dq[0], dq[1]);
        if (Line::onLeft(dq.back(), inter)) {
            dq.pop_front();
            pts.pop_front();
        } else {
            break;
        }
    }

    if ((int)dq.size() < 3) {
        // Intersection is a polygon with <= 2 edges (degenerate case)
        // Return the vertices stored in pts (deduplicated)
        vector<Point> result;
        for (const auto& p : pts) result.push_back(p);
        return result;
    }

    // Convert deque of lines to polygon vertices
    vector<Point> result;
    result.reserve(dq.size());
    for (size_t i = 0; i < dq.size(); ++i) {
        size_t j = (i + 1) % dq.size();
        result.push_back(getIntersection(dq[i], dq[j]));
    }
    return result;
}

// ---------------------------------------------------------------------------
// Convenience helpers
// ---------------------------------------------------------------------------

// Create a line from two points a and b (left side is the half-plane from a to b)
inline Line lineFromPoints(const Point& a, const Point& b) {
    return Line(a, b - a);
}

// Signed area (twice area) of polygon
inline double polygonArea2(const vector<Point>& poly) {
    double area2 = 0;
    int m = (int)poly.size();
    for (int i = 0; i < m; ++i) {
        const Point& p = poly[i];
        const Point& q = poly[(i + 1) % m];
        area2 += p.cross(q);
    }
    return area2;
}

// Check if intersection is empty (returned polygon is empty or area ~ 0)
inline bool isEmpty(const vector<Point>& poly) {
    return poly.empty() || fabs(polygonArea2(poly)) < 1e-9;
}

} // namespace HalfPlane
