# CLAUDE.md — ACM Champion 算法库

## 项目概述

这是为 ACM/ICPC/XCPC 竞赛设计的冠军级算法模板库。
任何 AI agent 在读完全部内容后，应能独立完成竞赛级别的问题。

## 目录结构

```
acm-champion/
├── data-structures/    # 21 个文件：BIT、线段树、Treap、LCT、KD-Tree、莫队等
├── graph-theory/       # 27 个文件：所有最短路、流量、匹配、连通性、HLD 等
├── strings/           # 12 个文件：KMP、SAM、AC自动机、回文树等
├── math/              # 21 个文件：NTT、Min25筛、杜教筛、Pollard-Rho、组合数学
├── geometry/          # 9 个文件：凸包、半平面交、扫描线等
├── dynamic-programming/ # 5 个文件：斜率优化、数位DP、状态压缩等
├── bit/               # 3 个文件：XOR基、子集枚举
├── misc/              # 8 个文件：大整数、表达式计算、博弈论等
├── problems/          # 6 个文档：按算法分类的经典题解
└── references/        # 2 个文档：快速索引、常用模式
```

## 核心原则

1. **先识别题型，再选算法**：见 `references/quick-index.md`
2. **优先使用 O(n log n)**：n ≤ 2×10⁵ 通常安全
3. **模板直接使用**：所有代码 `#pragma once`，`#include <bits/stdc++.h>`
4. **先建模，再编码**：数学推导 > 盲目编码

## 必知高频模板

- `dijkstra.hpp`：最短路（竞赛最常见）
- `dinic.hpp`：最大流（难题标配）
- `segtree-lazy.hpp`：线段树（区间问题万能解）
- `suffix-automaton.hpp`：子串问题（字符串终极武器）
- `ntt.hpp`：多项式乘法（组合数学难题必备）
- `min25.hpp`：积性函数前缀和（数论进阶）
- `slope-optim.hpp`：DP 斜率优化（DP 难题标配）
- `manacher.hpp`：回文 O(n)（简洁高效）

## Skill 入口

Skill 主文件：`C:\Users\wang\.claude\skills\acm-champion\SKILL.md`

## 参考文档

- `references/quick-index.md` — 完整算法索引（文件名→复杂度→适用场景）
- `references/common-patterns.md` — 竞赛常用套路（倍增、分治、离线等）
- `problems/*.md` — 按类别组织的经典题解
