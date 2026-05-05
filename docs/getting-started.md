# ACM Champion 快速上手

ACM Champion 是一个面向 ACM/ICPC/XCPC 竞赛的冠军级算法模板库，包含 116 个经过竞赛验证的完整实现，涵盖图论、字符串、数学、数据结构、几何、动态规划等方向。

## 安装方式

将 `acm-champion` 目录复制到你的项目任意位置即可。所有文件使用 `#pragma once`，无需额外配置。

```cpp
// 直接包含即可使用
#include "acm-champion/graph-theory/dijkstra.hpp"
#include "acm-champion/strings/kmp.hpp"
```

## 快速上手：最短路问题

给定 n 个节点、m 条边的带权图，求从节点 0 到所有节点的最短距离。

```cpp
#include <bits/stdc++.h>
using namespace std;

#include "acm-champion/graph-theory/dijkstra.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<pair<int,int>>> g(n);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});  // 无向图
    }

    auto dist = dijkstra(g, 0);
    for (int i = 0; i < n; i++)
        cout << dist[i] << " ";
    return 0;
}
```

`dijkstra` 函数接收邻接表 `g` 和起点，返回到所有节点的最短距离数组。

## 快速上手：字符串匹配

给定模式串 `p` 和文本串 `t`，查找 `p` 在 `t` 中所有出现位置。

```cpp
#include <bits/stdc++.h>
using namespace std;

#include "acm-champion/strings/kmp.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string p, t;
    cin >> p >> t;

    vector<int> pi = prefix_function(p);
    for (int i = 0, j = 0; i < (int)t.size(); i++) {
        while (j > 0 && t[i] != p[j]) j = pi[j - 1];
        if (t[i] == p[j]) j++;
        if (j == (int)p.size()) {
            cout << i - j + 1 << endl;  // 匹配位置
            j = pi[j - 1];
        }
    }
    return 0;
}
```

`prefix_function` 计算 KMP 前缀函数，配合主循环完成匹配，时间复杂度 O(n + m)。

## 目录结构

```
acm-champion/
├── data-structures/      # 21 个文件：树状数组、线段树、Treap、LCT 等
├── graph-theory/         # 27 个文件：Dijkstra、Dinic、二分图匹配等
├── strings/             # 12 个文件：KMP、后缀自动机、AC 自动机等
├── math/                # 21 个文件：NTT、Min25 筛、Pollard-Rho 等
├── geometry/            # 9 个文件：凸包、半平面交、扫描线等
├── dynamic-programming/  # 5 个文件：斜率优化、数位 DP、状态压缩等
├── bit/                 # 3 个文件：XOR 基、子集枚举
├── misc/                # 8 个文件：大整数、表达式求值、快速幂等
└── references/          # 快速索引和常用模式文档
```

## 集成到自己的项目

### 方式一：直接复制

将 `acm-champion` 整个目录复制到你的代码仓库，直接 `#include` 对应头文件。所有文件独立无依赖，可以按需只复制需要的文件。

### 方式二：子模块

```bash
git submodule add https://github.com/passionworkeer/acm-skill acm-champion
```

然后在代码中包含：
```cpp
#include "acm-champion/graph-theory/dijkstra.hpp"
```

### 方式三：复制单文件

如果只需要某个算法，直接复制对应的 `.hpp` 文件到你的项目目录。所有文件只依赖 `<bits/stdc++.h>`，可独立编译。

## 下一步

- 查看 `references/quick-index.md` 获取完整算法索引
- 查看 `references/common-patterns.md` 了解竞赛常用套路
- 竞赛中先识别题型，再从对应目录选取算法模板
