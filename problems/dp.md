# 动态规划经典题解

> 题目特征 → 算法选择 → 核心思路

---

## 1. 基础 DP

### 线性 DP（爬楼梯、Fibonacci）
**模板**：无特定文件，直接写转移
**核心**：`dp[i] = min/max(dp[i-1]+cost, dp[i-2]+cost)`

### 区间 DP
**模板**：`interval-dp.hpp`
**核心**：`dp[l][r] = min_{k} dp[l][k] + dp[k+1][r] + mergeCost(l,r)`
**典型题**：合并石子、括号序列、凸多边形三角剖分
**循环方向**：`for len = 2..n; for l = 1..n-len+1; r = l+len-1`

### 数位 DP
**模板**：`digit-dp.hpp`
**核心**：`dfs(pos, state, lead, limit)`，记忆化搜索
**典型题**：不含某数字的数数量、数字和模 K 问题
**注**：只有 `limit==0 && lead==0` 时才能记忆化

### 状态压缩 DP
**模板**：`state-compress.hpp`
**核心**：`dp[mask][i]` = 到达状态 mask、停在 i 的最优值；`mask` ≤ 2^n
**典型题**：TSP（n≤20）、集合划分、枚举子集
**转移**：`nxt = mask | (1<<j); dp[nxt][j] = min(dp[nxt][j], dp[mask][i] + w[i][j])`

---

## 2. 背包 DP

### 0-1 背包
**模板**：`knapsack.hpp`
**核心**：`for i=n..1: dp[w] = max(dp[w], dp[w-weight[i]]+value[i])`（逆序）

### 完全背包
**核心**：顺序循环（可重复选），`for i=1..n: dp[w] = max(dp[w], dp[w-weight[i]]+value[i])`

### 多重背包（单调队列优化）
**模板**：`knapsack.hpp` → `multiPackOptim()`
**核心**：按余数分组，滑动窗口最值；O(n·W)

### 分组背包
**核心**：每组至多选一件；`for group: for w=cap..0: for item in group: dp[w]=max(dp[w],dp[w-wi]+vi)`

---

## 3. 树形 DP

### 树上独立集 / 最大权独立集
**模板**：`graph-theory/dsu-on-tree.hpp` 或直接 DFS
**核心**：`dp[u][0/1]` = 选/不选 u 时子树最优；`dp[u][0] += max(dp[v][0], dp[v][1])`

### 树链剖分 + DP
**模板**：`heavy-light.hpp` + `segtree-lazy.hpp`
**核心**：把树链映射到序列上，区间 DP

---

## 4. 斜率优化 DP

### 适用条件
**题目特征**：`dp[i] = min_{j < i} dp[j] + cost(j,i)`，其中 cost 可写成 `(Y_j - Y_k) / (X_j - X_k)` 形式
**模板**：`slope-optim.hpp`
**核心**：维护凸包（下凸），队首弹出无效决策，二分或指针查找最优

**判断凸壳单调性**：
- 斜率单调递增 + x 单调 → 指针 O(n)
- 斜率单调递增 + x 不单调 → 二分 O(n log n)
- 斜率不单调 → CDQ 分治 or平衡树维护凸包

---

## 5. 单调队列优化 DP

### 适用条件
**题目特征**：`dp[i] = min_{i-k ≤ j < i} dp[j] + w(j+1..i)`，转移区间滑动
**模板**：`data-structures/sliding-window.hpp`
**核心**：转移区间 `[i-K, i-1]`，维护 dp[j]-someFunction(j) 的最小值

---

## 6. 矩阵加速

### 线性递推快速幂
**模板**：`polynomial.hpp`（矩阵乘法部分）
**核心**：构造转移矩阵 M，`ans = M^(n-1) * init`；O(k³ log n)
**典型题**：Fibonacci 第 n 项、任意线性递推

### Kitamasa（特征多项式）
**模板**：`polynomial.hpp`
**核心**：O(k² log n) 求线性递推，比矩阵乘法更快（k = 递推阶数）

---

## 7. DP 优化总览

| 条件 | 优化方法 | 复杂度 |
|------|---------|--------|
| 转移区间固定滑动 | 单调队列 | O(n) |
| 转移区间不固定，但 cost 可表示为上/下凸包 | 斜率优化（CDQ 或直接）| O(n log n) |
| x 单调递增，斜率单调 | 斜率优化（指针）| O(n) |
| 转移依赖 O(k) 历史 | 矩阵乘法 | O(k³ log n) |
| 转移依赖分治序 | CDQ 分治 DP | O(n log n) |
| 线性递推（k 阶）| Kitamasa | O(k² log n) |

---

## 8. 博弈论 DP

### SG 函数
**模板**：`simplex-game.hpp`
**核心**：`sg[x] = mex{ sg[y] for y in next(x) }`；多游戏 SG = 各子游戏 Nim 和
**典型题**：Nim 堆、独立游戏组合

### Grundy 递推打表
**核心**：预处理 SG 值到 N，然后按 SG=0 判定必败态
