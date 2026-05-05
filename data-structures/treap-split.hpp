#pragma once
#include <bits/stdc++.h>
using namespace std;

/*
 * Split-Merge Treap (按关键码分裂的Treap)
 *
 * Node.key    : 关键码（用于分裂/排序）
 * Node.prior  : 随机优先级（决定树形）
 * Node.size   : 子树节点数
 *
 * split(t, key, l, r)  : l 含 key < key 的节点，r 含 key >= key 的节点
 * merge(l, r)          : 合并两棵，priority 大的做根
 * insert / erase        : 基于 split+merge 的插入/删除
 * kth(k)                : 第 k 小（1-indexed）
 * rank(key)             : key 的排名（1-indexed）
 * inorder(out)          : 中序遍历收集
 *
 * 时间复杂度: 期望 O(log n)
 * 空间复杂度: O(n)
 */

// ====================== Node 定义 ======================

struct Node {
    int key;           // 关键码
    int prior;         // 随机优先级
    int size;          // 子树大小
    Node *l, *r;       // 左右孩子

    Node(int _key) : key(_key), prior(rand()), size(1), l(nullptr), r(nullptr) {}
};

// ====================== 辅助操作 ======================

// 更新节点 size（左右子树大小 + 自身）
void update(Node* t) {
    if (!t) return;
    t->size = 1;
    if (t->l) t->size += t->l->size;
    if (t->r) t->size += t->r->size;
}

// 获取子树大小（空树返回 0）
int getSize(Node* t) {
    return t ? t->size : 0;
}

// ====================== 核心分裂合并 ======================

/*
 * split(t, key, l, r)
 *
 * 将树 t 按关键码 key 分裂为 l 和 r 两部分：
 *   l: 所有 key < key 的节点
 *   r: 所有 key >= key 的节点
 *
 * 如果 key 相等时希望把等于 key 的分到左边，
 * 可以在外层用 (key+1) 分裂后再手动处理。
 */
void split(Node* t, int key, Node*& l, Node*& r) {
    if (!t) {
        l = r = nullptr;
        return;
    }

    if (t->key < key) {
        // 当前节点属于左半部分，递归分裂右子树
        split(t->r, key, t->r, r);
        l = t;
        update(l);
    } else {
        // 当前节点属于右半部分，递归分裂左子树
        split(t->l, key, l, t->l);
        r = t;
        update(r);
    }
}

/*
 * merge(l, r)
 *
 * 合并两棵满足以下条件的树：
 *   l 中所有 key < r 中所有 key
 * 取 prio 较大的作为新根，递归合并另一子树。
 */
Node* merge(Node* l, Node* r) {
    if (!l || !r) return l ? l : r;

    if (l->prior > r->prior) {
        l->r = merge(l->r, r);
        update(l);
        return l;
    } else {
        r->l = merge(l, r->l);
        update(r);
        return r;
    }
}

// ====================== 基本操作 ======================

/*
 * insert(t, it)
 *
 * 将新节点 it 插入 treap t。
 * 实现：按 it->key 分裂，再依次合并。
 */
Node* insert(Node* t, Node* it) {
    if (!t) return it;

    if (it->prior > t->prior) {
        // it 作为新的根，原树按 it->key 分裂
        split(t, it->key, it->l, it->r);
        update(it);
        return it;
    } else if (it->key < t->key) {
        t->l = insert(t->l, it);
        update(t);
        return t;
    } else {
        t->r = insert(t->r, it);
        update(t);
        return t;
    }
}

/*
 * erase(t, key)
 *
 * 从 treap t 中删除所有 key 的节点（精确匹配）。
 * 实现：递归深入，找到后 merge 左右子树替换。
 */
Node* erase(Node* t, int key) {
    if (!t) return nullptr;

    if (t->key == key) {
        Node* res = merge(t->l, t->r);
        delete t;               // 释放节点（实际使用时注意内存管理）
        return res;
    } else if (key < t->key) {
        t->l = erase(t->l, key);
    } else {
        t->r = erase(t->r, key);
    }
    update(t);
    return t;
}

/*
 * kth(t, k)
 *
 * 返回第 k 小的节点指针（1-indexed）。
 * 越界返回 nullptr。
 */
Node* kth(Node* t, int k) {
    if (!t || k < 1 || k > t->size) return nullptr;

    int leftSize = getSize(t->l);

    if (k <= leftSize) {
        return kth(t->l, k);
    } else if (k == leftSize + 1) {
        return t;
    } else {
        return kth(t->r, k - leftSize - 1);
    }
}

/*
 * rank(t, key)
 *
 * 返回 key 在树中的排名（严格小于 key 的元素数 + 1）。
 * 即 key 是第几小（1-indexed）。
 * 若 key 不存在，返回其应该插入的位置 + 1。
 */
int rank(Node* t, int key) {
    if (!t) return 1;

    if (key <= t->key) {
        return rank(t->l, key);
    } else {
        return getSize(t->l) + 1 + rank(t->r, key);
    }
}

/*
 * inorder(t, out)
 *
 * 中序遍历 treap t，将所有关键码依次 push 到 out。
 * 结果按关键码升序排列。
 */
void inorder(Node* t, vector<int>& out) {
    if (!t) return;
    inorder(t->l, out);
    out.push_back(t->key);
    inorder(t->r, out);
}

// ====================== 便捷包装 ======================

// 在外层方便使用的 insert（传入 key，自动构造 Node）
Node* treapInsert(Node* t, int key) {
    return insert(t, new Node(key));
}

// 外层方便使用的 erase（传入 root 指针的引用，自动更新）
void treapErase(Node*& t, int key) {
    t = erase(t, key);
}

// 查找 key 是否存在
bool treapFind(Node* t, int key) {
    while (t) {
        if (key == t->key) return true;
        t = (key < t->key) ? t->l : t->r;
    }
    return false;
}