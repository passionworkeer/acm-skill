# CLAUDE.md — ACM Champion 算法库

## 项目概述

面向 ACM/ICPC/XCPC 竞赛的冠军级 C++ 算法模板库。107 个文件，覆盖图论（28）、字符串（12）、数学（22）、数据结构（22）、计算几何（9）、动态规划（6）及杂项，所有代码 `#pragma once`，`#include <bits/stdc++.h>`，复制即用。AI agent 读完本库内容后，应能独立完成竞赛级别的问题。

---

## 文件结构

```
acm-champion/
├── bit/                    3  # XOR 线性基、子集/超集枚举
├── data-structures/       22  # BIT、SegTree、Treap、LCT、KD-Tree、莫队等
├── graph-theory/          28  # 最短路、最大流/费用流、匹配、连通性、HLD 等
├── strings/               12  # KMP、SAM、AC 自动机、回文树、后缀数组等
├── math/                  22  # NTT、Min25、杜教筛、Pollard-Rho、组合数学等
├── geometry/               9  # 凸包、旋转卡壳、半平面交、扫描线等
├── dynamic-programming/    6  # 斜率优化、数位 DP、状态压缩、区间 DP
├── misc/                   8  # 大整数、表达式求值、博弈论、ModInt 等
├── problems/               6  # 按类别整理的经典题解
├── references/             2  # 快速索引 + 常用竞赛套路
└── docs/                   4  # 入门指南、算法手册、API 速查、使用流程
```

---

## 核心原则

1. **先识别题型，再选算法**：读 `references/quick-index.md` 确定文件名 → 复杂度 → 适用场景
2. **优先 O(n log n)**：n ≤ 2×10⁵ 通常安全；n ≤ 10⁷ 考虑 O(n) 双指针；n > 10¹² 查 Pollard-Rho / Min25
3. **数学推导优先于编码**：DP 方程、凸包条件、图建模写清楚再动手
4. **模板直接使用，不过度封装**：竞赛追求稳定可靠，零依赖零陷阱
5. **先建模，再编码**：先确定算法路径，再写代码

---

## 必知高频模板（Top 10）

| 模板 | 文件 | 解决什么问题 |
|------|------|-------------|
| Dijkstra | `graph-theory/dijkstra.hpp` | 非负权最短路（竞赛最高频）|
| Dinic | `graph-theory/dinic.hpp` | 最大流（难题标配）|
| 线段树（懒标记）| `data-structures/segtree-lazy.hpp` | 区间加乘求和（万能区间操作）|
| 后缀自动机 | `strings/suffix-automaton.hpp` | 子串/出现次数/不同子串数（字符串终极武器）|
| NTT | `math/ntt.hpp` | 多项式乘法 / 组合计数（数论难题必备）|
| 树状数组 | `data-structures/fenwick.hpp` | 单点更新 + 前缀和（简洁高效）|
| Treap | `data-structures/treap.hpp` | 有序集合、第 k 小、排名（比 set 更灵活）|
| 斜率优化 DP | `dynamic-programming/slope-optim.hpp` | 凸性 DP 优化（省一/省队级别 DP 题标配）|
| 二进制跳表 LCA | `graph-theory/binary-lifting.hpp` | 树上路径查询（树上问题基础组件）|
| Manacher | `strings/manacher.hpp` | 最长回文子串（O(n) 简洁高效）|

---

## 参考文档

| 文档 | 用途 |
|------|------|
| `references/quick-index.md` | 算法速查表：文件名 → 复杂度 → 适用场景 |
| `references/common-patterns.md` | 竞赛常用套路（倍增、分治、离线、整体二分）|
| `problems/*.md` | 按类别（图论/字符串/数学/DS/几何/DP）经典题解 |
| `docs/api-reference.md` | 10 个高频模板的 API 签名速查 |
| `docs/workflow.md` | 竞赛使用流程：读题 → 定位 → 编码 → 自检 |

---

## Skill 入口

`C:\Users\wang\.claude\skills\acm-champion\SKILL.md`

---

## 解题工作流

```
1. 读题 → 提取约束（n, m, mod）→ 判断数据规模
2. 识别题型 → 查 quick-index.md 定位算法
3. 建模 → 写出数学公式 / 图模型 / DP 方程（不动笔不动代码）
4. 选模板 → 复制文件到代码目录
5. 适配题目 → 改类名、加边、调整输入输出格式
6. 自测 → 用题目样例验证，检查边界 case（n=1、空图、负权）
7. 提交
```

**常见规模决策**：
- n ≤ 10⁷ → O(n) 或 O(n log n)
- n ≤ 2×10⁵ → O(n log n)，线段树 / BIT 安全
- n ≤ 20 → 状态压缩 DP，O(n·2ⁿ)
- n ≤ 500 → 矩阵乘法 / Floyd，O(n³)
- 组合数学 + NTT → 查 `ntt.hpp` / `ntt-arbitrary.hpp`
- 积性函数前缀和 → 查 `min25.hpp` / `dujiao.hpp`