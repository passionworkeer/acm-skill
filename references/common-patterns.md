# 竞赛常用解题模式

> 这些模式不是具体算法，而是竞赛中反复出现的高频套路。

---

## 1. 倍增法（Binary Lifting）

**核心思想**：预处理 `f[i][j]` = 从 i 跳 2^j 步的结果，查询 O(log n)。

**典型应用**：
- LCA（二进制跳表）
- 二进制分组：把一个数拆成 2^j 的组合
- RMQ（稀疏表本质也是倍增）
- 快速幂（最简单的倍增）

```cpp
// 预处理：O(n log n)
for (int j = 1; j < LOG; j++)
    for (int i = 1; i <= n; i++)
        anc[i][j] = anc[anc[i][j-1]][j-1];

// 查询：从 u 跳到深度 d
int kthAncestor(int u, int k) {
    for (int j = 0; j < LOG; j++)
        if (k & (1<<j)) u = anc[u][j];
    return u;
}
```

---

## 2. 分治（CDQ）

**核心思想**：把问题按某一维排序，递归处理左右两部分，统计跨越中线的贡献。

**典型应用**：
- CDQ 分治优化 DP（当斜率优化不适用时）
- 统计三维偏序（按 x 排序，CDQ 按 y，树状数组按 z）
- 归并排序求逆序对（最简单的 CDQ）

```cpp
void cdq(int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    cdq(l, mid); cdq(mid+1, r);
    // 处理跨越 mid 的贡献：用两个指针合并（或 BIT 按 z）
    merge(l, mid, mid+1, r);
}
```

---

## 3. 离线处理（离线 + 逆序）

**核心思想**：将问题"在线操作"转为"离线逆序处理"，利用可逆操作。

**典型应用**：
- 并查集按秩合并 + 撤销 → 离线连通性问题
- 线段树从右往左做 → 避免删除操作
- 莫队：把左右指针移动操作均摊 O(1)

**并查集撤销模板**：
```cpp
stack<pair<int*, int>> stk;
void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return;
    if (sz[a] < sz[b]) swap(a, b);
    stk.push({&sz[a], sz[a]}); // 记录旧值
    stk.push({&fa[b], fa[b]});
    fa[b] = a; sz[a] += sz[b];
}
void undo() {
    while (!stk.empty()) {
        *stk.top().first = stk.top().second;
        stk.pop();
    }
}
```

---

## 4. 预处理 + 离线查询

**核心思想**：先 O(n log n) 预处理所有可能询问的答案，再 O(1) 响应每个查询。

**典型应用**：
- Sparse Table：预处理任意区间最小值，O(1) 查询
- RMQ（LCA）：Euler Tour + Sparse Table
- 前缀和 / 差分：一次预处理，多次 O(1) 查询
- 质数前缀和：Min25 / 杜教筛

---

## 5. 01 字典树（异或最值）

**核心思想**：把每个数的二进制从高位到低位插入字典树，贪心选使答案最大的分支。

**典型应用**：
- 最大异或和（从根贪心选相反位）
- 第 K 小异或和
- 子集异或和

```cpp
int queryMaxXor(int x) {
    int cur = 0;
    for (int i = MAXBIT; i >= 0; i--) {
        int b = (x>>i) & 1;
        if (tr[cur][b^1]) cur = tr[cur][b^1];
        else cur = tr[cur][b];
    }
    return x ^ val[cur];
}
```

---

## 6. 二进制枚举子集（Submask）

**核心思想**：利用 `(sub-1) & mask` 遍历所有子集。

```cpp
// 遍历 mask 的所有非空子集
for (int sub = mask; sub; sub = (sub-1) & mask) {
    // sub 是 mask 的一个非空子集
}

// 遍历所有子集（包括空集）
for (int sub = mask; ; sub = (sub-1) & mask) {
    // 处理 sub
    if (sub == 0) break;
}
```

**典型应用**：
- 状态压缩 DP 的子集转移
- 容斥原理枚举子集
- 子集求和（前缀和的逆运算）

---

## 7. 区间贡献分治（线段树分治）

**核心思想**：把"某个点影响一个区间"的全部操作，用线段树区间分解，均摊 O(log n)。

**典型应用**：
- 动态凸包（二分答案 + 线段树分治）
- 动态加边/删边（只加边则用时间线段树）
- 可撤销贪心

```cpp
void addEdge(int l, int r, Edge e) { // 边 e 在 [l, r] 时间有效
    segUpdate(1, 1, T, l, r, e);
}
```

---

## 8. 二分答案 + 可行性检验

**核心思想**：把"最优化"问题转为"判定"问题，二分最优值。

```cpp
int lo = 0, hi = INF;
while (lo < hi) {
    int mid = (lo + hi + 1) >> 1;
    if (feasible(mid)) lo = mid; // mid 可行，尝试更大
    else hi = mid - 1;
}
```

**典型场景**：
- 最大化最小值（最小生成树 → 最大边最小）
- 判定图是否有某性质
- 二分图最大匹配判定（加边 + Dinic 验证）

---

## 9. 分数规划 / 凸优化

**核心思想**：`max/min (a/b)` → 二分 λ，检查 `max (a - λ·b) ≥ 0`。

**典型应用**：
- 最大平均密度子图
- 最优比率生成树
- 比率背包

---

## 10. 贪心 + 堆（堆优化贪心）

**核心思想**：把 O(n²) 的全局最优贪心，通过"当前最优候选"数据结构加速到 O(n log n)。

**典型应用**：
- Dijkstra（堆维护当前最短距离）
- Kruskal（排序 + 并查集）
- 滑动窗口最值（单调队列）
- Huffman 编码（合并果子）

---

## 模式选择指南

| 问题特征 | 首先考虑 |
|---------|---------|
| 有单调性（递增/递减）| 单调队列、Two Pointers |
| 可离线、逆操作 | 逆序处理、并查集撤销 |
| 二分可行 | 二分答案 |
| 高维 DP、转移有偏序 | CDQ 分治 |
| 需要历史版本 | 可持久化数据结构 |
| 在线 + 区间 | 莫队（离线）或线段树 |
| 路径/子树询问 | 树链剖分/DSU on Tree |
| 异或/AND/OR 最值 | 二进制 Trie |
| 递推+大指数 | 矩阵乘法快速幂 |
