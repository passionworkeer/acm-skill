# ACM Champion — API 速查手册

---

## 1. Fenwick 树（点更新 + 前缀和）

**文件**: `data-structures/fenwick.hpp`

**用途**: 高效维护动态数组的区间前缀和，支持 O(log n) 单点修改和前缀查询。

**API**:

```cpp
struct Fenwick {
    Fenwick(int n);                        // 构造，n 个元素
    void add(int idx, T val);              // idx += val (1-indexed)
    T sum(int idx);                        // 前缀和 [1, idx]
    T rangeSum(int l, int r);              // 区间和 [l, r]
};
```

**复杂度**: 构造 O(n)，单次 add/sum O(log n)。

---

## 2. Lazy Segment Tree（区间加 + 区间和）

**文件**: `data-structures/segtree-lazy.hpp`

**用途**: 懒标记线段树，支持区间加、区间乘、区间求和等操作的合并。

**API**:

```cpp
struct SegTree {
    SegTree(int n);                        // 初始化
    void rangeAdd(int l, int r, T val);    // [l, r] += val
    T querySum(int l, int r);              // 查询区间和
};
```

**复杂度**: 建树 O(n)，单次区间操作 O(log n)。

---

## 3. Dijkstra（单源最短路）

**文件**: `graph-theory/dijkstra.hpp`

**用途**: 非负权图上求单源最短路，使用优先队列优化。

**API**:

```cpp
vector<T> dijkstra(int src, const vector<vector<pair<int,T>>>& g);
// 返回从 src 到各点的最短距离，无法到达为 INF
```

**复杂度**: O((n + m) log n)。

---

## 4. Dinic（最大流）

**文件**: `graph-theory/dinic.hpp`

**用途**: 求网络最大流，支持整数容量（int64 推荐）。

**API**:

```cpp
struct Dinic {
    Dinic(int n);                          // n 个点（0-indexed）
    void addEdge(int u, int v, T cap);     // 加有向边
    T maxFlow(int s, int t);               // s->t 最大流
};
```

**复杂度**: O(E sqrt(V))（单位容量），一般 O(E V^2)，实际极快。

---

## 5. Hopcroft-Karp（二分图最大匹配）

**文件**: `graph-theory/hopcroft-karp.hpp`

**用途**: 求二分图最大匹配（一边 n1 点，一边 n2 点，边集 G）。

**API**:

```cpp
int hopcroftKarp(const vector<vector<int>>& g, int n1, int n2);
// g[u] = 邻接的右侧节点列表，返回最大匹配数
```

**复杂度**: O(E sqrt(V))。

---

## 6. Heavy-Light Decomposition（树链剖分）

**文件**: `graph-theory/heavy-light.hpp`

**用途**: 将树拆为重链，支持树上路径的 O(log^2 n) 查询（求和、最大等）。

**API**:

```cpp
struct HLD {
    HLD(const vector<vector<int>>& tree);
    void pathQuery(int u, int v, const SegTree& seg, Fn fn);
    // 支持：点更新 + 路径聚合（需配合线段树）
};
```

**复杂度**: 预处理 O(n)，单次路径查询 O(log^2 n)。

---

## 7. KMP（单模式串匹配）

**文件**: `strings/kmp.hpp`

**用途**: 在文本中查找模式串出现位置，O(n + m)。

**API**:

```cpp
vector<int> prefixFunction(const string& s);           // 前缀函数
vector<int> kmpSearch(const string& text,
                      const string& pattern);           // 返回所有匹配下标
```

**复杂度**: O(n + m)。

---

## 8. Suffix Automaton（后缀自动机）

**文件**: `strings/suffix-automaton.hpp`

**用途**: 线性构建字符串所有子串结构，支持 O(1) 子串查询、最长公共子串等。

**API**:

```cpp
struct SuffixAutomaton {
    void build(const string& s);            // O(n) 建图
    int longestCommonSubstring(const string& t);
    // 扩展接口：子串出现次数、不同子串数量等
};
```

**复杂度**: 建图 O(n)，查询 O(1) ~ O(n)。

---

## 9. NTT（数论变换）

**文件**: `math/ntt.hpp`

**用途**: 快速多项式乘法（替代 FFT），在模 p = 998244353 下工作。

**API**:

```cpp
void ntt(vector<int>& a, bool invert);     // in-place NTT
vector<int> multiply(const vector<int>& a,
                      const vector<int>& b);  // 返回 a * b
```

**复杂度**: O(n log n)。

---

## 10. Slope Optimization（斜率优化 DP）

**文件**: `dynamic-programming/slope-optim.hpp`

**用途**: 将形如 `dp[i] = min/max{ dp[j] + C(j) }` 的 DP 优化至均摊 O(1)，其中 C(j) 满足凸包/单调性。

**API**:

```cpp
// 模板方法，需要自行实现：
// - calc(j, i)  // 计算 dp[j] + cost(j, i)
// - slope(a, b) // 两点的斜率，用于二分
// - maintain(): 维护下凸/上凸包
```

**复杂度**: 单次决策均摊 O(1)，整体 O(n)。

---

> 以上为最常用的 10 个模块。如需详细实现或示例，请参考各 `test/` 目录下的单元测试。
