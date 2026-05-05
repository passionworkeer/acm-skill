# 竞赛使用流程

> 如何用 ACM Champion 在比赛中快速解题

---

## 阶段一：赛前准备（5 分钟）

1. 把 `acm-champion/` 目录放在项目根目录
2. 或 `git submodule add <repo_url> acm-champion`
3. IDE 设置 include 路径：`C++ → Preprocessor → add acm-champion/`

---

## 阶段二：读题与算法定位（3-10 分钟）

### 第一步：识别题型大类

| 题面关键词 | 大类 |
|-----------|------|
| 最短路 / 最小生成树 / 路径 | 图论 |
| 匹配 / 连通分量 / 割点 | 图论 |
| 子串 / 回文 / 模式匹配 | 字符串 |
| 质数 / 组合数 / 前缀和（积性）| 数学 |
| 区间加 / 区间求和 / 第 K 小 | 数据结构 |
| 凸包 / 面积 / 点在多边形 | 计算几何 |
| 合并相邻 / 数字计数 / 集合遍历 | 动态规划 |

### 第二步：锁定具体算法

参考 `references/problem-types.md` 的「问题描述 → 算法」对照表。

常见陷阱：
- "无负边" → Dijkstra（别用 Bellman-Ford）
- "离线询问" → 莫队 or 持久化线段树
- "树上路径" → HLD（非 DFS 递归）
- "多模式" → AC 自动机（别用 KMP 跑多次）

---

## 阶段三：模板选取与代码拼装（5-15 分钟）

### 选模板
```cpp
// 按需 #include，不要一股脑全加
#include "acm-champion/graph-theory/dijkstra.hpp"
#include "acm-champion/data-structures/segtree-lazy.hpp"
```

### 改模板的注意事项

| 模板 | 常见修改点 |
|------|-----------|
| `dijkstra.hpp` | 检查点是 0-index 还是 1-index；确认是否有重边 |
| `dinic.hpp` | 确认 INF 设足够大（通常 1e18）；记得加反向边 |
| `segtree-lazy.hpp` | 注意 lazy 方向（是 += 还是 =）；更新 pushdown 顺序 |
| `kmp.hpp` | 确认模式串 pattern 是否从 0 开始匹配 |
| `manacher.hpp` | 奇回文/偶回文分开处理 |
| `ntt.hpp` | 仅支持 mod = 998244353；输入长度须为 2 的幂 |

### 调试技巧
- 模板代码不疑，疑题目建模
- 小数据手跑验证边界情况
- 重点关注：`l < r` vs `l <= r`、`0-index` vs `1-index`

---

## 阶段四：复杂度验证

| n 规模 | 必须达到的复杂度 |
|--------|------------------|
| ≤ 10⁶ | O(n log n) 或更好 |
| ≤ 2×10⁵ | O(n log n) |
| ≤ 5000 | O(n²) 可接受 |
| ≤ 20 | O(n·2ⁿ) 状态压缩 |
| 超大数 | 考虑数学公式 + NTT / 快速幂 |

---

## 阶段五：提交前自检

- [ ] 读了题目所有 subtask，确认算法对所有数据有效
- [ ] 数据类型用了 `long long`（n,m ≤ 10⁶ 时尤其注意）
- [ ] 树/图的 DFS 用了非递归或加大栈（`ios::sync_with_stdio(false)`）
- [ ] 模运算用了 `int64_t` 再取模，防止溢出
- [ ] 有负数时 Math 类的除法用了 `floorDiv` / `floorMod`

---

## 高频模板速查

```
最短路（非负）  →  graph-theory/dijkstra.hpp
最大流          →  graph-theory/dinic.hpp
树上路径        →  graph-theory/heavy-light.hpp
字符串匹配      →  strings/kmp.hpp （单）/ strings/ac-automaton.hpp （多）
子串查询        →  strings/suffix-automaton.hpp
区间操作        →  data-structures/segtree-lazy.hpp
有序集合        →  data-structures/treap.hpp
离线区间询问    →  data-structures/mo-algorithm.hpp
多项式乘法      →  math/ntt.hpp
积性函数前缀和  →  math/min25.hpp
DP 斜率优化     →  dynamic-programming/slope-optim.hpp
回文            →  strings/manacher.hpp
```

---

## 典型工坊流程（45 分钟模拟赛）

```
00:00 读 A、B 题，找签到题
05:00 A 题 AC，开始看 B
15:00 B 题 AC，分析 C（字符串 or 图论）
20:00 C 题思路确定，开始编码
40:00 C 题 AC，分析 D（数学 or DP）
50:00 D 题思路正确，编码中
70:00 D 题 AC，E/F 待分析
```

关键：前 3 题每题不超过 20 分钟。若超过，放弃，换下一题。