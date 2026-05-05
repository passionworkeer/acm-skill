# ACM Champion 算法库

> 面向 ACM/ICPC/XCPC 冠军级竞赛的完整算法模板库。
> 任何 AI agent 在读完全部内容后，应能独立完成竞赛级别的问题。

[![GitHub stars](https://img.shields.io/github/stars/passionworkeer/acm-skill)](https://github.com/passionworkeer/acm-skill)
[![License: MIT](https://img.shields.io/github/license/passionworkeer/acm-skill)](https://github.com/passionworkeer/acm-skill)

---

## Git Tree

```
main ●── feat/initial               (116 files, 12K+ lines)
      │
      ├── data-structures/  (21)    # BIT · SegTree · Treap · LCT · KD-Tree · Mo
      │   ├── fenwick.hpp          # 树状数组
      │   ├── segtree*.hpp          # 普通/懒标记/持久化线段树
      │   ├── treap*.hpp            # 分裂合并/旋转版 Treap
      │   ├── lct.hpp              # Link-Cut Tree（动态树）
      │   ├── kd-tree.hpp          # KD-Tree（动态2D范围和）
      │   └── mo-algorithm.hpp     # 莫队（离线区间询问）
      │
      ├── graph-theory/    (27)    # 最短路 · 流量 · 匹配 · 连通性 · HLD
      │   ├── dijkstra.hpp         # 非负权最短路
      │   ├── dinic.hpp            # 最大流
      │   ├── mcmf.hpp             # 最小费用最大流
      │   ├── hopcroft-karp.hpp   # 二分图最大匹配
      │   ├── blossom.hpp          # 一般图最大匹配（开花算法）
      │   └── heavy-light.hpp     # 重链剖分（树上路径）
      │
      ├── strings/         (12)    # KMP · SAM · AC自动机 · 回文树
      │   ├── kmp.hpp              # 单模式匹配
      │   ├── ac-automaton.hpp     # 多模式匹配
      │   ├── suffix-automaton.hpp # 子串问题终极武器
      │   └── palindromic-tree.hpp # 回文树
      │
      ├── math/            (21)    # NTT · Min25 · 杜教筛 · Pollard-Rho
      │   ├── ntt.hpp               # 多项式乘法（模 998244353）
      │   ├── min25.hpp             # 积性函数前缀和
      │   ├── dujiao.hpp            # 杜教筛（φ/μ 前缀和）
      │   └── pollard-rho.hpp       # 大整数分解
      │
      ├── geometry/        (9)     # 凸包 · 旋转卡壳 · 半平面交 · 扫描线
      ├── dynamic-programming/(5) # 斜率优化 · 数位DP · 状态压缩 · 背包
      ├── bit/             (3)     # XOR基 · 子集枚举
      ├── misc/            (8)     # 大整数 · 表达式计算 · 博弈论
      ├── problems/        (6)     # 经典题解（按算法分类）
      └── references/      (2)     # 快速索引 · 常用模式
```

---

## 内容概览

| 类别 | 文件数 | 覆盖范围 |
|------|--------|---------|
| 数据结构 | 21 | BIT、线段树（含持久化/懒标记）、Treap、Link-Cut Tree、KD-Tree、莫队等 |
| 图论 | 27 | 所有最短路、最大流/费用流、二分图匹配、一般图匹配、HLD、虚树、DSU on Tree、SCC/双连通等 |
| 字符串 | 12 | KMP、Z函数、Manacher、AC自动机、Trie、后缀数组/自动机、回文树、扩展SAM、Lyndon |
| 数学 | 21 | NTT（三模数CRT）、FFT、Min25筛、杜教筛、Pollard-Rho/Miller-Rabin、BSGS、组合数学（Catalan/Stirling/Polya）、高斯消元、单纯形 |
| 计算几何 | 9 | 凸包、旋转卡壳、半平面交、扫描线、三维几何 |
| 动态规划 | 5 | 斜率优化DP、数位DP、状态压缩DP、区间DP、背包DP |
| 位运算 | 3 | XOR基、子集枚举 |
| 杂项 | 8 | 大整数、表达式计算、博弈论、快速IO |

---

## 快速决策

**图论问题** → `graph-theory/`
- 最短路 → `dijkstra.hpp`
- 最大流 → `dinic.hpp`
- 二分匹配 → `hopcroft-karp.hpp`
- 一般图匹配 → `blossom.hpp`
- 树上路径 → `heavy-light.hpp`
- 动态树 → `lct.hpp`

**字符串问题** → `strings/`
- 单模式匹配 → `kmp.hpp`
- 多模式匹配 → `ac-automaton.hpp`
- 子串/回文 → `suffix-automaton.hpp` / `manacher.hpp`

**数学问题** → `math/`
- 卷积 → `ntt.hpp`
- 大数分解 → `pollard-rho.hpp`
- 积性函数前缀和 → `min25.hpp` / `dujiao.hpp`
- 组合计数 → `catalan.hpp` / `stirling.hpp`

**数据结构问题** → `data-structures/`
- 区间和/单点更新 → `fenwick.hpp`
- 区间操作 → `segtree-lazy.hpp`
- 有序集合 → `treap.hpp`
- 离线区间询问 → `mo-algorithm.hpp`

---

## 复杂度参考

| n 上界 | 推荐复杂度 |
|--------|-----------|
| 10 | O(n!)、暴力枚举 |
| 20 | O(n·2ⁿ)，状态压缩 |
| 500 | O(n³)，矩阵乘法 |
| 2000 | O(n²)，Floyd |
| 2×10⁵ | O(n log n)，线段树 |
| 10⁷ | O(n)，双指针 |
| 10¹² | O(n^(1/3))，Pollard-Rho |

---

## 使用方法

```cpp
#include <bits/stdc++.h>
using namespace std;

// 直接使用模板
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Fenwick BIT(n);
    Dijkstra dij(n);
    // ...
}
```

---

## 参考文档

- `references/quick-index.md` — 完整算法速查表
- `references/common-patterns.md` — 常用竞赛套路
- `problems/*.md` — 按类别经典题解

## Skill 入口

`C:\Users\wang\.claude\skills\acm-champion\SKILL.md`
