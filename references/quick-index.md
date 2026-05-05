# 算法快速索引

> 算法名 → 文件路径 → 时间复杂度 → 适用场景

---

## 数据结构

| 算法 | 文件 | 时间复杂度 | 空间 | 适用场景 |
|------|------|-----------|------|----------|
| 树状数组 | `data-structures/fenwick.hpp` | O(log n) | O(n) | 单点更新 + 前缀和 |
| 树状数组（区间更新）| `data-structures/fenwick-range.hpp` | O(log n) | O(n) | 区间加 + 区间求和 |
| 线段树 | `data-structures/segtree.hpp` | O(log n) | O(n) | 区间操作（加/乘/最值）|
| 懒标记线段树 | `data-structures/segtree-lazy.hpp` | O(log n) | O(n) | 区间赋值+求和，懒传播 |
| 可持久化线段树 | `data-structures/segtree-persistent.hpp` | O(log n) per version | O(n log n) | 历史版本查询 |
| 稀疏表 | `data-structures/sparse-table.hpp` | O(1) 查询 / O(n log n) 建 | O(n log n) | RMQ（静态区间最值）|
| 并查集 | `data-structures/union-find.hpp` | α(n)（几乎 O(1)）| O(n) | 连通性、集合合并 |
| Treap（分裂合并）| `data-structures/treap.hpp` | O(log n) | O(n) | 有序集合，第k小，排名 |
| Treap（旋转）| `data-structures/treap-rotate.hpp` | O(log n) | O(n) | 同上，另一种实现 |
| 配对堆 | `data-structures/pairing-heap.hpp` | O(log n) | O(n) | 斐波那契堆替代品（竞赛）|
| 莫队算法 | `data-structures/mo-algorithm.hpp` | O((n+q)√n) | O(n) | 离线区间询问 |
| 单调队列 | `data-structures/sliding-window.hpp` | O(n) | O(k) | 滑动窗口最值 |
| 动态凸包 | `data-structures/dynamic-hull.hpp` | O(log n) 插入+查询 | O(n) | 动态切线 |
| Link-Cut Tree | `data-structures/lct.hpp` | O(log n) | O(n) | 动态树：连边/断边，路径查询 |
| KD-Tree | `data-structures/kd-tree.hpp` | O(log n) ~ O(n) | O(n) | 动态2D范围和，K近点 |
| 单调DP凸包 | `data-structures/monotonic-dp-hull.hpp` | O(n) | O(n) | DP斜率优化（单调版）|

---

## 图论

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| Dijkstra | `graph-theory/dijkstra.hpp` | O((n+m) log n) | 非负权最短路 |
| Bellman-Ford | `graph-theory/bellman-ford.hpp` | O(nm) | 含负边，负环检测 |
| SPFA | （同上，队列优化）| O(km) 期望 | 差分约束，最长路 |
| 二进制跳表 LCA | `graph-theory/binary-lifting.hpp` | O(n log n) 建，O(log n) 查 | 树上路径询问 |
| 欧拉序 LCA | `graph-theory/euler-lca.hpp` | O(n) 建，O(1) 查 | RMQ版 LCA |
| 重链剖分 HLD | `graph-theory/heavy-light.hpp` | O(log² n) 路径 | 树上路径操作 |
| 质心分解 | `graph-theory/centroid-decomp.hpp` | O(n log n) | 树上分治，路径统计 |
| 虚树 | `graph-theory/virtual-tree.hpp` | O(k log k) | 关键点子树/路径问题 |
| DSU on Tree | `graph-theory/dsu-on-tree.hpp` | O(n log n) | 子树询问离线 |
| Tarjan SCC | `graph-theory/tarjan-scc.hpp` | O(n+m) | 强连通分量 |
| Kosaraju | `graph-theory/kosaraju.hpp` | O(n+m) | SCC（两遍DFS）|
| 割点/桥 | `graph-theory/bridges.hpp` | O(n+m) | 双连通分量 |
| 双边连通分量 | `graph-theory/two-edge-connected.hpp` | O(n+m) | 边双连通 |
| 拓扑排序 | `graph-theory/topo-sort.hpp` | O(n+m) | DAG 排序 |
| 欧拉回路 | `graph-theory/eulerian.hpp` | O(n+m) | 欧拉路径构造 |
| Dinic 最大流 | `graph-theory/dinic.hpp` | O(E√V)（整数）| 最大流 |
| Push-Relabel | `graph-theory/push-relabel.hpp` | O(V³)（最坏）| 大流量图 |
| MCMF | `graph-theory/mcmf.hpp` | O(F·E·logV) | 最小费用最大流 |
| Hopcroft-Karp | `graph-theory/hopcroft-karp.hpp` | O(E√V) | 二分图最大匹配 |
| Hungarian KM | `graph-theory/km.hpp` | O(n³) | 二分图最大权匹配 |
| Blossom 开花 | `graph-theory/blossom.hpp` | O(n³) | 一般图最大匹配 |
| 2-SAT | `graph-theory/two-sat.hpp` | O(n+m) | 布尔约束满足 |

---

## 字符串

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| KMP | `strings/kmp.hpp` | O(n+m) | 单模式匹配 |
| Z 函数 | `strings/z-function.hpp` | O(n+m) | 匹配+相似度 |
| Manacher | `strings/manacher.hpp` | O(n) | 最长回文子串 |
| AC 自动机 | `strings/ac-automaton.hpp` | O(n+∑|P|) 建，O(|T|) 跑 | 多模式匹配 |
| 字典树 | `strings/trie.hpp` | O(∑|s|) 建 | 前缀匹配，前缀和 |
| 后缀数组 | `strings/suffix-array.hpp` | O(n log n)（倍增）| LCP，排名 |
| 后缀自动机 | `strings/suffix-automaton.hpp` | O(n) 建 | 所有子串，查询 |
| 扩展后缀自动机 | `strings/ex-sam.hpp` | O(∑|s|) 建 | 多串子串统计 |
| 回文树 | `strings/palindromic-tree.hpp` | O(n) | 回文统计，计数 |
| Lyndon 分解 | `strings/lyndon.hpp` | O(n) | 最小表示，Lyndon 串 |
| 字符串哈希 | `strings/polyhash.hpp` | O(n) 建，O(1) 哈希 | 子串比较 |

---

## 数学

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 快速幂 | `misc/fast-pow.hpp` | O(log k) | 模幂运算 |
| 扩展欧几里得 | `math/ex-gcd.hpp` | O(log min(a,b)) | 乘法逆元，CRT |
| 中国剩余定理 | `math/crt.hpp` | O(k log m) | 模数互质合并 |
| Miller-Rabin | `math/miller-rabin.hpp` | O(k log³n) | 质数判定 |
| Pollard-Rho | `math/pollard-rho.hpp` | O(n^1/4) 期望 | 大整数分解 |
| BSGS | `math/bsgs.hpp` | O(√p log p) | 离散对数 |
| Lucas 定理 | `math/lucas.hpp` | O(p + log_p n) | 大组合数 mod p |
| NTT | `math/ntt.hpp` | O(n log n) | 多项式乘法（整数）|
| 任意模数 NTT | `math/ntt-arbitrary.hpp` | O(n log n) | 任意模数卷积 |
| FFT | `math/fft.hpp` | O(n log n) | 浮点多项式乘法 |
| Min25 筛 | `math/min25.hpp` | O(n^(2/3)) | 积性函数前缀和 |
| 杜教筛 | `math/dujiao.hpp` | O(n^(2/3)) | φ/μ 前缀和 |
| 卡特兰数 | `math/catalan.hpp` | O(1) per n | 括号/出栈序列 |
| 斯特林数 | `math/stirling.hpp` | O(n²) 预计算 | 集合/圆排列 |
| Polya 计数 | `math/polya.hpp` | O(n log n) | 染色方案计数 |
| 高斯消元 | `math/gauss.hpp` | O(n³) | 线性方程组，行列式 |
| 拉格朗日插值 | `math/lagrange.hpp` | O(n²) | 已知前 n 项求任意项 |
| 线性规划/单纯形 | `math/simplex.hpp` | O(n³ m) | 最优解 |

---

## 计算几何

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 凸包（Andrew）| `geometry/convex-hull.hpp` | O(n log n) | 点集最小包围 |
| 旋转卡壳 | `geometry/rotating-calipers.hpp` | O(n)（在凸包上）| 最远点对，直径 |
| 半平面交 | `geometry/half-plane.hpp` | O(n log n) | 线性规划，交集 |
| 扫描线 | `geometry/scanning-line.hpp` | O(n log n) | 矩形并面积 |
| 3D 几何 | `geometry/geometry-3d.hpp` | - | 三维叉积、体积 |

---

## 动态规划

| 优化类型 | 文件 | 复杂度 | 条件 |
|---------|------|--------|------|
| 区间 DP | `dynamic-programming/interval-dp.hpp` | O(n³) | 区间合并 |
| 数位 DP | `dynamic-programming/digit-dp.hpp` | O(n·states) | 数字/计数 |
| 状态压缩 DP | `dynamic-programming/state-compress.hpp` | O(n·2^n) | n≤20 集合 |
| 背包 DP | `dynamic-programming/knapsack.hpp` | O(n·W) | 组合优化 |
| 斜率优化 DP | `dynamic-programming/slope-optim.hpp` | O(n) 或 O(n log n) | cost 凸性 |
| CDQ 分治优化 | `misc/divide-conquer-dp.hpp`（参考 kirainmoe）| O(n log n) | 斜率优化（非单调）|

---

## 位运算

| 操作 | 文件 | 说明 |
|------|------|------|
| XOR 线性基 | `bit/xor-basis.hpp` | 求最大异或和，子集异或 |
| 枚举子集 | `bit/iterate-submasks.hpp` | `for(sub=mask; sub; sub=(sub-1)&mask)` |
| 枚举超集 | `bit/iterate-supermasks.hpp` | 从子集扩展到全集 |
