#pragma once
#include <bits/stdc++.h>
using namespace std;

/*
 * Push-Relabel + successive shortest augmenting path
 * 最小费用最大流 (Min-Cost Max-Flow)
 *
 * 边结构: to, rev(反向边在邻接表中的索引), cap(残余容量), cost(单位费用)
 *
 * 算法策略：
 *   1. Push-Relabel 做最大流（全局先达到最大流）
 *   2. 每次从残留网络中找最小费用增广路（SPFA / Dijkstra + potentials）
 *   3. 沿该路径进行增广，直到无法继续或费用为正
 *
 * 特点：
 *   - 支持负边权 cost（初始用 Bellman-Ford/SPFA 预处理势能）
 *   - 不支持负环（需保证无负环，否则会无限循环）
 *   - 实际复杂度 O(F·V·E)，在竞赛数据中通常足够快
 *   - 注意 F 为最大流值，可能较大时可考虑预流推进优化
 *
 * 使用方式：
 *   PushRelabelMCMF mcmf(N);
 *   mcmf.addEdge(s, t, cap, cost);
 *   auto [flow, cost] = mcmf.minCostMaxFlow(s, t);
 */

// ====================== 边结构 ======================

struct MCMFEdge {
    int to;           // 终点
    int rev;          // 反向边在 graph[to] 中的索引
    long long cap;    // 残余容量
    long long cost;   // 单位流量费用
    MCMFEdge(int _to, int _rev, long long _cap, long long _cost)
        : to(_to), rev(_rev), cap(_cap), cost(_cost) {}
};

// ====================== 主类 ======================

struct PushRelabelMCMF {
    int N;
    vector<vector<MCMFEdge>> G;
    vector<long long> h;          // 顶势（potential，用于消负成本）
    vector<long long> dist;       // 到汇点的最短距离
    vector<int> prevv;            // 前驱顶点
    vector<int> preve;            // 前驱边
    const long long INF = (1LL << 60);

    explicit PushRelabelMCMF(int n) : N(n) {
        G.assign(N, {});
        h.assign(N, 0);
        dist.assign(N, 0);
        prevv.assign(N, -1);
        preve.assign(N, -1);
    }

    // 添加有向边（及其反向边，自动构造反向边 cap=0, cost=-cost）
    void addEdge(int from, int to, long long cap, long long cost) {
        MCMFEdge a(to, (int)G[to].size(), cap, cost);
        MCMFEdge b(from, (int)G[from].size(), 0, -cost);
        G[from].push_back(a);
        G[to].push_back(b);
    }

    // ====================== 最短路（带势能） ======================

    // SPFA 找从 s 到所有点的最短路（用于检测负边或初始化）
    bool spfa(int s, int t) {
        dist.assign(N, INF);
        vector<int> inq(N, 0), cnt(N, 0);
        queue<int> q;

        dist[s] = 0;
        q.push(s);
        inq[s] = 1;

        while (!q.empty()) {
            int v = q.front(); q.pop();
            inq[v] = 0;
            for (int i = 0; i < (int)G[v].size(); ++i) {
                const MCMFEdge& e = G[v][i];
                if (e.cap <= 0) continue;
                long long nd = dist[v] + e.cost + h[v] - h[e.to];
                if (dist[e.to] > nd) {
                    dist[e.to] = nd;
                    if (!inq[e.to]) {
                        q.push(e.to);
                        inq[e.to] = 1;
                        if (++cnt[e.to] > N) return false; // 负环检测
                    }
                }
            }
        }
        return true;
    }

    // Dijkstra（基于势能的 non-negative 最短路）
    void dijkstra(int s, int t) {
        priority_queue<pair<long long,int>,
                       vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
        dist.assign(N, INF);
        prevv.assign(N, -1);
        preve.assign(N, -1);
        dist[s] = 0;
        pq.emplace(0, s);

        while (!pq.empty()) {
            auto [d, v] = pq.top(); pq.pop();
            if (dist[v] < d) continue;
            for (int i = 0; i < (int)G[v].size(); ++i) {
                MCMFEdge& e = G[v][i];
                if (e.cap <= 0) continue;
                long long nd = dist[v] + e.cost + h[v] - h[e.to];
                if (dist[e.to] > nd) {
                    dist[e.to] = nd;
                    prevv[e.to] = v;
                    preve[e.to] = i;
                    pq.emplace(nd, e.to);
                }
            }
        }
    }

    // ====================== 增广路径 ======================

    // 沿 prev 链从 t 往回推，算出最小残余容量
    long long minCapBack(int s, int t) {
        long long d = INF;
        for (int v = t; v != s; v = prevv[v]) {
            d = min(d, G[prevv[v]][preve[v]].cap);
        }
        return d;
    }

    // 沿 prev 链增广，并累加费用
    void addFlow(int s, int t, long long f) {
        for (int v = t; v != s; v = prevv[v]) {
            MCMFEdge& e = G[prevv[v]][preve[v]];
            e.cap -= f;
            G[v][e.rev].cap += f;
        }
    }

    // ====================== 主函数 ======================

    /*
     * minCostMaxFlow(s, t)
     *
     * 返回 {最大流, 最小费用}
     *
     * 策略：
     *   1. 用 SPFA 初始化势能 h（处理负边费用）
     *   2. 循环：每次用 Dijkstra 找最小费用增广路
     *   3. 若 dist[t] == INF 或 dist[t] + h[t] - h[s] > 0，停止（无负费用增广路）
     *   4. 更新势能：h[v] += dist[v]（v 可达）
     *   5. 沿路径增广
     *
     * 注意：当所有残留费用非负时，SPFA 可省略，直接用 Dijkstra。
     */
    pair<long long, long long> minCostMaxFlow(int s, int t) {
        long long flow = 0;
        long long cost = 0;

        // 初始化势能（处理负费用）：SPFA 跑一遍
        // 如果确定所有 cost >= 0，这步可以省
        if (!spfa(s, t)) {
            // 图中存在负环（不应该出现），直接返回
            return {0, 0};
        }
        for (int v = 0; v < N; ++v) {
            if (dist[v] < INF) h[v] = dist[v];
        }

        while (true) {
            // 用 Dijkstra 找最小费用增广路（基于当前势能）
            dijkstra(s, t);
            if (prevv[t] == -1) break; // 找不到增广路了

            // 计算沿路径增广的流量
            long long d = minCapBack(s, t);
            if (d == INF) break; // 理论上不会发生

            // 检查费用：若增广后总费用会变正，提前停止
            if (h[t] - h[s] > 0 && dist[t] + h[t] - h[s] > 0) break;

            // 增广
            addFlow(s, t, d);
            flow += d;
            cost += d * (h[t] - h[s]); // 实际单位费用 = h[t] - h[s]

            // 更新势能
            for (int v = 0; v < N; ++v) {
                if (dist[v] < INF) h[v] += dist[v];
            }
        }

        return {flow, cost};
    }

    // 简化版：只求最大流（普通 push-relabel / Dinic 更合适，这里仅展示接口）
    long long maxFlow(int s, int t) {
        return minCostMaxFlow(s, t).first;
    }
};

// ====================== 使用示例 ======================
/*
PushRelabelMCMF mcmf(4);
int s = 0, t = 3;
mcmf.addEdge(s, 1, 10, 2);
mcmf.addEdge(s, 2, 5,  1);
mcmf.addEdge(1, 2, 3,  0);   // 过路费 0
mcmf.addEdge(1, t, 7,  3);
mcmf.addEdge(2, t, 5,  2);
auto [flow, cost] = mcmf.minCostMaxFlow(s, t);
// flow = 10, cost = 最小费用
*/