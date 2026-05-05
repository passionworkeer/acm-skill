/**
 * Link-Cut Tree (LCT)
 * Implements dynamic tree operations on a forest of rooted trees.
 * Based on the Sleator-Tarjan link-cut tree data structure.
 *
 * Supported operations:
 *   access(x)       - expose node x (makes path from root to x a preferred path)
 *   makeRoot(x)     - make x the root of its tree
 *   link(x, y)      - connect two trees by linking x as a child of y
 *   cut(x, y)       - disconnect the edge between x and y
 *   findRoot(x)     - find the root of the tree containing x
 *   split(x, y)     - make path x-y a separate splay tree (accessible via y)
 *   queryPath(x, y) - query an aggregate (e.g. sum) on the path x-y
 *
 * Usage:
 *   LCT lct(n);          // create n nodes (1..n)
 *   lct.link(u, v);      // link u under v (u becomes child)
 *   lct.cut(u, v);       // cut edge u-v
 *   lct.split(u, v);     // now v is the root of the splay for path u-v
 *   int ans = lct.queryPath(u, v);
 *
 * Template parameter T: value type (e.g. int, long long).
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

// ---------------------------------------------------------------------------
// Link-Cut Tree
// ---------------------------------------------------------------------------
template <typename T>
class LinkCutTree {
    struct Node {
        Node *ch[2];   // left and right child
        Node *fa;      // parent pointer
        T val;         // node value
        T sum;         // aggregate of the subtree rooted at this node
        bool rev;       // lazy reverse flag

        int dir() const {
            if (!fa) return -1;
            return fa->ch[0] == this ? 0 : (fa->ch[1] == this ? 1 : -1);
        }

        bool isRoot() const {
            return fa == nullptr || (fa->ch[0] != this && fa->ch[1] != this);
        }

        Node(T v = T())
            : fa(nullptr), val(v), sum(v), rev(false) {
            ch[0] = ch[1] = nullptr;
        }
    };

    int n;
    vector<Node> nodes;

    // ---- internal helpers ----------------------------------------------------
    void push(Node *x) {
        if (!x || !x->rev) return;
        swap(x->ch[0], x->ch[1]);
        if (x->ch[0]) x->ch[0]->rev ^= true;
        if (x->ch[1]) x->ch[1]->rev ^= true;
        x->rev = false;
    }

    void pushUp(Node *x) {
        if (!x) return;
        x->sum = x->val;
        if (x->ch[0]) x->sum = x->sum + x->ch[0]->sum;
        if (x->ch[1]) x->sum = x->sum + x->ch[1]->sum;
    }

    void rotate(Node *x) {
        Node *p = x->fa;
        Node *g = p->fa;
        if (p->isRoot()) {
            // nothing
        } else {
            // inherit direction from p
        }

        int d = p->ch[0] == x ? 0 : 1;   // which side is x
        Node *y = x->ch[d ^ 1];

        // rotation
        if (!p->isRoot()) {
            if (g->ch[0] == p) g->ch[0] = x;
            else if (g->ch[1] == p) g->ch[1] = x;
        }
        x->fa = g;

        x->ch[d ^ 1] = p;
        p->fa = x;

        p->ch[d] = y;
        if (y) y->fa = p;

        pushUp(p);
        pushUp(x);
    }

    void pushPath(Node *x) {
        if (!x->isRoot()) pushPath(x->fa);
        push(x);
    }

    void splay(Node *x) {
        pushPath(x);
        while (!x->isRoot()) {
            Node *p = x->fa;
            Node *g = p->fa;
            if (!p->isRoot()) {
                if ((p->ch[0] == x) ^ (p->ch[0] == p)) {
                    rotate(x);
                } else {
                    rotate(p);
                }
            }
            rotate(x);
        }
    }

    // -------------------------------------------------------------------------
    // public interface
    // -------------------------------------------------------------------------
public:
    explicit LinkCutTree(int n = 0, T defaultVal = T()) : n(n), nodes(n + 1) {
        for (int i = 1; i <= n; ++i) nodes[i] = Node(defaultVal);
    }

    // set value of node id (1-indexed)
    void setVal(int id, T v) {
        nodes[id].val = v;
        pushUp(&nodes[id]);
    }

    // expose node x: make the path from root of its tree to x a preferred path
    // After access(x), x becomes the rightmost node on the preferred path
    void access(int x) {
        Node *last = nullptr;
        for (Node *cur = &nodes[x]; cur; cur = cur->fa) {
            splay(cur);
            cur->ch[1] = last;
            pushUp(cur);
            last = cur;
        }
        splay(&nodes[x]);   // bring x to the top of the splay tree
    }

    // make x the root of its tree
    void makeRoot(int x) {
        access(x);
        nodes[x].rev ^= true;   // reverse the whole tree
    }

    // link x as a child of y  (x must be in a different tree from y)
    void link(int x, int y) {
        makeRoot(x);          // x becomes root
        nodes[x].fa = &nodes[y]; // directly attach x under y
    }

    // cut the edge between x and y
    void cut(int x, int y) {
        makeRoot(x);          // make x the root
        access(y);            // access y
        // now y is the root of the splay tree; x should be y's left child
        // and y should have no right child (only the path x-y remains)
        Node *ny = &nodes[y];
        if (ny->ch[0] == &nodes[x]) {
            ny->ch[0]->fa = nullptr;
            ny->ch[0] = nullptr;
            pushUp(ny);
        }
    }

    // find the root of the tree containing x (with path compression)
    int findRoot(int x) {
        access(x);
        Node *cur = &nodes[x];
        push(cur);               // propagate lazy reverse
        while (cur->ch[0]) {
            cur = cur->ch[0];
            push(cur);
        }
        splay(cur);
        return int(cur - &nodes[0]);   // index = pointer offset
    }

    // make path x-y a separate splay tree; after split(x,y), node y is the root
    // of the splay representing that path
    void split(int x, int y) {
        makeRoot(x);
        access(y);              // y becomes the root of the splay for path x-y
    }

    // query aggregate (sum) on the path x-y
    // After split(x, y), nodes[y].sum holds the result.
    T queryPath(int x, int y) {
        split(x, y);
        return nodes[y].sum;
    }
};
