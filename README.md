# ACM Champion 算法库

> 面向 ACM/ICPC/XCPC 冠军级竞赛的完整算法模板库。
> 任何 AI agent 在读完全部内容后，应能独立完成竞赛级别的问题。

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
