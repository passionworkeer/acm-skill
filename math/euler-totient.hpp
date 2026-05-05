#pragma once
#include <bits/stdc++.h>
using namespace std;

/*
 * 扩展欧拉函数 (Euler's Totient)
 *
 * phi(x)            : 单个数的欧拉函数（基于质因数分解）
 * phi_range(n)      : O(n log log n) 筛法，求 [1..n] 所有 phi
 * isPrime(n)        : 简单素性测试（试除到 sqrt(n)），适合小范围
 * eulerTotientSummatory(n) : Σ_{i=1}^{n} φ(i)，杜教筛优化到 O(n^{2/3})
 *
 * 基础性质：
 *   - φ(p) = p-1（p 为素数）
 *   - φ(p^k) = p^k - p^{k-1} = p^k * (1 - 1/p)
 *   - φ(a·b) = φ(a)·φ(b)（a,b 互质）
 *   - Σ_{d|n} φ(d) = n
 */

// ====================== 质因数分解（辅助） ======================

// 试验除法分解 n（n <= 1e12 可用）
vector<pair<long long,int>> factorize(long long n) {
    vector<pair<long long,int>> fac;
    for (long long p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) { n /= p; ++cnt; }
            fac.emplace_back(p, cnt);
        }
    }
    if (n > 1) fac.emplace_back(n, 1);
    return fac;
}

// ====================== 单个数欧拉函数 ======================

/*
 * phi(x) — 欧拉函数
 *
 * 基于质因数分解：phi(n) = n * Π_{p|n} (1 - 1/p)
 *
 * 时间复杂度: O(sqrt(n)) 试除
 * 适用于单次查询，大数时用 Pollard-Rho 替代
 */
long long phi(long long n) {
    if (n == 0) return 0;
    long long res = n;
    long long p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            res -= res / p; // res = res * (1 - 1/p)
        }
        if (p == 2) p = 3;
        else p += 2;
    }
    if (n > 1) res -= res / n; // 最后剩下的质因数
    return res;
}

// ====================== O(n log log n) 筛法 ======================

/*
 * phi_range(n)
 *
 * 计算 [1..n] 所有数的欧拉函数值。
 * 使用埃拉托斯特尼筛法变形：
 *   若 i 为质数，phi[i] = i - 1
 *   若 i % p == 0，phi[i*p] = phi[i] * p
 *   若 i % p != 0，phi[i*p] = phi[i] * (p-1)
 *
 * 返回 vector，phi[i] = 欧拉函数值（1-indexed，phi[0]=0）
 * 时间复杂度: O(n log log n)
 * 空间复杂度: O(n)
 */
vector<long long> phi_range(int n) {
    vector<long long> phi(n + 1);
    vector<int> primes;
    vector<bool> is_composite(n + 1, false);

    for (int i = 2; i <= n; ++i) {
        if (!is_composite[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int p : primes) {
            if (1LL * i * p > n) break;
            is_composite[i * p] = true;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p;      // p | i
                break;
            } else {
                phi[i * p] = phi[i] * (p - 1); // p ∤ i
            }
        }
    }
    phi[0] = 0;
    phi[1] = 1;
    return phi;
}

// ====================== 简单素性测试 ======================

/*
 * isPrime(n)
 *
 * 试除到 sqrt(n)，适合 n <= 1e12 量级。
 * 对于更大的数使用 Miller-Rabin。
 * 注意：2 是唯一的偶素数，单独处理。
 */
bool isPrime(long long n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long p = 3; p * p <= n; p += 2) {
        if (n % p == 0) return false;
    }
    return true;
}

// ====================== 杜教筛求前缀和 Σφ ======================

/*
 * eulerTotientSummatory(n)
 *
 * 计算 S(n) = Σ_{i=1}^{n} φ(i)
 *
 * 杜教筛优化：
 *   利用恒等式 Σ_{d|n} φ(d) = n
 *   => Σ_{i=1}^{n} Σ_{d|i} φ(d) = Σ_{d=1}^{n} φ(d) * floor(n/d)
 *   => Σ_{i=1}^{n} i = n(n+1)/2
 *   => S(n) = n(n+1)/2 - Σ_{d=2}^{n} S(floor(n/d))
 *
 * 复杂度: O(n^{2/3}) 预处理 O(n^{2/3}) 的哈希表
 * 对于 n = 1e11 仍可在 1s 内完成。
 *
 * 注意：返回值可能超过 64 位有符号范围，n <= 1e11 时安全。
 */
unordered_map<long long, __int128> _phi_summatory_cache;

long long eulerTotientSummatory(long long n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    static vector<long long> prefix;  // n^{2/3} 预计算的前缀
    static bool initialized = false;
    const long long M = (long long)pow(n, 2.0 / 3.0) + 10;

    if (!initialized) {
        // 预处理 [1..M] 的 φ 前缀和
        auto phiVals = phi_range((int)M);
        prefix.assign(M + 1, 0);
        for (long long i = 1; i <= M; ++i)
            prefix[i] = prefix[i - 1] + phiVals[i];
        initialized = true;
    }

    // 递归 + 记忆化
    function<long long(long long)> S = [&](long long x) -> long long {
        if (x <= M) return prefix[x];
        if (_phi_summatory_cache.find(x) != _phi_summatory_cache.end())
            return (long long)_phi_summatory_cache[x];

        long long ans = x * (x + 1) / 2; // Σ i

        for (long long l = 2, r; l <= x; l = r + 1) {
            r = x / (x / l);
            ans -= (r - l + 1) * S(x / l);
        }

        _phi_summatory_cache[x] = ans;
        return ans;
    };

    return S(n);
}

// ====================== 辅助：phi 前缀和（简化版，无缓存全局变量） ======================

/*
 * phiSummatoryNaive(n, phiArr)
 *
 * 基于预先计算的 phi_range 结果，返回 Σ_{i=1}^{n} φ(i)
 * phiArr 由 phi_range 提前计算得到。
 */
long long phiSummatoryNaive(int n, const vector<long long>& phiArr) {
    long long sum = 0;
    for (int i = 1; i <= n; ++i) sum += phiArr[i];
    return sum;
}

// ====================== 使用示例 ======================
/*
// 单个数
cout << phi(12);           // 4

// 批量
auto phiArr = phi_range(10);
for (int i = 1; i <= 10; ++i) cout << i << ":" << phiArr[i] << " ";
// 1:1 2:1 3:2 4:2 5:4 6:2 7:6 8:4 9:6 10:4

// 素性测试
isPrime(17); // true
isPrime(100); // false

// 前缀和（杜教筛）
cout << eulerTotientSummatory(1000000); // Σ_{i=1}^{10^6} φ(i)
*/