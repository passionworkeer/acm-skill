# 算法分类指南

本指南按类别整理 ACM Champion 库中的全部 107 个文件，帮助按问题类型快速定位所需工具。每个表格均包含算法名称、头文件路径、功能描述和典型问题陈述。

---

## 1. 数据结构（Data Structures）

**核心问题**：区间操作、有序集合、树上辅助结构。

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| 树状数组（点更新）| `data-structures/fenwick.hpp` | 单点增减 + 前缀和查询 | 动态前缀和、逆序对 |
| 树状数组（区间）| `data-structures/fenwick-range.hpp` | 区间增减 + 区间求和 | 区间染色计数 |
| 线段树（基础）| `data-structures/segtree.hpp` | 区间操作（加/乘/最值）| 区间最值查询 |
| 懒标记线段树 | `data-structures/segtree-lazy.hpp` | 区间批量更新 + 查询，懒传播 | 区间赋值求和，区间乘加 |
| 可持久化线段树 | `data-structures/segtree-persistent.hpp` | 历史版本查询，函数式 | 区间第 k 小（K-th number）|
| 稀疏表 | `data-structures/sparse-table.hpp` | 静态 RMQ，O(1) 查询 | 区间最小值（只读）|
| 稀疏表（2D）| `data-structures/sparse-table-2d.hpp` | 静态二维 RMQ | 二维区间最值 |
| 并查集 | `data-structures/union-find.hpp` | 集合合并、连通分量 | 最小生成树、连通性 |
| Treap（分裂合并）| `data-structures/treap.hpp` | 有序集合，第 k 小，排名 | 动态中位数 |
| Treap（旋转）| `data-structures/treap-rotate.hpp` | 与上同，旋转实现 | 同上 |
| Treap（集合版）| `data-structures/treap-set.hpp` | 替代 `std::set`，去重 | 有序去重维护 |
| Treap（分裂版）| `data-structures/treap-split.hpp` | 按关键字分裂合并 | 动态顺序统计 |
| 配对堆 | `data-structures/pairing-heap.hpp` | 优先队列，斐波那契堆替代 | Dijkstra 堆优化（竞赛用）|
| 最小堆 | `data-structures/min-heap.hpp` | 标准最小优先队列 | 贪心辅助结构 |
| Link-Cut Tree | `data-structures/lct.hpp` | 动态树：连边/断边，路径聚合 | 动态边权修改、树上路径最值 |
| KD-Tree | `data-structures/kd-tree.hpp` | 动态二维范围和、K 近邻 | 二维平面最近点对 |
| 莫队算法 | `data-structures/mo-algorithm.hpp` | 离线区间询问 | 区间众数、区间不同数 |
| 单调队列 | `data-structures/sliding-window.hpp` | 滑动窗口最值 | 滑动窗口最大值 |
| 单调 DP 凸包 | `data-structures/monotonic-dp-hull.hpp` | DP 斜率优化（单调队列版）| 决策单调 DP |
| 动态凸包 | `data-structures/dynamic-hull.hpp` | 动态插入点，查询切线 | 动态凸包最值 |
| 二维差分数组 | `data-structures/2d-diff-array.hpp` | O(1) 二维区间增减 | 矩形染色 |
| 二维前缀和 | `data-structures/2d-pref-sum.hpp` | O(1) 二维区域求和 | 静态子矩阵和 |

---

## 2. 图论（Graph Theory）

### 最短路

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| Dijkstra（堆优化）| `graph-theory/dijkstra.hpp` | 单源非负权最短路 | 城市间最短路径 |
| Bellman-Ford | `graph-theory/bellman-ford.hpp` | 含负边单源最短路、负环检测 | 含负权路的最短路 |

### 树上操作

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| 二进制跳表 LCA | `graph-theory/binary-lifting.hpp` | K 祖先、LCA、树上距离 | 树上两节点距离 |
| 欧拉序 LCA | `graph-theory/euler-lca.hpp` | O(1) LCA（需 Sparse Table）| 高频 LCA 查询 |
| 重链剖分 HLD | `graph-theory/heavy-light.hpp` | 树上路径更新 / 查询 | 路径求和、更改边权 |
| 质心分解 | `graph-theory/centroid-decomp.hpp` | 树上分治，路径统计 | 距离为 K 的点对数 |
| 虚树 | `graph-theory/virtual-tree.hpp` | 关键点子集树压缩 | 多关键点树上 DP |
| DSU on Tree | `graph-theory/dsu-on-tree.hpp` | 子树信息批量合并 | 子树颜色数、频率统计 |

### 连通性与分量

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| Tarjan SCC | `graph-theory/tarjan-scc.hpp` | 强连通分量（一次 DFS）| 缩点后 DAG |
| Kosaraju | `graph-theory/kosaraju.hpp` | 强连通分量（两遍 DFS）| 同上，另一种写法 |
| 割点 / 桥 | `graph-theory/bridges.hpp` | 找出所有割点和桥 | 删除某边使图不连通 |
| 双边连通分量 | `graph-theory/two-edge-connected.hpp` | 边双连通分量缩点 | 边双连通加边数 |
| 双连通分量 | `graph-theory/biconnected.hpp` | 点双连通分量 | 点双连通加边数 |

### 欧拉图与哈密顿

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| 欧拉回路（无向）| `graph-theory/eulerian.hpp` | 无向图欧拉回路 / 路径构造 | 一笔画问题 |
| 欧拉路径（有向）| `graph-theory/eulerian-path.hpp` | 有向图欧拉路径 | 有向一笔画 |
| 哈密顿环 | `graph-theory/hamiltonian-cycle.hpp` | 哈密顿圈构造（状态压缩）| 旅行商问题（NPC，无通用多项式）|

### 网络流

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| Dinic | `graph-theory/dinic.hpp` | 最大流，O(E√V) 整数 | 二分图最大匹配、割最小 |
| Push-Relabel | `graph-theory/push-relabel.hpp` | 推送-重贴标签最大流 | 大流量图 |
| MCMF | `graph-theory/mcmf.hpp` | 最小费用最大流 | 费用最小的运输方案 |
| Push-Relabel MCMF | `graph-theory/push-relabel-mcmf.hpp` | PR 版费用流 | 大规模最小费用流 |

### 二分图匹配

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| Hopcroft-Karp | `graph-theory/hopcroft-karp.hpp` | 二分图最大匹配，O(E√V) | 最多能匹配多少对 |
| Hungarian KM | `graph-theory/km.hpp` | 二分图最大权匹配 | 权值最大的匹配方案 |
| 增广路 BPM | `graph-theory/augmented-path-bpm.hpp` | 二分图最大匹配，O(VE) | 简单图最大匹配 |
| Blossom | `graph-theory/blossom.hpp` | 一般图最大匹配 | 奇环存在时的最大匹配 |
| 2-SAT | `graph-theory/two-sat.hpp` | 布尔蕴含约束满足 | 变量赋值是否可满足 |

### 其他图论

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| 环计数 | `graph-theory/count-cycles.hpp` | 统计图中简单环数量 | 环数量分析 |
| 最大团 | `graph-theory/max-clique.hpp` | 分支定界搜索最大团 | NPC，通常跑不满 |

---

## 3. 字符串（Strings）

**核心问题**：模式匹配、子串结构、回文检测。

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| KMP | `strings/kmp.hpp` | 单模式匹配，O(n+m) | 查找子串位置 |
| Z 函数 | `strings/z-function.hpp` | 最长公共前缀，O(n+m) | 匹配+相似度分析 |
| Manacher | `strings/manacher.hpp` | 最长回文子串，O(n) | 回文半径 / 子串 |
| 字典树 | `strings/trie.hpp` | 前缀插入 / 计数 | 前缀统计、敏感词过滤 |
| AC 自动机 | `strings/ac-automaton.hpp` | 多模式匹配 | 同时匹配多个模式串 |
| 后缀数组 | `strings/suffix-array.hpp` | 字符串排序，O(n log n) | LCP、字符串排名 |
| 后缀数组（线性）| `strings/suffix-array-linear.hpp` | SA 线性构建 | 大规模字符串排序 |
| 后缀自动机 | `strings/suffix-automaton.hpp` | 所有子串 O(n) 建 / 查 | 子串出现次数、最长公共子串 |
| 扩展后缀自动机 | `strings/ex-sam.hpp` | 多串子串统计 | 多字符串公共子串 |
| 回文树 | `strings/palindromic-tree.hpp` | 回文统计，O(n) | 本质不同回文子串数量 |
| Lyndon 分解 | `strings/lyndon.hpp` | 最小表示，Duval 算法 | 字符串最小表示 |
| 多项式哈希 | `strings/polyhash.hpp` | 子串哈希 O(1) | 近似匹配、查重 |

---

## 4. 数学（Math）

### 基础数论

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| 快速幂 | `math/fast-pow.hpp` | 模幂运算，O(log k) | a^k mod p |
| 扩展欧几里得 | `math/ex-gcd.hpp` | 乘法逆元、线性同余 | 求 a^-1 mod p |
| 中国剩余定理 | `math/crt.hpp` | 合并模数互质同余方程 | 求满足多个同余条件的数 |
| Miller-Rabin | `math/miller-rabin.hpp` | 质数判定（概率高确定性）| 判断大数是否为质数 |
| Pollard-Rho | `math/pollard-rho.hpp` | 大整数分解 | 分解 10¹⁸ 以内整数 |
| 质因数分解 | `math/factorization.hpp` | 合数分解为质因数 | 求所有质因子及指数 |
| 质数筛 | `math/prime-sieve.hpp` | 线性筛质数及 φ/μ | 预处理质数表 |
| 欧拉函数 | `math/euler-totient.hpp` | φ(n) 计算 | 欧拉函数前缀和 |
| BSGS | `math/bsgs.hpp` | 离散对数 a^x ≡ b (mod p) | 求 x 最小正整数解 |

### 组合数学

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| 二项式系数 | `math/binomial.hpp` | C(n,k) mod p | 大组合数取模 |
| Lucas 定理 | `math/lucas.hpp` | C(n,k) mod p（小质数）| n,k 极大时的组合数 |
| 组合数学汇总 | `math/combinatorics.hpp` | 阶乘、排列、组合 | 综合组合计数 |
| 卡特兰数 | `math/catalan.hpp` | Cat_n = C(2n,n)/(n+1) | 合法括号序列、出栈序列 |
| 斯特林数 | `math/stirling.hpp` | 第 1/2 类斯特林数 | 集合划分、圆排列 |
| Polya 计数 | `math/polya.hpp` | 群作用下的染色方案数 | 手镯 / 立方体染色 |

### 多项式与 FFT

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| NTT | `math/ntt.hpp` | 数论变换，模质数卷积 | 多项式乘法（整数域）|
| 任意模数 NTT | `math/ntt-arbitrary.hpp` | 任意模数卷积（三模数合并）| 非质数模数卷积 |
| FFT | `math/fft.hpp` | 浮点多项式乘法 | 系数较大时卷积 |
| 多项式基础 | `math/polynomial.hpp` | 多项式加减乘除、求逆、求导 | 构造生成函数 |
| 高斯消元 | `math/gauss.hpp` | 解线性方程组、求行列式 | n 元线性方程组 |
| 拉格朗日插值 | `math/lagrange.hpp` | O(n²) 插值 | 已知前 n 项求第 k 项 |
| 线性规划单纯形 | `math/simplex.hpp` | 标准形线性规划最优解 | 资源分配最优方案 |

### 数论前缀和

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| Min25 筛 | `math/min25.hpp` | O(n^(2/3)) 积性函数前缀和 | ∑ prime ≤ n、∑ φ(i) |
| 杜教筛 | `math/dujiao.hpp` | O(n^(2/3)) φ/μ 前缀和 | 积性函数求和 |

---

## 5. 计算几何（Computational Geometry）

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| 点与向量基础 | `geometry/point.hpp` | 叉积、点积、距离、极角 | 所有几何问题的基石 |
| 直线 | `geometry/line.hpp` | 直线表示、交点、距离 | 直线与直线关系 |
| 线段 | `geometry/segment.hpp` | 线段交点、距离判定 | 线段相交检测 |
| 角度工具 | `geometry/angle.hpp` | 角度运算、弧度转换 | 角度范围内的点数 |
| 圆 | `geometry/circle.hpp` | 圆与圆/直线交点、切线 | 圆相关面积 |
| 多边形 | `geometry/polygon.hpp` | 面积、有向面积、内点判定 | 点是否在多边形内 |
| 凸包（Andrew）| `geometry/convex-hull.hpp` | 点集最小凸多边形 | 平面点集最值 |
| 旋转卡壳 | `geometry/rotating-calipers.hpp` | 凸包直径、最远点对 | 凸包直径 / 宽度 |
| 半平面交 | `geometry/half-plane.hpp` | 多个半平面的交集 | 线性规划可行域 |
| 扫描线 | `geometry/scanning-line.hpp` | 矩形并面积、区间合并 | 矩形覆盖面积 |

---

## 6. 动态规划（Dynamic Programming）

**核心问题**：状态爆炸压缩、转移顺序优化。

| 优化类型 | 头文件 | 条件 | 典型问题 |
|---------|--------|------|---------|
| 背包 DP | `dynamic-programming/knapsack.hpp` | 物品重量价值 | 0/1 背包、完全背包、多重背包 |
| 区间 DP | `dynamic-programming/interval-dp.hpp` | 区间合并 | 石子合并、括号序列 |
| 数位 DP | `dynamic-programming/digit-dp.hpp` | 按位处理 | [L,R] 内满足条件的数的个数 |
| 状态压缩 DP | `dynamic-programming/state-compress.hpp` | n ≤ 20，集合枚举 | TSP、铺砖、集合覆盖 |
| 斜率优化 DP | `dynamic-programming/slope-optim.hpp` | 决策单调，斜率单调或可二分 | 烽火传递、任务分配 |
| CDQ 分治优化 | `dynamic-programming/divide-conquer-dp.hpp` | 偏序转移，非单调斜率 | 高维 DP、斜率优化（非单调）|

---

## 7. 位运算与杂项（Bit / Misc）

| 算法名称 | 头文件 | 功能 | 典型问题 |
|---------|--------|------|---------|
| XOR 线性基 | `bit/xor-basis.hpp` | 最大异或和、子集异或 | 最大异或子集 |
| 枚举子集 | `bit/iterate-submasks.hpp` | `for(sub=mask; sub; sub=(sub-1)&mask)` | 状态压缩 DP 转移 |
| 枚举超集 | `bit/iterate-supermasks.hpp` | 从子集枚举到全集 | SOS DP |
| 大整数 | `misc/bigint.hpp` | 高精度加减乘除 | 超过 64 位整数范围 |
| 表达式求值 | `misc/expression.hpp` | 中缀表达式解析计算 | 表达式计算器 |
| 自定义哈希 | `misc/custom-hash.hpp` | 随机种子抗哈希攻击 | `unordered_map` 安全化 |
| 模整数 | `misc/modint.hpp` | 自动取模的整数类型 | NTT 中减少取模代码 |
| 逆序对 | `misc/inversion-count.hpp` | 归并排序求逆序数 | 逆序对计数 |
| 博弈论单纯形 | `misc/simplex-game.hpp` | 两人零和博弈最优策略 | 博弈论最优混合策略 |
| 通用工具 | `misc/util.hpp` | 常用辅助函数 | 快读快写、随机数等 |

---

## 问题 → 算法 反向查找表

不知道用什么算法？按问题特征直接查表：

### 最短路与网络流

| 问题 | 算法 | 头文件 |
|------|------|--------|
| 单源非负权最短路 | Dijkstra | `graph-theory/dijkstra.hpp` |
| 含负权的最短路 | Bellman-Ford | `graph-theory/bellman-ford.hpp` |
| 最大流 | Dinic | `graph-theory/dinic.hpp` |
| 更大流量的最大流 | Push-Relabel | `graph-theory/push-relabel.hpp` |
| 最小费用最大流 | MCMF | `graph-theory/mcmf.hpp` |
| 二分图最大匹配 | Hopcroft-Karp | `graph-theory/hopcroft-karp.hpp` |
| 二分图最大权匹配 | Hungarian KM | `graph-theory/km.hpp` |
| 一般图最大匹配 | Blossom | `graph-theory/blossom.hpp` |
| 有上下界的网络流 | Dinic（加超级源汇）| `graph-theory/dinic.hpp` |

### 树上操作

| 问题 | 算法 | 头文件 |
|------|------|--------|
| LCA / K 祖先 | 二进制跳表 | `graph-theory/binary-lifting.hpp` |
| 高频 LCA 查询 | 欧拉序 LCA | `graph-theory/euler-lca.hpp` |
| 树上路径更新 / 查询 | HLD | `graph-theory/heavy-light.hpp` |
| 动态树连边断边 | Link-Cut Tree | `data-structures/lct.hpp` |
| 树上路径统计（多次）| 质心分解 | `graph-theory/centroid-decomp.hpp` |
| 多关键点树上 DP | 虚树 | `graph-theory/virtual-tree.hpp` |
| 子树批量合并 | DSU on Tree | `graph-theory/dsu-on-tree.hpp` |
| 树上割点 / 桥 | Bridges | `graph-theory/bridges.hpp` |

### 连通性

| 问题 | 算法 | 头文件 |
|------|------|--------|
| 强连通分量 | Tarjan SCC / Kosaraju | `graph-theory/tarjan-scc.hpp` |
| 点双连通分量 | Biconnected | `graph-theory/biconnected.hpp` |
| 边双连通分量 | Two-Edge-Connected | `graph-theory/two-edge-connected.hpp` |
| 欧拉回路（无向）| Eulerian | `graph-theory/eulerian.hpp` |
| 欧拉路径（有向）| Eulerian-Path | `graph-theory/eulerian-path.hpp` |

### 字符串

| 问题 | 算法 | 头文件 |
|------|------|--------|
| 单模式匹配 | KMP | `strings/kmp.hpp` |
| 多模式匹配 | AC 自动机 | `strings/ac-automaton.hpp` |
| 最长回文子串 | Manacher | `strings/manacher.hpp` |
| 所有子串查询 | 后缀自动机 | `strings/suffix-automaton.hpp` |
| 多串公共子串 | 扩展后缀自动机 | `strings/ex-sam.hpp` |
| 回文统计 | 回文树 | `strings/palindromic-tree.hpp` |
| 字符串排序 | 后缀数组 | `strings/suffix-array.hpp` |
| 最小表示 | Lyndon 分解 | `strings/lyndon.hpp` |
| 子串哈希比较 | 多项式哈希 | `strings/polyhash.hpp` |

### 区间 / 序列操作

| 问题 | 算法 | 头文件 |
|------|------|--------|
| 单点更新 + 前缀和 | 树状数组 | `data-structures/fenwick.hpp` |
| 区间更新 + 区间和 | 区间树状数组 | `data-structures/fenwick-range.hpp` |
| 区间操作（复杂）| 线段树（懒）| `data-structures/segtree-lazy.hpp` |
| 静态区间最值 | 稀疏表 | `data-structures/sparse-table.hpp` |
| 历史版本区间查询 | 可持久化线段树 | `data-structures/segtree-persistent.hpp` |
| 滑动窗口最值 | 单调队列 | `data-structures/sliding-window.hpp` |
| 离线区间询问 | 莫队 | `data-structures/mo-algorithm.hpp` |
| 动态第 k 小 | 可持久化线段树 | `data-structures/segtree-persistent.hpp` |

### 数学 / 数论

| 问题 | 算法 | 头文件 |
|------|------|--------|
| 模幂运算 | 快速幂 | `math/fast-pow.hpp` |
| 乘法逆元 / CRT | 扩展欧几里得 | `math/ex-gcd.hpp` |
| 大数质数判定 | Miller-Rabin | `math/miller-rabin.hpp` |
| 大整数分解 | Pollard-Rho | `math/pollard-rho.hpp` |
| 离散对数 | BSGS | `math/bsgs.hpp` |
| 大组合数（模小质数）| Lucas | `math/lucas.hpp` |
| 多项式乘法 | NTT / FFT | `math/ntt.hpp` |
| 积性函数前缀和 | Min25 筛 | `math/min25.hpp` |
| φ / μ 前缀和 | 杜教筛 | `math/dujiao.hpp` |
| 线性方程组 | 高斯消元 | `math/gauss.hpp` |
| 已知前 n 项求任意项 | 拉格朗日插值 | `math/lagrange.hpp` |
| 卡特兰数计数 | Catalan | `math/catalan.hpp` |

### 动态规划优化

| 问题 | 条件 | 算法 | 头文件 |
|------|------|------|--------|
| 决策单调 DP | 斜率单调 | 斜率优化（单调队列）| `dynamic-programming/slope-optim.hpp` |
| 决策单调 DP | 斜率不单调 | CDQ 分治 | `dynamic-programming/divide-conquer-dp.hpp` |
| 高维偏序 DP | 三维偏序 | CDQ 分治 | `dynamic-programming/divide-conquer-dp.hpp` |
| 数位统计 | 按位处理 | 数位 DP | `dynamic-programming/digit-dp.hpp` |
| 集合遍历 | n ≤ 20 | 状态压缩 DP | `dynamic-programming/state-compress.hpp` |
| 区间合并 | 区间划分 | 区间 DP | `dynamic-programming/interval-dp.hpp` |

### 计算几何

| 问题 | 算法 | 头文件 |
|------|------|--------|
| 点集凸包 | Andrew / Graham | `geometry/convex-hull.hpp` |
| 凸包直径 | 旋转卡壳 | `geometry/rotating-calipers.hpp` |
| 矩形并面积 | 扫描线 | `geometry/scanning-line.hpp` |
| 半平面交 | 半平面交算法 | `geometry/half-plane.hpp` |
| 点在多边形内 | 射线法 / 弧角法 | `geometry/polygon.hpp` |
| 圆与圆交点 | 解析几何 | `geometry/circle.hpp` |

### 位运算与集合

| 问题 | 算法 | 头文件 |
|------|------|--------|
| 最大异或和 | XOR 线性基 | `bit/xor-basis.hpp` |
| 子集枚举 / SOS DP | 子集枚举 | `bit/iterate-submasks.hpp` |
| 超集枚举 | 超集枚举 | `bit/iterate-supermasks.hpp` |

---

> 本库所有文件均使用 `#pragma once`，依赖 `<bits/stdc++.h>`，无外部依赖。按问题类型查找上表，确认头文件路径后直接 `#include` 即可使用。