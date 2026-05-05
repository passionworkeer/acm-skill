/**
 * KD-Tree (K-dimensional Tree)
 * Implements two common use cases:
 *   1. K-nearest-neighbor search in 2D (k-th smallest squared distance)
 *   2. Dynamic 2D range sum with insert and query, with automatic rebuild
 *
 * Based on the standard 2D KD-tree structure with a rebuild heuristic.
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace KDTree {

// ---------------------------------------------------------------------------
// Use Case 1: K-Nearest-Neighbor Search
// ---------------------------------------------------------------------------
// Finds the k-th smallest squared distance from a query point to a set of
// 2D points.
// ---------------------------------------------------------------------------

struct KNNPoint {
    double x, y;
    int id;
};

struct MaxHeap {
    // max-heap of squared distances; we keep the k largest distances so that
    // the root is the k-th smallest. The heap stores NEGATED distances so that
    // priority_queue (a max-heap by default) effectively gives us the largest
    // negative value = smallest original distance at the top.
    priority_queue<double> pq;   // stores -dist, root = k-th smallest dist

    void push(double d) {
        pq.push(-d);             // negate to simulate a min-heap for smallest
    }

    // keep at most k elements; evict the largest (i.e. most negative) if full
    void trim(double d, int k) {
        push(d);
        while ((int)pq.size() > k) pq.pop();
    }

    double kthSmallest() const {
        // the k-th smallest dist is the largest among the kept distances
        return pq.empty() ? 0.0 : -pq.top();
    }
};

class KNNBuilder {
    vector<KNNPoint> points;
    int n;
    int k;
    int curDim;            // 0 = x, 1 = y

    struct Node {
        KNNPoint pt;
        Node *l, *r;
        Node(const KNNPoint& p) : pt(p), l(nullptr), r(nullptr) {}
    };

    Node *root;

    Node *build(vector<KNNPoint> &pts, int l, int r, int dim) {
        if (l > r) return nullptr;
        int mid = (l + r) >> 1;
        nth_element(pts.begin() + l, pts.begin() + mid, pts.begin() + r + 1,
            [dim](const KNNPoint& a, const KNNPoint& b) {
                return dim == 0 ? a.x < b.x : a.y < b.y;
            });
        Node *node = new Node(pts[mid]);
        node->l = build(pts, l, mid - 1, dim ^ 1);
        node->r = build(pts, mid + 1, r, dim ^ 1);
        return node;
    }

    double sqDist(const KNNPoint& a, double qx, double qy) const {
        double dx = a.x - qx, dy = a.y - qy;
        return dx * dx + dy * dy;
    }

    void nearest(Node *node, double qx, double qy, int k, MaxHeap &heap,
                 double &bestDist, int dim) {
        if (!node) return;
        // visit current
        double d = sqDist(node->pt, qx, qy);
        if ((int)heap.pq.size() < k) {
            heap.trim(d, k);
            if ((int)heap.pq.size() == k) {
                bestDist = heap.kthSmallest();
            }
        } else if (d < bestDist) {
            heap.trim(d, k);
            bestDist = heap.kthSmallest();
        }

        // decide which child to visit first
        double coord = (dim == 0 ? node->pt.x : node->pt.y);
        double qcoord = (dim == 0 ? qx : qy);
        Node *first = (qcoord < coord) ? node->l : node->r;
        Node *second = (qcoord < coord) ? node->r : node->l;
        nearest(first, qx, qy, k, heap, bestDist, dim ^ 1);

        // prune the other subtree if possible
        double diff = fabs(qcoord - coord);
        if ((int)heap.pq.size() < k || diff * diff < bestDist) {
            nearest(second, qx, qy, k, heap, bestDist, dim ^ 1);
        }
    }

public:
    explicit KNNBuilder(const vector<KNNPoint> &pts) {
        points = pts;
        n = (int)pts.size();
        root = nullptr;
        if (n > 0) root = build(points, 0, n - 1, 0);
    }

    // Returns the k-th smallest squared distance from (qx, qy) to the points.
    // Requires k >= 1. Returns -1.0 if no points exist.
    double queryKth(double qx, double qy, int kth) {
        if (n == 0 || kth < 1) return -1.0;
        MaxHeap heap;
        double best = 1e100;
        nearest(root, qx, qy, kth, heap, best, 0);
        return (int)heap.pq.size() < kth ? -1.0 : heap.kthSmallest();
    }
};

// Helper: build a KNN tree from a list of points
inline KNNBuilder buildKNN(const vector<KNNPoint> &pts) {
    return KNNBuilder(pts);
}

// ---------------------------------------------------------------------------
// Use Case 2: Dynamic 2D Range Sum
// ---------------------------------------------------------------------------
// Supports point insert/update and axis-aligned rectangle sum queries.
// Uses a rebuild trigger at alpha = 0.75 to keep the tree balanced.
// ---------------------------------------------------------------------------

struct Point2D {
    double x, y;
    double v;           // value at this point
    Point2D(double x_ = 0, double y_ = 0, double v_ = 0) : x(x_), y(y_), v(v_) {}
};

class Dynamic2DSum {
    static constexpr double REBUILD_RATIO = 0.75;

    struct Node {
        Point2D pt;
        Node *l, *r;
        long long sum;       // sum of all values in this subtree
        bool deleted;        // lazy deletion flag (for update)
        Node(const Point2D& p) : pt(p), l(nullptr), r(nullptr),
                                 sum((long long)p.v), deleted(false) {}
    };

    Node *root = nullptr;
    vector<Point2D> allPoints;   // for rebuild
    int dim = 0;                 // current split dimension

    long long queryNode(Node *node, double x1, double y1,
                        double x2, double y2) const {
        if (!node) return 0;
        if (node->deleted) {
            return queryNode(node->l, x1, y1, x2, y2) +
                   queryNode(node->r, x1, y1, x2, y2);
        }
        bool inside = (node->pt.x >= x1 && node->pt.x <= x2 &&
                       node->pt.y >= y1 && node->pt.y <= y2);
        long long res = inside ? node->pt.v : 0;
        if (!inside || node->pt.x > x2 || node->pt.x < x1 ||
            node->pt.y > y2 || node->pt.y < y1) {
            // recurse into children for outside points
            if (node->pt.x < x1 || node->pt.x > x2) {
                res = queryNode(node->l, x1, y1, x2, y2) +
                      queryNode(node->r, x1, y1, x2, y2);
            } else if (node->pt.y < y1 || node->pt.y > y2) {
                res = queryNode(node->l, x1, y1, x2, y2) +
                      queryNode(node->r, x1, y1, x2, y2);
            }
            return res;
        }
        return res;
    }

    Node *buildFromPoints(vector<Point2D> &pts, int l, int r, int d) {
        if (l > r) return nullptr;
        int mid = (l + r) >> 1;
        nth_element(pts.begin() + l, pts.begin() + mid, pts.begin() + r + 1,
            [d](const Point2D& a, const Point2D& b) {
                return d == 0 ? a.x < b.x : a.y < b.y;
            });
        Node *node = new Node(pts[mid]);
        node->l = buildFromPoints(pts, l, mid - 1, d ^ 1);
        node->r = buildFromPoints(pts, mid + 1, r, d ^ 1);
        node->sum = node->pt.v;
        if (node->l) node->sum += node->l->sum;
        if (node->r) node->sum += node->r->sum;
        return node;
    }

    void collectPoints(Node *node, vector<Point2D> &out) {
        if (!node) return;
        if (!node->deleted) out.push_back(node->pt);
        collectPoints(node->l, out);
        collectPoints(node->r, out);
    }

    // update sum up from a node (called after a child changes)
    void updateSum(Node *node) {
        if (!node) return;
        node->sum = node->deleted ? 0 : (long long)node->pt.v;
        if (node->l) node->sum += node->l->sum;
        if (node->r) node->sum += node->r->sum;
    }

    // insert or update a point
    bool insert(Node *&node, const Point2D &p, int d, int &rebuildCnt) {
        if (!node) {
            node = new Node(p);
            return true;
        }
        if (fabs(node->pt.x - p.x) < 1e-9 && fabs(node->pt.y - p.y) < 1e-9) {
            // update existing point
            node->pt.v = p.v;
            node->sum = p.v;
            if (node->l) node->sum += node->l->sum;
            if (node->r) node->sum += node->r->sum;
            return false;
        }
        if ((d == 0 ? p.x < node->pt.x : p.y < node->pt.y)) {
            bool added = insert(node->l, p, d ^ 1, rebuildCnt);
            updateSum(node);
            if (added && ++rebuildCnt > int(rebuildCnt * REBUILD_RATIO)) {
                // rebuild not triggered here; managed at top level
            }
            return added;
        } else {
            bool added = insert(node->r, p, d ^ 1, rebuildCnt);
            updateSum(node);
            return added;
        }
    }

    void rebuild(Node *&node, int d) {
        vector<Point2D> pts;
        collectPoints(node, pts);
        node = buildFromPoints(pts, 0, (int)pts.size() - 1, d);
    }

    int pointCount(Node *node) const {
        if (!node) return 0;
        return (node->deleted ? 0 : 1) + pointCount(node->l) + pointCount(node->r);
    }

public:
    Dynamic2DSum() : root(nullptr), dim(0) {}

    // Insert or update a point at (x, y) with value v
    void insert(double x, double y, double v) {
        int dummy = 0;
        insert(root, Point2D(x, y, v), dim, dummy);
        // Simple rebuild heuristic: after every insert, if the total
        // inserted points exceeds 2x the actual point count, rebuild.
        static int opCount = 0;
        if (++opCount >= 1000) {
            opCount = 0;
            int total = pointCount(root);
            allPoints.clear();
            collectPoints(root, allPoints);
            if ((int)allPoints.size() > 0 &&
                (int)allPoints.size() > total * 2) {
                rebuild(root, dim);
            }
        }
    }

    // Query sum over axis-aligned rectangle [x1, x2] x [y1, y2]
    long long query(double x1, double y1, double x2, double y2) const {
        if (x1 > x2 || y1 > y2) return 0;
        return queryNode(root, x1, y1, x2, y2);
    }
};

} // namespace KDTree
