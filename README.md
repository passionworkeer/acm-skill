# ACM Champion 算法库

[![GitHub stars](https://img.shields.io/github/stars/passionworkeer/acm-skill)](https://github.com/passionworkeer/acm-skill)
[![License: MIT](https://img.shields.io/github/license/passionworkeer/acm-skill)](https://github.com/passionworkeer/acm-skill)

> 竞赛级算法模板库，面向 ACM/ICPC/XCPC 选手与 AI 辅助竞赛场景。开箱即用，100+ 模板覆盖图论、字符串、数论、数据结构、计算几何、动态规划等全方向。

---

## 为什么选择这个库

- **零配置**：所有代码 `#pragma once`，`#include <bits/stdc++.h>`，复制粘贴即可编译运行
- **全覆盖**：100+ 模板，从 Dijkstra 到 Link-Cut Tree，从 NTT 到杜教筛，从 SAM 到 Blossom，竞赛常考的一个不漏
- **按问题组织**：每份文档标注"适用场景"和"复杂度"，读完题目即可定位模板，无需大海捞针
- **文档先行**：配套速查表（文件名 → 复杂度 → 场景）、常用竞赛套路、按类别的经典题解
- **AI 就绪**：Claude Code 启动时自动加载 `.claude/CLAUDE.md`，读完即可独立完成竞赛级问题

---

## 目录结构

```
acm-champion/
├── bit/                     # XOR 线性基、子集/超集枚举
├── data-structures/         # 树状数组、线段树（普通/懒标记/持久化）
│   treap.hpp                # 分裂合并 Treap（有序集合、第 k 小）
│   lct.hpp                  # Link-Cut Tree（动态树：连边断边）
│   kd-tree.hpp              # KD-Tree（动态 2D 范围和）
│   mo-algorithm.hpp         # 莫队（离线区间询问）
│   dynamic-hull.hpp         # 动态凸包
│   monotonic-dp-hull.hpp   # 单调队列 DP 凸包
│   sliding-window.hpp       # 单调队列（滑动窗口最值）
│   pairing-heap.hpp         # 配对堆
│   sparse-table.hpp         # 稀疏表（RMQ，O(1) 查询）
│   union-find.hpp           # 并查集
├── graph-theory/            # Dijkstra、Bellman-Ford、最大流/费用流
│   dinic.hpp                # Dinic 最大流
│   mcmf.hpp                 # 最小费用最大流
│   hopcroft-karp.hpp        # Hopcroft-Karp 二分图最大匹配
│   blossom.hpp              # Blossom 一般图最大匹配
│   hungarian.hpp            # Hungarian KM 二分图最大权匹配
│   heavy-light.hpp          # 重链剖分（树上路径操作）
│   centroid-decomp.hpp      # 质心分解（树上分治）
│   virtual-tree.hpp         # 虚树（关键点路径问题）
│   dsu-on-tree.hpp          # DSU on Tree（子树询问）
│   tarjan-scc.hpp           # Tarjan 强连通分量
│   two-sat.hpp              # 2-SAT
│   euler-lca.hpp            # 欧拉序 LCA（O(1) 查询）
│   binary-lifting.hpp       # 二进制跳表 LCA
│   eulerian.hpp             # 欧拉回路/路径
│   bridges.hpp              # 割点与桥
├── strings/                 # KMP、AC 自动机、后缀数组/自动机
│   suffix-automaton.hpp     # 后缀自动机（子串问题终极武器）
│   ex-sam.hpp               # 扩展后缀自动机（多串统计）
│   palindromic-tree.hpp     # 回文树
│   manacher.hpp             # Manacher（回文 O(n)）
│   ac-automaton.hpp         # AC 自动机（多模式匹配）
│   suffix-array.hpp         # 后缀数组（O(n log n)）
│   suffix-array-linear.hpp  # 后缀数组（线性 DC3）
│   lyndon.hpp               # Lyndon 分解
├── math/                    # NTT、FFT、Min25、杜教筛、Pollard-Rho
│   ntt.hpp                  # NTT（模 998244353）
│   ntt-arbitrary.hpp        # 任意模数 NTT（三模数 CRT）
│   dujiao.hpp               # 杜教筛（φ/μ 前缀和）
│   min25.hpp                # Min25 筛（积性函数前缀和）
│   bsgs.hpp                 # BSGS（离散对数）
│   miller-rabin.hpp         # Miller-Rabin 质数判定
│   pollard-rho.hpp          # Pollard-Rho 大整数分解
│   lagrange.hpp             # 拉格朗日插值
│   stirling.hpp             # 斯特林数
│   catalan.hpp              # 卡特兰数
│   simplex.hpp              # 单纯形（线性规划）
│   gauss.hpp                # 高斯消元
│   polya.hpp                # Polya 计数
├── geometry/                # 凸包、旋转卡壳、半平面交、扫描线
│   point.hpp                # 点与向量基础
│   line.hpp                 # 直线与线段
│   polygon.hpp              # 多边形（面积、Pick 定理）
│   circle.hpp               # 圆
│   half-plane.hpp           # 半平面交
│   scanning-line.hpp        # 扫描线（矩形并面积）
│   convex-hull.hpp         # Andrew 凸包
├── dynamic-programming/     # 斜率优化、数位 DP、状态压缩、区间 DP
│   slope-optim.hpp          # 斜率优化 DP
│   state-compress.hpp       # 状态压缩 DP（n ≤ 20）
│   knapsack.hpp             # 背包 DP
│   interval-dp.hpp          # 区间 DP
├── misc/                    # 大整数、表达式求值、ModInt、博弈论
│   bigint.hpp               # 高精度整数
│   modint.hpp               # 模运算自动取模
│   custom-hash.hpp          # 防哈希攻击哈希函数
│   inversion-count.hpp      # 逆序数（BIT 实现）
├── problems/                # 按类别整理的经典题解（6 篇）
├── references/               # 快速索引 + 常用竞赛套路
└── docs/                    # 入门指南、算法手册、API 速查
```

---

## 快速入门

**方式一：复制单个文件**

找到所需模板，复制到本地，编译运行：

```cpp
#include <bits/stdc++.h>
using namespace std;
#pragma once
#include "dinic.hpp"

int main() {
    int n, m;  cin >> n >> m;
    Dinic dinic(n);
    for (int i = 0; i < m; i++) {
        int u, v, c;  cin >> u >> v >> c;
        dinic.addEdge(u, v, c);
    }
    cout << dinic.maxflow(1, n) << '\n';
}
```

**方式二：整个仓库为头文件目录**

```bash
git clone https://github.com/passionworkeer/acm-skill.git
```

添加 `-I acm-skill/` 编译，或在代码中写相对路径 include。

**方式三：Git Submodule（推荐团队使用）**

```bash
git submodule add https://github.com/passionworkeer/acm-skill.git libs/acm-champion
```

---

## 文档导航

| 文档 | 说明 |
|------|------|
| [docs/getting-started.md](docs/getting-started.md) | 安装配置、两道完整例题带你入门 |
| [docs/algorithm-guide.md](docs/algorithm-guide.md) | 按类别（图论 / 字符串 / 数学 / DS / 几何 / DP）梳理全部算法 |
| [docs/api-reference.md](docs/api-reference.md) | 10 个高频模板的 API 签名速查 |
| [references/quick-index.md](references/quick-index.md) | 完整速查表：文件名 → 时间复杂度 → 适用场景 |
| [references/common-patterns.md](references/common-patterns.md) | 竞赛常用套路：倍增、分治、离线、整体二分等 |
| [problems/*.md](problems/) | 按算法分类的经典题解（6 篇） |

---

## 贡献指南

欢迎提交 PR 或 Issue。主要方向：

- **补充缺失算法**：当前未覆盖但竞赛高频的模板（如 Dancing Links、支配树等）
- **题解扩充**：在 `problems/` 下补充更多经典题目的解题思路
- **Bug 修复与优化**：常数优化、边界 case 处理、接口改进
- **文档改进**：让文档更清晰、更易于检索

提交前请确保代码可编译（`g++ -std=c++17 -fsyntax-only`）。

---

## Star History

[![Star History](https://api.star-history.dev/svg?repos=passionworkeer/acm-skill&type=Public)](https://www.star-history.dev/#passionworkeer/acm-skill)