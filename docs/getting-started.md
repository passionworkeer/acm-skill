# ACM Champion 快速上手

ACM Champion 是一个面向 ACM/ICPC/XCPC 竞赛的冠军级算法模板库，涵盖图论、字符串、数学、数据结构、几何、动态规划等方向，全部文件均为单头文件、无外部依赖，可直接 `#include` 使用。

---

## 安装方式

### 方式一：复制整个目录（最常用）

将 `acm-champion/` 目录复制到你的项目任意位置，直接包含即可：

```cpp
#include "acm-champion/graph-theory/dijkstra.hpp"
#include "acm-champion/strings/kmp.hpp"
```

所有文件使用 `#pragma once`，不会产生重复包含问题。

### 方式二：Git 子模块

```bash
git submodule add https://github.com/passionworkeer/acm-champion acm-champion
```

更新子模块：
```bash
git submodule update --init --recursive
```

### 方式三：按需复制单文件

如果只需要某个算法，直接复制对应的 `.hpp` 文件到项目目录。所有文件只依赖 `<bits/stdc++.h>`（竞赛通用头文件），可独立编译，无任何外部依赖。

---

## 第一个程序：单源最短路

**问题**：给定 n 个节点、m 条边的无向带权图（边权非负），求从节点 0 到所有节点的最短距离。

**算法**：Dijkstra + 堆优化，竞赛中出现频率最高的算法。

```cpp
#include <bits/stdc++.h>
using namespace std;

#include "acm-champion/graph-theory/dijkstra.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<pair<int,int>>> g(n);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});           // 无向图，有向图删掉这行
    }

    vector<long long> dist = dijkstra(g, 0);
    for (int i = 0; i < n; i++)
        cout << dist[i] << (i+1==n?'\n':' ');
    return 0;
}
```

**复杂度**：O((n+m) log n)，空间 O(n+m)。

**常见变种**：
- 有向图：删除建立反向边的那一行。
- 负权边：改用 `bellman-ford.hpp` 或 SPFA（竞赛中 SPFA 容易被卡，慎用）。
- 多源最短路：把多个源点都 push 进初始堆中。
- 输出路径：用 `prev` 数组记录前驱，逆推路径。

---

## 第二个程序：字符串模式匹配

**问题**：给定模式串 `p` 和文本串 `t`，输出 `p` 在 `t` 中所有匹配位置（1-indexed）。

**算法**：KMP（Knuth-Morris-Pratt），时间最优且稳定，不会退化。

```cpp
#include <bits/stdc++.h>
using namespace std;

#include "acm-champion/strings/kmp.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string p, t;
    cin >> p >> t;

    vector<int> pi = prefix_function(p);          // 预处理前缀函数
    for (int i = 0, j = 0; i < (int)t.size(); i++) {
        while (j > 0 && t[i] != p[j]) j = pi[j-1]; // 失配时跳转
        if (t[i] == p[j]) j++;
        if (j == (int)p.size()) {
            cout << i - j + 2 << '\n';              // 1-indexed 位置
            j = pi[j-1];                            // 继续找重叠匹配
        }
    }
    return 0;
}
```

**复杂度**：O(|p| + |t|)，空间 O(|p|)。

**常见变种**：
- 统计出现次数：匹配成功时计数，不立即 j=0。
- 多个模式串：改用 `ac-automaton.hpp`（AC 自动机）。
- 回文相关：改用 `manacher.hpp`（O(n) 求最长回文子串）。
- 子串查询：改用 `suffix-automaton.hpp`（后缀自动机，O(n) 建，可查所有子串）。

---

## 目录结构详解

```
acm-champion/
├── data-structures/      # 22 个文件
│   ├── fenwick.hpp        # 树状数组，单点更新+前缀和
│   ├── fenwick-range.hpp  # 区间加+区间和
│   ├── segtree.hpp        # 线段树基础版
│   ├── segtree-lazy.hpp   # 懒标记线段树（最常用）
│   ├── segtree-persistent.hpp  # 可持久化线段树
│   ├── sparse-table.hpp   # 静态 RMQ，O(1) 查询
│   ├── union-find.hpp     # 并查集
│   ├── treap.hpp          # Treap 分裂合并版
│   ├── treap-rotate.hpp   # Treap 旋转版
│   ├── treap-set.hpp      # Treap 替代 set
│   ├── treap-split.hpp    # Treap 按关键字分裂
│   ├── lct.hpp            # Link-Cut Tree，动态树
│   ├── kd-tree.hpp        # KD-Tree，2D 范围查询
│   ├── mo-algorithm.hpp   # 莫队，离线区间
│   ├── sliding-window.hpp # 单调队列，滑动窗口
│   ├── monotonic-dp-hull.hpp  # 单调 DP 凸包
│   ├── dynamic-hull.hpp   # 动态凸包
│   ├── pairing-heap.hpp   # 配对堆
│   ├── min-heap.hpp       # 最小堆
│   ├── 2d-diff-array.hpp  # 二维差分
│   └── 2d-pref-sum.hpp    # 二维前缀和
│
├── graph-theory/          # 28 个文件
│   ├── dijkstra.hpp       # 最短路（最高频）
│   ├── bellman-ford.hpp   # 负权最短路
│   ├── binary-lifting.hpp  # 二进制跳表，LCA
│   ├── euler-lca.hpp      # 欧拉序 LCA，O(1) 查询
│   ├── heavy-light.hpp    # 轻重链剖分
│   ├── centroid-decomp.hpp  # 质心分解
│   ├── virtual-tree.hpp   # 虚树
│   ├── dsu-on-tree.hpp    # 树上 DSU
│   ├── bridges.hpp        # 割边/桥
│   ├── biconnected.hpp    # 点双连通分量
│   ├── two-edge-connected.hpp  # 边双连通
│   ├── tarjan-scc.hpp     # 强连通分量
│   ├── kosaraju.hpp       # Kosaraju SCC
│   ├── eulerian.hpp       # 无向欧拉回路
│   ├── eulerian-path.hpp  # 有向欧拉路径
│   ├── hamiltonian-cycle.hpp  # 哈密顿圈
│   ├── dinic.hpp          # 最大流（最高频）
│   ├── push-relabel.hpp   # 推送-重贴标签
│   ├── mcmf.hpp           # 最小费用最大流
│   ├── push-relabel-mcmf.hpp  # PR 版费用流
│   ├── hopcroft-karp.hpp  # 二分图最大匹配
│   ├── hungarian.hpp      # KM 算法，权匹配
│   ├── augmented-path-bpm.hpp  # 增广路匹配
│   ├── blossom.hpp        # 一般图匹配
│   ├── two-sat.hpp        # 2-SAT
│   ├── count-cycles.hpp   # 环计数
│   └── max-clique.hpp     # 最大团
│
├── strings/              # 12 个文件
│   ├── kmp.hpp           # 单模式匹配（高频）
│   ├── z-function.hpp    # Z 函数
│   ├── manacher.hpp      # 回文子串 O(n)
│   ├── trie.hpp          # 字典树
│   ├── ac-automaton.hpp  # AC 自动机，多模式匹配
│   ├── suffix-array.hpp  # 后缀数组
│   ├── suffix-array-linear.hpp  # SA 线性
│   ├── suffix-automaton.hpp  # SAM，子串终极武器
│   ├── ex-sam.hpp        # 扩展后缀自动机
│   ├── palindromic-tree.hpp  # 回文树
│   ├── lyndon.hpp        # Lyndon 分解
│   └── polyhash.hpp      # 字符串哈希
│
├── math/                 # 22 个文件
│   ├── fast-pow.hpp      # 快速幂（基础工具）
│   ├── ex-gcd.hpp        # 扩展欧几里得
│   ├── crt.hpp           # 中国剩余定理
│   ├── bsgs.hpp          # BSGS，离散对数
│   ├── miller-rabin.hpp  # Miller-Rabin 质数判定
│   ├── pollard-rho.hpp   # Pollard-Rho 分解
│   ├── factorization.hpp # 质因数分解
│   ├── prime-sieve.hpp   # 质数筛
│   ├── euler-totient.hpp # 欧拉函数
│   ├── binomial.hpp      # 二项式系数
│   ├── lucas.hpp         # Lucas 定理
│   ├── combinatorics.hpp # 组合数学汇总
│   ├── ntt.hpp           # NTT 数论变换
│   ├── ntt-arbitrary.hpp # 任意模数 NTT
│   ├── fft.hpp           # FFT
│   ├── polynomial.hpp    # 多项式基础
│   ├── gauss.hpp         # 高斯消元
│   ├── lagrange.hpp      # 拉格朗日插值
│   ├── simplex.hpp       # 单纯形（线性规划）
│   ├── min25.hpp         # Min25 筛（积性函数前缀和）
│   ├── dujiao.hpp        # 杜教筛
│   ├── catalan.hpp       # 卡特兰数
│   ├── stirling.hpp      # 斯特林数
│   └── polya.hpp         # Polya 计数
│
├── geometry/            # 9 个文件
│   ├── point.hpp         # 点与向量基础
│   ├── line.hpp          # 直线
│   ├── segment.hpp       # 线段
│   ├── angle.hpp         # 角度工具
│   ├── circle.hpp        # 圆
│   ├── polygon.hpp       # 多边形
│   ├── convex-hull.hpp  # 凸包
│   ├── rotating-calipers.hpp  # 旋转卡壳
│   ├── half-plane.hpp    # 半平面交
│   └── scanning-line.hpp # 扫描线
│
├── dynamic-programming/  # 6 个文件
│   ├── knapsack.hpp      # 背包 DP
│   ├── interval-dp.hpp   # 区间 DP
│   ├── digit-dp.hpp      # 数位 DP
│   ├── state-compress.hpp  # 状态压缩 DP
│   ├── slope-optim.hpp   # 斜率优化 DP
│   └── divide-conquer-dp.hpp  # CDQ 分治优化 DP
│
├── bit/                  # 3 个文件
│   ├── xor-basis.hpp     # XOR 线性基
│   ├── iterate-submasks.hpp  # 子集枚举
│   └── iterate-supermasks.hpp  # 超集枚举
│
├── misc/                  # 8 个文件
│   ├── bigint.hpp        # 大整数
│   ├── expression.hpp     # 表达式求值
│   ├── custom-hash.hpp   # 抗哈希攻击哈希
│   ├── modint.hpp        # 模整数类型
│   ├── fast-pow.hpp      # 快速幂（备选）
│   ├── inversion-count.hpp  # 逆序对
│   ├── simplex-game.hpp   # 博弈论单纯形
│   └── util.hpp           # 通用工具
│
├── docs/                  # 入门文档
│   ├── getting-started.md  # 本文件：安装和快速上手
│   └── algorithm-guide.md  # 算法分类指南
│
└── references/            # 参考文档
    ├── quick-index.md     # 快速索引（文件名→复杂度→场景）
    └── common-patterns.md  # 常用竞赛模式
```

---

## 常见问题 FAQ

**Q: 如何在代码中引入多个模板文件？会不会冲突？**

不会冲突。每个 `.hpp` 文件使用 `#pragma once`，且均依赖 `<bits/stdc++.h>`，按需直接 `#include` 即可。头文件之间相互独立，无依赖关系。

**Q: 竞赛环境（OJ）支持 C++17 吗？**

主流 OJ（如 Codeforces、AtCoder、LOJ、Luogu）均支持 C++17。`#pragma once` 是 MSVC/GCC/Clang 均支持的非标准但广泛兼容的宏。如果 OJ 不支持，可以用传统 `#ifndef ... #define ... #endif` 包裹。

**Q: 如何选择合适的算法？如果选错了怎么办？**

1. 先看数据规模：n ≤ 2×10⁵ 用 O(n log n)，n ≤ 10⁵ 用 O(n √n)。
2. 识别问题类型：区间操作 → 线段树/树状数组；树上路径 → HLD/LCA；字符串匹配 → KMP/AC自动机；最优化 → 二分答案/分数规划。
3. 参考 `references/quick-index.md` 的"适用场景"列。
4. 选错算法通常会 TLE（看是否超时）或 WA（看是否逻辑错误），根据反馈调整。

**Q: 如何调试模板代码？**

- 本地 IDE（如 VS Code + C++ 插件）断点调试是最可靠的方式。
- 在关键函数前后加 `assert` 检查前置条件（如 `if (l > r) return`）。
- 复杂度不确定时，用小数据手工模拟验证逻辑正确性。
- 图论算法：构造小图（3~5 个点）画出来对照。

**Q: 模板代码的性能如何？有没有可能被卡常数？**

- 所有模板均使用 `std::vector` 和标准库容器，在竞赛环境下常数小。
- 如果担心常数：树状数组比线段树常数小（优先使用）；Dinic 在大多数情况下比 Push-Relabel 快；莫队的块大小可用 `int block = sqrt(n)` 微调。
- 极常数敏感场景：用 `scanf` / `printf` 替代 `cin` / `cout`（已用 `ios::sync_with_stdio(false)` 优化过）。

**Q: 想深入学习某个算法，有哪些参考资源？**

- OI Wiki（https://oi-wiki.org）：中文竞赛算法百科，最全面。
- cp-algorithms.com：英文，代码简洁，理论严谨。
- 各头文件内的注释也有算法说明和参考文献。

**Q: 模板库更新了，怎么同步？**

使用 Git 子模块：`git submodule update --remote acm-champion`。使用复制目录则手动替换。推荐用子模块方式，便于跟踪更新。

---

## 下一步

1. 查看 `references/quick-index.md` 获取完整算法索引，了解每个文件的时间/空间复杂度。
2. 查看 `references/common-patterns.md` 掌握竞赛高频套路（倍增、CDQ、莫队等）。
3. 查看 `docs/algorithm-guide.md` 按问题类型检索算法。
4. 遇到具体问题时，先识别题型，再从对应目录选取算法模板。