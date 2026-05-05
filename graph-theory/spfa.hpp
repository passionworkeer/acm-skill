#pragma once

/**
 * SPFA (Shortest Path Faster Algorithm)
 *
 * Queue-optimized Bellman-Ford. Average O(m), worst O(nm).
 * Works with negative edges but detects negative cycles.
 *
 * Usage:
 *   SPFA spfa(n);
 *   for each edge (u,v,w): spfa.add(u, v, w);
 *   auto dist = spfa.shortest(1);  // from node 1
 *   bool hasNegCycle = spfa.hasNegativeCycle();
 *
 * For difference constraints (x_j - x_i <= w):
 *   Add edge i -> j with weight w, add super source S -> all with 0.
 *   Then find longest path from S. If negative cycle -> infeasible.
 */

#include <bits/stdc++.h>
using namespace std;

struct SPFA {
    struct Edge {
        int to;
        long long w;
        int next;
    };

    int n;
    vector<Edge> e;
    vector<int> head;
    vector<long long> dist;
    vector<int> inq;
    vector<int> cnt;  // times a node is relaxed (for negative cycle detection)

    SPFA(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        head.assign(n + 1, -1);
        e.clear();
        dist.assign(n + 1, 0);
        inq.assign(n + 1, 0);
        cnt.assign(n + 1, 0);
    }

    void add(int u, int v, long long w) {
        e.push_back({v, w, head[u]});
        head[u] = (int)e.size() - 1;
    }

    // Returns distance vector. If hasNegativeCycle(), some dist = -INF.
    vector<long long> shortest(int s) {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        dist[s] = 0;

        queue<int> q;
        q.push(s);
        inq[s] = 1;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = 0;

            for (int i = head[u]; i != -1; i = e[i].next) {
                int v = e[i].to;
                long long w = e[i].w;
                if (dist[u] != LLONG_MAX && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    cnt[v]++;
                    if (cnt[v] > n) {
                        // negative cycle reachable from s
                        return dist;
                    }
                    if (!inq[v]) {
                        q.push(v);
                        inq[v] = 1;
                    }
                }
            }
        }
        return dist;
    }

    // Detect if there's a negative cycle (call after shortest())
    // Note: run from every node or add super source for global detection
    bool hasNegativeCycle(int s = 1) {
        // Run SPFA; if any node is relaxed > n times, negative cycle exists
        fill(cnt.begin(), cnt.end(), 0);
        queue<int> q;
        vector<bool> vis(n + 1, false);

        // Relax all edges n times to propagate negative cycle effects
        for (int i = 1; i <= n; i++) {
            for (int u = 1; u <= n; u++) {
                if (dist[u] == LLONG_MAX) continue;
                for (int i_e = head[u]; i_e != -1; i_e = e[i_e].next) {
                    int v = e[i_e].to;
                    long long w = e[i_e].w;
                    if (dist[u] + w < dist[v]) {
                        dist[v] = -LLONG_MAX;  // mark as reachable via neg cycle
                    }
                }
            }
        }
        // Simplified: just check if dist[v] == -LLONG_MAX after running
        for (int v = 1; v <= n; v++) {
            if (dist[v] == -LLONG_MAX) return true;
        }
        return false;
    }

    // Standard Bellman-Ford style negative cycle detection
    bool hasNegCycleBF(int s) {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        dist[s] = 0;
        // Relax all edges n-1 times
        for (int i = 0; i < n - 1; i++) {
            bool updated = false;
            for (int u = 1; u <= n; u++) {
                if (dist[u] == LLONG_MAX) continue;
                for (int i_e = head[u]; i_e != -1; i_e = e[i_e].next) {
                    int v = e[i_e].to;
                    long long w = e[i_e].w;
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        updated = true;
                    }
                }
            }
            if (!updated) break;
        }
        // n-th relaxation: if any edge can be relaxed, negative cycle exists
        for (int u = 1; u <= n; u++) {
            if (dist[u] == LLONG_MAX) continue;
            for (int i_e = head[u]; i_e != -1; i_e = e[i_e].next) {
                int v = e[i_e].to;
                long long w = e[i_e].w;
                if (dist[u] + w < dist[v]) {
                    return true;  // negative cycle
                }
            }
        }
        return false;
    }
};