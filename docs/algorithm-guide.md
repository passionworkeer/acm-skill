# 算法分类指南

本指南按类别整理 ACM Champion 库中的核心算法，帮助快速定位所需工具。

---

## 1. 图论 (Graph Theory)

**核心问题：** 最短路、网络流、匹配、树上特殊操作

| 头文件 | 算法 | 适用场景 |
|--------|------|----------|
| `graph/dijkstra.hpp` | 堆优化 Dijkstra | 单源最短路，非负权边 |
| `graph/spfa.hpp` | SPFA | 含负权的最短路，检测负环 |
| `graph/floyd.hpp` | Floyd-Warshall | 全源最短路，传递闭包 |
| `graph/mcmf.hpp` | 最小费用最大流 | 费用最小的最大流 |
| `graph/dinic.hpp` | Dinic | 最大流，复杂度 O(E√V) |
| `graph/hungarian.hpp` | Hungarian | 二分图最大权匹配 |
| `graph/hld.hpp` | 轻重链剖分 | 路径/子树查询，O(log²N) |
| `graph/lct.hpp` | Link-Cut Tree | 动态树路径修改/查询 |
| `graph/virtual_tree.hpp` | 虚树 | 多关键点树上问题压缩 |
| `graph/centroid.hpp` | 重心分解 | 树上路径统计，O(N log N) |

---

## 2. 字符串 (String)

**核心问题：** 模式匹配、子串结构、回文检测

| 头文件 | 算法 | 适用场景 |
|--------|------|----------|
| `string/kmp.hpp` | KMP | 单模式匹配，O(N+M) |
| `string/ac_automaton.hpp` | AC 自动机 | 多模式匹配，字典树+KMP |
| `string/suffix_array.hpp` | 后缀数组 | 字符串排序，LCP 查询 |
| `string/sam.hpp` | 后缀自动机 | 所有子串枚举，最长公共子串 |
| `string/palindrome.hpp` | 回文树 (Eertree) | 回文子串计数，本质不同回文 |
| `string/manacher.hpp` | Manacher | 全回文半径，O(N) |

---

## 3. 数学 (Math)

**核心问题：** 大数质因数、多项式卷积、组合计数

### 数论

| 头文件 | 算法 | 适用场景 |
|--------|------|----------|
| `math/miller_rabin.hpp` | Miller-Rabin | 素数判定，大数快速检测 |
| `math/pollard_rho.hpp` | Pollard-Rho | 大整数分解，递归质因数 |
| `math/min25.hpp` | Min25 筛 | 积性函数前缀和 |
| `math/exgcd.hpp` | 扩展欧几里得 | 线性同余方程，逆元 |

### 卷积

| 头文件 | 算法 | 适用场景 |
|--------|------|----------|
| `math/ntt.hpp` | NTT | 模质数卷积，FFT 替代 |
| `math/fft.hpp` | FFT | 浮点卷积，支持复数 |
| `math/convolution.hpp` | 任意模数卷积 | 三模数 NTT 合并 |

### 组合数学

| 头文件 | 公式 | 适用场景 |
|--------|------|----------|
| `math/catalan.hpp` | Catalan 数 | 合法括号、树形计数 |
| `math/stirling.hpp` | Stirling 数 | 集合划分，第二类求幂 |
| `math/polya.hpp` | Polya 计数 | 群论染色问题 |

---

## 4. 数据结构 (Data Structures)

**核心问题：** 区间操作、区间排序、滑动最值

| 头文件 | 结构 | 适用场景 |
|--------|------|----------|
| `ds/bit.hpp` | 树状数组 | 单点修改/区间和，O(logN) |
| `ds/segtree.hpp` | 线段树 | 区间修改/查询，懒标记 |
| `ds/treap.hpp` | 随机平衡树 | 有序集合，插入/删除/排名 |
| `ds/mo.hpp` | Mo's Algorithm | 离线区间查询，O((N+Q)√N) |
| `ds/monotonic_queue.hpp` | 单调队列 | 滑动窗口最值，O(N) |
| `ds/sparse_table.hpp` | 稀疏表 | RMQ，O(1) 查询（仅静态） |
| `ds/dsu.hpp` | 并查集 | 连通性，Kruskal，Tarjan |

---

## 5. 计算几何 (Computational Geometry)

**核心问题：** 点集凸包、距离计算、区域划分

| 头文件 | 算法 | 适用场景 |
|--------|------|----------|
| `geometry/convex_hull.hpp` | Andrew / Graham | 点集凸包构建，O(N log N) |
| `geometry/rotating_calipers.hpp` | 旋转卡尺 | 凸包直径、最远点对 |
| `geometry/half_plane.hpp` | 半平面交 | 线性规划、多边形交集 |
| `geometry/scanline.hpp` | 扫描线 | 矩形面积并、点定位 |
| `geometry/point.hpp` | 点/向量基础 | 叉积、面积、有向距离 |
| `geometry/polygon.hpp` | 多边形操作 | 面积、内点判定 |

---

## 6. 动态规划 (Dynamic Programming)

**核心问题：** 状态爆炸压缩、特殊转移顺序

| 头文件 | 类型 | 适用场景 |
|--------|------|----------|
| `dp/interval.hpp` | 区间 DP | 合并、括号、区间划分 |
| `dp/digit.hpp` | 数位 DP | 按位统计，满足条件的数 |
| `dp/state_compress.hpp` | 状态压缩 DP | 集合遍历，TSP、铺砖 |
| `dp/slope.hpp` | 斜率优化 DP | 决策单调，CDQ 分治 |
| `dp/quadrangle.hpp` | 四边形优化 DP | 区间 DP 加速，Knuth 优化 |
| `dp/tree.hpp` | 树上 DP | 树形依赖，子树聚合 |

---

## 快速查询

**不知道用什么算法？按问题特征查找：**

- 树上路径查询 → `HLD` 或 `LCT`
- 字符串多模式匹配 → `AC 自动机`
- 大数质因数分解 → `Pollard-Rho`
- 区间询问离线 → `Mo's Algorithm`
- 凸多边形问题 → `旋转卡尺`
- 满足条件的数字计数 → `数位 DP`
- 负权最短路 → `SPFA`（注意卡限）
- 最大流/费用流 → `Dinic` / `MCMF`

---

> 本库遵循 OI/ICPC 风格，接口尽量保持一致。详见各头文件内注释。
