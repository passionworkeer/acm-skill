# 算法快速索引

> 算法名 → 文件路径 → 时间复杂度 → 空间复杂度 → 适用场景
>
> 共 107 个文件，分为 7 个大类。

---

## 高频必备（Top 15）

| 优先级 | 文件 | 一句话说明 |
|--------|------|-----------|
| ★★★ | `graph-theory/dijkstra.hpp` | 最短路，竞赛出现频率最高的算法 |
| ★★★ | `graph-theory/dinic.hpp` | 最大流，模型建图后几乎无敌 |
| ★★★ | `data-structures/segtree-lazy.hpp` | 区间操作万能数据结构 |
| ★★★ | `data-structures/fenwick.hpp` | 树状数组，单点+前缀和极简实现 |
| ★★★ | `strings/kmp.hpp` | 单模式串匹配，O(n+m) |
| ★★★ | `strings/suffix-automaton.hpp` | 后缀自动机，字符串子串问题终版 |
| ★★★ | `math/ntt.hpp` | 多项式乘法，组合数学神器 |
| ★★★ | `math/fast-pow.hpp` | 快速幂，数论基础工具 |
| ★★☆ | `data-structures/union-find.hpp` | 并查集，连通性判定 |
| ★★☆ | `graph-theory/binary-lifting.hpp` | 二进制跳表，LCA + K 祖先 |
| ★★☆ | `data-structures/mo-algorithm.hpp` | 莫队，离线区间询问 |
| ★★☆ | `math/min25.hpp` | Min25 筛，积性函数前缀和 |
| ★★☆ | `dynamic-programming/slope-optim.hpp` | DP 斜率优化，决策单调性 |
| ★★☆ | `strings/manacher.hpp` | 最长回文子串，O(n) |
| ★★☆ | `math/ex-gcd.hpp` | 扩展欧几里得，逆元/CRT 基础 |

---

## 数据结构（Data Structures）— 22 个文件

| 算法 | 文件 | 时间复杂度 | 空间 | 适用场景 |
|------|------|-----------|------|----------|
| 树状数组 | `data-structures/fenwick.hpp` | O(log n) | O(n) | 单点更新 + 前缀和查询 |
| 树状数组（区间） | `data-structures/fenwick-range.hpp` | O(log n) | O(n) | 区间加 + 区间求和 |
| 线段树 | `data-structures/segtree.hpp` | O(log n) | O(n) | 区间操作（加/乘/最值/自定义）|
| 懒标记线段树 | `data-structures/segtree-lazy.hpp` | O(log n) | O(n) | 区间赋值+求和，懒传播 |
| 可持久化线段树 | `data-structures/segtree-persistent.hpp` | O(log n)/操作 | O(n log n) | 历史版本查询（函数式线段树）|
| 稀疏表 | `data-structures/sparse-table.hpp` | O(1) 查询 / O(n log n) 建 | O(n log n) | 静态 RMQ（不可修改）|
| 稀疏表 2D | `data-structures/sparse-table-2d.hpp` | O(1) 查询 | O(nm log n log m) | 静态二维 RMQ |
| 并查集 | `data-structures/union-find.hpp` | α(n)（均摊 O(1)）| O(n) | 连通分量、集合合并 |
| Treap（分裂合并）| `data-structures/treap.hpp` | O(log n) | O(n) | 有序集合、第 k 小、排名 |
| Treap（旋转）| `data-structures/treap-rotate.hpp` | O(log n) | O(n) | 同上，旋转版实现 |
| Treap（集合版）| `data-structures/treap-set.hpp` | O(log n) | O(n) | 去重集合，替代 std::set |
| Treap（分裂版）| `data-structures/treap-split.hpp` | O(log n) | O(n) | 按关键字分裂/合并 |
| 配对堆 | `data-structures/pairing-heap.hpp` | O(log n) | O(n) | 斐波那契堆的竞赛替代品 |
| 最小堆 | `data-structures/min-heap.hpp` | O(log n) | O(n) | 纯最小优先队列 |
| Link-Cut Tree | `data-structures/lct.hpp` | O(log n) | O(n) | 动态树：连边/断边，路径操作 |
| KD-Tree | `data-structures/kd-tree.hpp` | O(log n)~O(n) | O(n) | 动态二维范围和、K 近邻点 |
| 莫队算法 | `data-structures/mo-algorithm.hpp` | O((n+q)√n) | O(n) | 离线区间询问 |
| 单调队列 | `data-structures/sliding-window.hpp` | O(n) | O(k) | 滑动窗口最值 |
| 单调 DP 凸包 | `data-structures/monotonic-dp-hull.hpp` | O(n) | O(n) | DP 斜率优化（单调版）|
| 动态凸包 | `data-structures/dynamic-hull.hpp` | O(log n) 插入+查询 | O(n) | 动态维护凸包，切线查询 |
| 二维差分数组 | `data-structures/2d-diff-array.hpp` | O(1) 区间更新 | O(nm) | 二维矩形加减 |
| 二维前缀和 | `data-structures/2d-pref-sum.hpp` | O(1) 查询 | O(nm) | 二维区域求和 |

---

## 图论（Graph Theory）— 28 个文件

### 最短路

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| Dijkstra | `graph-theory/dijkstra.hpp` | O((n+m) log n) | 单源非负权最短路（最常用）|
| Bellman-Ford | `graph-theory/bellman-ford.hpp` | O(nm) | 含负边单源最短路、负环检测 |

### 树上操作

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 二进制跳表 LCA | `graph-theory/binary-lifting.hpp` | O(n log n) 建 / O(log n) 查 | 树上 K 祖先、距离、LCA |
| 欧拉序 LCA | `graph-theory/euler-lca.hpp` | O(n) 建 / O(1) 查 | RMQ 版 LCA（需 Sparse Table）|
| 重链剖分 HLD | `graph-theory/heavy-light.hpp` | O(log² n)（路径）| 树上路径更新/查询 |
| 质心分解 | `graph-theory/centroid-decomp.hpp` | O(n log n) | 树上分治，路径统计 |
| 虚树 | `graph-theory/virtual-tree.hpp` | O(k log k) | 关键点子集树压缩 |
| DSU on Tree | `graph-theory/dsu-on-tree.hpp` | O(n log n) | 子树信息批量合并（树上莫队）|

### 连通性与分量

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| Tarjan SCC | `graph-theory/tarjan-scc.hpp` | O(n+m) | 强连通分量 |
| Kosaraju | `graph-theory/kosaraju.hpp` | O(n+m) | 强连通分量（两遍 DFS）|
| 割点/桥 | `graph-theory/bridges.hpp` | O(n+m) | 点双连通、边双连通 |
| 双边连通分量 | `graph-theory/two-edge-connected.hpp` | O(n+m) | 边双连通分量缩点 |
| 双连通分量 | `graph-theory/biconnected.hpp` | O(n+m) | 点双连通分量 |

### 欧拉图与哈密顿

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 欧拉回路（无向）| `graph-theory/eulerian.hpp` | O(n+m) | 无向图欧拉回路/路径 |
| 欧拉路径（有向）| `graph-theory/eulerian-path.hpp` | O(n+m) | 有向图欧拉路径 |
| 哈密顿环 | `graph-theory/hamiltonian-cycle.hpp` | 状态压缩 | 哈密顿圈构造（NPC，无通用多项式解）|

### 网络流

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| Dinic | `graph-theory/dinic.hpp` | O(E√V)（整数）| 最大流（最常用）|
| Push-Relabel | `graph-theory/push-relabel.hpp` | O(V³)（最坏）| 大流量图、更高常数 |
| MCMF | `graph-theory/mcmf.hpp` | O(F·E·logV) | 最小费用最大流 |
| Push-Relabel MCMF | `graph-theory/push-relabel-mcmf.hpp` | 视实现 | Push-Relabel 版费用流 |

### 二分图匹配

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| Hopcroft-Karp | `graph-theory/hopcroft-karp.hpp` | O(E√V) | 二分图最大匹配（最快）|
| Hungarian KM | `graph-theory/km.hpp` | O(n³) | 二分图最大权匹配 |
| 增广路 BPM | `graph-theory/augmented-path-bpm.hpp` | O(VE) | 二分图最大匹配（简单版）|
| Blossom | `graph-theory/blossom.hpp` | O(n³) | 一般图最大匹配 |
| 2-SAT | `graph-theory/two-sat.hpp` | O(n+m) | 布尔约束满足 |

### 其他图论

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 环计数 | `graph-theory/count-cycles.hpp` | - | 统计图中环数量 |
| 最大团 | `graph-theory/max-clique.hpp` | 最坏指数级 | 最大团搜索（_branch & bound）|

---

## 字符串（Strings）— 12 个文件

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| KMP | `strings/kmp.hpp` | O(n+m) | 单模式匹配 |
| Z 函数 | `strings/z-function.hpp` | O(n+m) | 匹配+公共前缀 |
| Manacher | `strings/manacher.hpp` | O(n) | 最长回文子串/半径 |
| 字典树 | `strings/trie.hpp` | O(∑|s|) 建 | 前缀匹配、前缀计数 |
| AC 自动机 | `strings/ac-automaton.hpp` | O(∑|P|+|T|) | 多模式匹配 |
| 后缀数组 | `strings/suffix-array.hpp` | O(n log n) 倍增 | 字符串排名、LCP |
| 后缀数组（线性）| `strings/suffix-array-linear.hpp` | O(n) | SA 线性构建 |
| 后缀自动机 | `strings/suffix-automaton.hpp` | O(n) 建 | 所有子串查询、最长公共子串 |
| 扩展后缀自动机 | `strings/ex-sam.hpp` | O(∑|s|) 建 | 多串子串统计 |
| 回文树 | `strings/palindromic-tree.hpp` | O(n) | 回文统计、计数 |
| Lyndon 分解 | `strings/lyndon.hpp` | O(n) | 最小表示、Duval 算法 |
| 多项式哈希 | `strings/polyhash.hpp` | O(n) 建 / O(1) 查 | 子串哈希比较 |

---

## 数学（Math）— 22 个文件

### 基础数论

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 快速幂 | `math/fast-pow.hpp` | O(log k) | 模幂运算 |
| 扩展欧几里得 | `math/ex-gcd.hpp` | O(log min(a,b)) | 乘法逆元、线性同余、CRT |
| 中国剩余定理 | `math/crt.hpp` | O(k log m) | 合并模数互质的同余方程 |
| Miller-Rabin | `math/miller-rabin.hpp` | O(k log³n) | 质数判定（概率，高确定性）|
| Pollard-Rho | `math/pollard-rho.hpp` | O(n^1/4) 期望 | 大整数分解 |
| 质因数分解 | `math/factorization.hpp` | O(n^1/4) | 合数分解为质因数 |
| 质数筛 | `math/prime-sieve.hpp` | O(n log log n) | 线性筛质数 |
| 欧拉函数 | `math/euler-totient.hpp` | O(n) 或 O(log n) 单个 | φ(n) 计算 |
| BSGS | `math/bsgs.hpp` | O(√p log p) | 离散对数 a^x ≡ b (mod p) |

### 组合数学

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 二项式系数 | `math/binomial.hpp` | O(n) 预处理 | C(n,k) mod p |
| Lucas 定理 | `math/lucas.hpp` | O(p + log_p n) | 大组合数模小质数 |
| 组合数学汇总 | `math/combinatorics.hpp` | O(n) | 阶乘、排列、组合 |
| 卡特兰数 | `math/catalan.hpp` | O(1) 每项 | 合法括号、出栈序列 |
| 斯特林数 | `math/stirling.hpp` | O(n²) | 集合/圆排列划分 |
| Polya 计数 | `math/polya.hpp` | O(n log n) | 群作用染色方案计数 |

### 多项式与FFT

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| NTT | `math/ntt.hpp` | O(n log n) | 多项式乘法（模质数）|
| 任意模数 NTT | `math/ntt-arbitrary.hpp` | O(n log n) | 任意模数卷积（三模数合并）|
| FFT | `math/fft.hpp` | O(n log n) | 浮点多项式乘法 |
| 多项式基础 | `math/polynomial.hpp` | O(n²)~O(n log n) | 多项式加减乘除、求逆、求导 |
| 高斯消元 | `math/gauss.hpp` | O(n³) | 线性方程组、行列式 |
| 拉格朗日插值 | `math/lagrange.hpp` | O(n²) | 已知 n 点求任意 x 值 |
| 线性规划单纯形 | `math/simplex.hpp` | O(n³ m) | 线性规划最优解 |

### 数论前缀和

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| Min25 筛 | `math/min25.hpp` | O(n^(2/3)) | 积性函数前缀和（如 ∑ prime）|
| 杜教筛 | `math/dujiao.hpp` | O(n^(2/3)) | φ/μ 前缀和 |

---

## 计算几何（Geometry）— 9 个文件

| 算法 | 文件 | 时间复杂度 | 适用场景 |
|------|------|-----------|----------|
| 点与向量基础 | `geometry/point.hpp` | - | 叉积、点积、距离 |
| 直线与线段 | `geometry/line.hpp` / `geometry/segment.hpp` | - | 交点、距离、平行 |
| 角度工具 | `geometry/angle.hpp` | - | 角度运算、弧度转换 |
| 圆 | `geometry/circle.hpp` | - | 圆与圆/直线交点 |
| 多边形 | `geometry/polygon.hpp` | - | 面积、有向面积、内点判定 |
| 凸包（Andrew）| `geometry/convex-hull.hpp` | O(n log n) | 点集最小包围凸多边形 |
| 旋转卡壳 | `geometry/rotating-calipers.hpp` | O(n)（在凸包上）| 凸包直径、最远点对 |
| 半平面交 | `geometry/half-plane.hpp` | O(n log n) | 线性规划、多边形交集 |
| 扫描线 | `geometry/scanning-line.hpp` | O(n log n) | 矩形并面积、区间合并 |

---

## 动态规划（Dynamic Programming）— 6 个文件

| 优化类型 | 文件 | 复杂度 | 条件 |
|---------|------|--------|------|
| 背包 DP | `dynamic-programming/knapsack.hpp` | O(n·W) | 组合优化 |
| 区间 DP | `dynamic-programming/interval-dp.hpp` | O(n³) | 区间合并、区间划分 |
| 数位 DP | `dynamic-programming/digit-dp.hpp` | O(n·states) | 按位统计、数字计数 |
| 状态压缩 DP | `dynamic-programming/state-compress.hpp` | O(n·2^n) | n≤20 集合遍历 |
| 斜率优化 DP | `dynamic-programming/slope-optim.hpp` | O(n) 或 O(n log n) | 决策单调、cost 线性 |
| CDQ 分治优化 | `dynamic-programming/divide-conquer-dp.hpp` | O(n log n) | 高维偏序、斜率优化（非单调）|

---

## 位运算（Bit）— 3 个文件

| 操作 | 文件 | 说明 |
|------|------|------|
| XOR 线性基 | `bit/xor-basis.hpp` | 最大异或和、子集异或、贪心 |
| 枚举子集 | `bit/iterate-submasks.hpp` | `for(sub=mask; sub; sub=(sub-1)&mask)` |
| 枚举超集 | `bit/iterate-supermasks.hpp` | 从子集枚举到全集 |

---

## 杂项（Misc）— 8 个文件

| 工具 | 文件 | 说明 |
|------|------|------|
| 大整数 | `misc/bigint.hpp` | 高精度加减乘除 |
| 表达式求值 | `misc/expression.hpp` | 中缀表达式解析 |
| 自定义哈希 | `misc/custom-hash.hpp` | 防止哈希攻击（随机种子）|
| 模整数 | `misc/modint.hpp` | 自动取模运算类型 |
| 快速幂 | `misc/fast-pow.hpp` | 重复包含备用 |
| 逆序对 | `misc/inversion-count.hpp` | 归并排序求逆序数 |
| 博弈论单纯形 | `misc/simplex-game.hpp` | 两人零和博弈 |
| 通用工具 | `misc/util.hpp` | 常用辅助函数 |