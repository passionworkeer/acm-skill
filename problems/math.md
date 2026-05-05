# 数学经典题解

> 题目特征 → 算法选择 → 核心思路

---

## 1. 数论基础

### 质数判定（Miller-Rabin）
**模板**：`miller-rabin.hpp`
**核心**：枚举几个固定底数（2,7,61 等），用费马小定理 + 二次探测，复杂度 O(k log n)

### 质因数分解（Pollard-Rho）
**模板**：`pollard-rho.hpp`
**核心**：伪随机序列找因子 + 递归分解；结合 Miller-Rabin 判定质数；大整数分解主力

### 质数筛
**模板**：`prime-sieve.hpp`
**核心**：线性筛 O(n)，同时预处理 phi/mu

### 组合数取模（大 n，模数为质数）
**算法**：Lucas 定理
**模板**：`lucas.hpp` + `binomial.hpp`
**核心**：`C(n,k) mod p = C(n%p, k%p) * Lucas(n/p, k/p) mod p`

### 中国剩余定理
**模板**：`crt.hpp`
**核心**：`x ≡ a_i (mod m_i)`，两两合并，注意检验 `gcd(m1,m2) | (a2-a1)`

---

## 2. 乘法逆元

### 单点逆元（费马小定理）
**模板**：`ex-gcd.hpp` 或 `fast-pow.hpp`
**核心**：`inv(a,p) = a^(p-2) mod p`（p 为质数），或 `exgcd` 扩展欧几里得

### 线性预处理阶乘逆元
**模板**：`binomial.hpp`
**核心**：`invFact[n] = (n!)^{-1} mod p`，`invFact[n-1] = invFact[n] * n % p`

---

## 3. 离散对数 / 高次剩余

### 离散对数（BSGS）
**模板**：`bsgs.hpp`
**核心**：`a^x ≡ b (mod p)`，令 `m=√p`，预处理 `a^{jm}` 表后枚举验证；O(√p log p)

### 原根
**模板**：`prime-sieve.hpp`
**核心**：分解 p-1，若 ∀q|p-1 有 `g^{(p-1)/q} ≠ 1 mod p`，则 g 为原根

### 二次剩余（Tonelli-Shanks）
**模板**：`math/simplex.hpp`（参考 quadratic-residue 部分）
**核心**：求 `x² ≡ n (mod p)`，p 为奇素数

---

## 4. FFT / NTT / 多项式

### 卷积（任意整数 mod）
**模板**：`fft.hpp`（浮点）或 `ntt.hpp`（整数）
**核心**：O(n log n) 计算多项式乘法；FFT 用复数，精度误差注意

### 数论变换（模数为特殊质数）
**模板**：`ntt.hpp`
**核心**：`mod = k*2^m+1`，原根 = 3；适用于 998244353 等

### 任意模数卷积
**模板**：`ntt-arbitrary.hpp`
**核心**：三模数 NTT + CRT 合并；可处理任意 ≤ 1e9 模数

### 多项式全家桶（求逆/求导/积分/exp/ln）
**模板**：`polynomial.hpp` + `ntt.hpp` + `polynomial-extra.hpp`（来自 kirainmoe exp-ln）
**核心**：`inv = Newton` 迭代；`ln = integrate(inv * derivative(f))`；`exp = Newton` 迭代

---

## 5. 筛法进阶

### Min25 筛（求积性函数前缀和）
**模板**：`min25.hpp`
**核心**：O(n^(2/3)) 求 Σ f(i)（素数部分用筛，素数以外递归）；适合 f(p^k) 已知

### 杜教筛（求 φ/μ 前缀和）
**模板**：`dujiao.hpp`
**核心**：`S(n) = Σ_{i=1}^n f(i)`；选辅助 g 使得 `f*g` 前缀和好求；μ 的前缀和用公式 `S(n)=1-Σ_{i=2}^n S(n/i)`

---

## 6. 组合数学

### 卡特兰数
**模板**：`catalan.hpp`
**公式**：`C_n = C(2n,n)/(n+1) = C(2n,n) - C(2n,n+1)`

### 斯特林数
**模板**：`stirling.hpp`
**应用**：集合划分 / 圆排列计数；通项公式用容斥

### Polya 计数
**模板**：`polya.hpp`
**公式**：`Ans = (1/|G|) Σ_{g∈G} m^{c(g)}`；求循环节数目用 gcd

---

## 7. 线性代数

### 高斯消元（模意义 / 浮点）
**模板**：`gauss.hpp`
**核心**：选主元（列主元或全主元），消元后回代；求行列式、秩、方程组解

### 矩阵快速幂
**模板**：`math/polynomial.hpp`（矩阵乘法）
**核心**：`A^n` 用二分快速幂；常用于线性递推（结合特征多项式）

### 单纯形（线性规划）
**模板**：`simplex.hpp`
**核心**：将 `max c·x` 转为标准形，O(n³ m) 求最优解；竞赛中求整数规划可行域

---

## 8. 常用套路

| 场景 | 套路 |
|------|------|
| 大组合数取模（质数）| Lucas + 阶乘逆元 |
| 大组合数取模（合数）| CRT 合并多个质数模数 |
| 线性递推第 K 项 | 矩阵快速幂 or 特征多项式（Kitamasa）|
| Σ 质数相关函数前缀和 | Min25 筛 |
| Σ φ 或 μ 前缀和 | 杜教筛 |
| 大整数分解 | Pollard-Rho + Miller-Rabin |
| 乘法群原根 | 枚举 + 质因数验证 |
