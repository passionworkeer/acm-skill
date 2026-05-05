# 图论经典题解

> 题目特征 → 算法选择 → 核心思路

---

## 1. 最短路

### 单源非负权
**题目特征**：边权 ≥ 0，求 s → 所有点 / t 的最短路
**算法**：Dijkstra（堆优化）
**模板**：`dijkstra.hpp`
**核心**：`priority_queue<(dist,v)>`，O((n+m) log n)

### 含负权边（无负环）
**题目特征**：边权可负，无负环
**算法**：Bellman-Ford 或 SPFA
**模板**：`bellman-ford.hpp`
**核心**：n-1 轮松弛；SPFA = 队列优化的 BF，最坏 O(nm)

### 差分约束
**题目特征**：形如 `x_j - x_i <= w` 的不等式组
**算法**：SPFA 判负环 + 求最长路
**模板**：`spfa.hpp`（或 `graph-theory/spfa.hpp`）
**核心**：建超级源点，连 `S→i (0)`，`i→j (w)`，求最长路 = 解

### K 短路
**题目特征**：求第 K 短路径（可重复经过节点）
**算法**：A* + Dijkstra
**模板**：参考 `k-short-path.tex`（kirainmoe 仓库）
**核心**：用 dist[y] 做启发式函数，出队 K 次到 t 即为 K 短路

---

## 2. 连通性 / SCC / 双连通

### 求割点 / 桥
**题目特征**：删某点/边后图是否连通
**模板**：`bridges.hpp`, `graph-theory/two-edge-connected.hpp`
**核心**：Tarjan DFS，`dfn[u] < low[v]` → 边 (u,v) 是桥；桥的条件

### 强连通分量
**模板**：`tarjan-scc.hpp`（DFS 版本）或 `kosaraju.hpp`（两遍 DFS）
**核心**：Tarjan：维护 dfn/low/stack，low[u]==dfn[u] 则弹出栈顶得到 SCC

### 2-SAT
**题目特征**：布尔变量 + 蕴含关系，判断是否有解
**模板**：`two-sat.hpp`
**核心**：每个变量拆两点 x 和 ¬x，边 (x→y) = (¬x ∨ y)，缩点后判 x 和 ¬x 是否同属一 SCC

---

## 3. 匹配

### 二分图最大匹配（无权）
**算法**：Hopcroft-Karp（O(E√V)）
**模板**：`hopcroft-karp.hpp`

### 二分图最大权匹配
**算法**：KM 算法（O(n³)）
**模板**：`km.hpp`
**核心**：维护顶标，slack 优化，完整匹配存在当且仅当完美匹配

### 一般图最大匹配
**算法**：Edmonds Blossom（开花算法）
**模板**：`blossom.hpp`
**核心**：奇环缩花，复杂度 O(n³)，竞赛中常用 BFS 版

---

## 4. 网络流

### 最大流
**模板**：`dinic.hpp`（常用，O(E√V) 或 O(V²E)）
**核心**：BFS 建层图 → DFS 阻塞流 → 重复
**备用**：`push-relabel.hpp`（更快的 HLPP 实现）

### 最小费用最大流
**模板**：`mcmf.hpp`
**核心**：每次 SPFA / Dijkstra 找最小费用增广路（边权 = 费用），重复直到无增广路

### 循环流 / 有上下界流
**模板**：`dinic.hpp` + 有源汇的上下界可行流
**核心**：拆弧，下界变 0，上界 = ub-lb，超级源汇平衡

---

## 5. 树结构

### LCA
**模板**：`binary-lifting.hpp`（倍增），`euler-lca.hpp`（欧拉序 + RMQ）
**核心**：`binary lifting`: `anc[u][k] = anc[anc[u][k-1]][k-1]`，预处理 O(n log n)，查询 O(log n)

### Heavy-Light 剖分
**模板**：`heavy-light.hpp`
**核心**：剖分后任意路径 = O(log n) 条重链拼接，每条链用线段树维护

### 虚树
**模板**：`virtual-tree.hpp`
**核心**：给定 k 个关键点，建 O(k) 大小的虚树；先按 Euler 序排序，相邻关键点两两求 LCA

### 质心分解
**模板**：`centroid-decomp.hpp`
**核心**：递归去掉质心，在每层统计跨子树的贡献（分治），O(n log n)

### DSU on Tree（树上莫队）
**模板**：`dsu-on-tree.hpp`
**核心**：DSU 合并子树，统计子树答案，O(n log n) 离线处理子树询问

---

## 6. 欧拉回路 / 哈密顿

### 欧拉路/回路（判断 + 构造）
**模板**：`eulerian.hpp`（无向），`eulerian-path.hpp`（有向）
**核心**：所有点度数为偶数 ⇔ 欧拉回路；Hierholzer 算法 O(n+m)

---

## 7. 常用套路

| 场景 | 套路 |
|------|------|
| 图上 DP / 树形 DP | 先求 LCA 或重链剖分 |
| 多源最短路 | 超级源点连 0 边 |
| 负环检测 | SPFA 计数入队 > n 次 |
| 平面图最小割 | 转换为最短路（对偶图）|
| 树上路径加/求和 | HLD + Segtree |
| 删除某点后连通块数 | Tarjan 求割点 |
