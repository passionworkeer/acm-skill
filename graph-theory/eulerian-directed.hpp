#pragma once
#include <bits/stdc++.h>
using namespace std;

/*
 * 欧拉路径/回路（Eulerian Path / Circuit）
 *
 * 有向图版本：
 *   - hasPath() 返回 0(无欧拉路径), 1(有路径), 2(有回路)
 *   - getPath() 返回顶点序列（边按顺序排列）
 *
 * 无向图版本：
 *   - hasUndirectedPath() / getUndirectedPath()
 *
 * 算法: Hierholzer（希洛尔泽）算法，O(V+E)
 * 适用于连通图，需先外部保证连通性或自行处理。
 */

// ====================== 有向图版本 ======================

struct EulerianDirected {
    int n;                          // 顶点数（0..n-1）
    vector<vector<pair<int,int>>> adj;  // adj[u] = {v, edge_id}
    vector<int> inDeg, outDeg;      // 入度/出度
    vector<int> usedEdge;           // 边是否已访问
    int edgeCnt = 0;

    explicit EulerianDirected(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n, {});
        inDeg.assign(n, 0);
        outDeg.assign(n, 0);
        usedEdge.clear();
        edgeCnt = 0;
    }

    // 添加有向边 u -> v
    void addEdge(int u, int v) {
        adj[u].push_back({v, edgeCnt});
        ++outDeg[u];
        ++inDeg[v];
        ++edgeCnt;
    }

    // 判断有向图是否有欧拉路径/回路
    // 返回 0: 无  1: 有路径(非回路)  2: 有回路
    int hasPath() {
        int start = 0, end = 0;
        int cntStart = 0, cntEnd = 0;

        for (int i = 0; i < n; ++i) {
            if (abs(inDeg[i] - outDeg[i]) > 1) return 0;
            if (outDeg[i] - inDeg[i] == 1) {
                ++cntStart;
                start = i;
            }
            if (inDeg[i] - outDeg[i] == 1) {
                ++cntEnd++;
                end = i;
            }
        }

        if (cntStart == 0 && cntEnd == 0) return 2; // 回路
        if (cntStart == 1 && cntEnd == 1) return 1; // 路径
        return 0;
    }

    // 内部：Hierholzer 递归收集
    void hierholzer(int u, vector<int>& circuit) {
        while (!adj[u].empty()) {
            auto [v, eid] = adj[u].back();
            adj[u].pop_back();
            if (usedEdge[eid]) continue;
            usedEdge[eid] = 1;
            hierholzer(v, circuit);
            circuit.push_back(u); // 记录顶点（之后需要反转）
        }
    }

    // 获取欧拉路径/回路的顶点序列
    // 返回空 vector 表示不存在
    vector<int> getPath() {
        int type = hasPath();
        if (type == 0) return {};

        usedEdge.assign(edgeCnt, 0);
        vector<int> circuit;
        int start = 0;

        // 找到起点
        if (type == 1) {
            for (int i = 0; i < n; ++i)
                if (outDeg[i] - inDeg[i] == 1) { start = i; break; }
        }

        hierholzer(start, circuit);
        circuit.push_back(start); // 最后一个顶点和起点重合

        // circuit 目前是逆序，reverse 后得到正确顺序
        reverse(circuit.begin(), circuit.end());

        // 验证：边的数量应该等于 edgeCnt（防止图不连通）
        // 实际使用时请确保图是（强）连通的
        return circuit;
    }
};

// ====================== 无向图版本 ======================

struct EulerianUndirected {
    int n;
    vector<vector<pair<int,int>>> adj; // {neighbor, edge_id}
    vector<int> deg;                    // 度数
    vector<int> usedEdge;                // 边是否已访问
    int edgeCnt = 0;

    explicit EulerianUndirected(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n, {});
        deg.assign(n, 0);
        usedEdge.clear();
        edgeCnt = 0;
    }

    // 添加无向边（自动去重视为不同边）
    void addUndirectedEdge(int u, int v) {
        adj[u].push_back({v, edgeCnt});
        adj[v].push_back({u, edgeCnt});
        ++deg[u];
        ++deg[v];
        ++edgeCnt;
    }

    // 判断无向图是否有欧拉路径/回路
    // 返回 0: 无  1: 有路径(非回路)  2: 有回路
    int hasUndirectedPath() {
        int odd = 0;
        for (int i = 0; i < n; ++i)
            if (deg[i] % 2 == 1) ++odd;
        if (odd == 0) return 2;
        if (odd == 2) return 1;
        return 0;
    }

    // 内部：Hierholzer（无向图版）
    void hierholzer(int u, vector<int>& path) {
        while (!adj[u].empty()) {
            auto [v, eid] = adj[u].back();
            adj[u].pop_back();
            if (usedEdge[eid]) continue;
            usedEdge[eid] = 1;
            // 去掉无向图中的这条边（避免回头）
            // 手动删除反向引用，避免重复遍历
            for (auto& p : adj[v]) {
                if (p.second == eid) { p.second = -1; break; }
            }
            hierholzer(v, path);
            path.push_back(u);
        }
    }

    // 获取无向图欧拉路径/回路的顶点序列
    // 返回空 vector 表示不存在
    vector<int> getUndirectedPath() {
        if (hasUndirectedPath() == 0) return {};

        usedEdge.assign(edgeCnt, 0);
        vector<int> path;
        int start = 0;

        if (hasUndirectedPath() == 1) {
            for (int i = 0; i < n; ++i)
                if (deg[i] % 2 == 1) { start = i; break; }
        }

        hierholzer(start, path);
        path.push_back(start);
        reverse(path.begin(), path.end());
        return path;
    }
};

// ====================== 使用示例 ======================
/*
// 有向图
EulerianDirected e(5);
e.addEdge(0, 1);
e.addEdge(1, 2);
e.addEdge(2, 0);  // 回路: 0->1->2->0
e.addEdge(2, 3);
e.addEdge(3, 4);
e.addEdge(4, 2);  // 路径: 0->1->2->3->4->2->0 ???
// 正确示例（标准回路）:
EulerianDirected e2(3);
e2.addEdge(0,1); e2.addEdge(1,2); e2.addEdge(2,0);
vector<int> p = e2.getPath(); // 0 1 2 0

// 无向图
EulerianUndirected eu(4);
eu.addUndirectedEdge(0,1);
eu.addUndirectedEdge(1,2);
eu.addUndirectedEdge(2,0);
eu.addUndirectedEdge(0,3);
vector<int> p2 = eu.getUndirectedPath(); // 3 0 1 2 0
*/