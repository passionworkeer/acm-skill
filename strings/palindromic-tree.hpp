/**
 * Palindromic Tree (Eertree)  回文树
 *
 * A linear-time data structure for processing palindromic substrings.
 * Each node represents a distinct palindrome substring found in the input.
 *
 * Node structure:
 *   - len    : length of the palindrome represented by this node
 *   - fail   : failure link (longest proper palindromic suffix)
 *   - cnt    : number of occurrences of this palindrome (valid after build())
 *   - num    : number of palindromic suffixes ending at this position
 *   - next[] : transitions keyed by character (0-indexed alphabet)
 *
 * Two root nodes:
 *   node 0: len = -1 (odd-length super-root, always matches any character)
 *   node 1: len =  0 (empty string root)
 *
 * Usage:
 *   PalindromicTree pt(500000);
 *   pt.init();
 *   for (char ch : "abacaba") pt.addChar(ch - 'a');
 *   pt.build();
 *   cout << pt.getNodeCount() - 2 << endl;  // distinct non-empty palindromes
 *
 * Time complexity:  O(n) for building
 * Space complexity: O(n * ALPH)
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

/**
 * Node in the palindromic tree.
 *
 * len : length of the palindrome represented by this node
 * fail: index of the longest proper palindromic suffix (failure link)
 * cnt : number of occurrences of this palindrome (after build())
 * num : number of palindromic suffixes ending at this node
 * next: transition table; next[c] = node index for palindrome + char(c) + palindrome
 *       0 means no transition (only roots 0 and 1 can have link 0 meaning self)
 */
struct PTNode {
    int len;              // palindrome length
    int fail;             // failure link (suffix link)
    int cnt;              // occurrence count (valid after build())
    int num;              // number of suffix palindromes ending here
    int next[26];         // child links, 0 = undefined

    PTNode(int l = 0) : len(l), fail(0), cnt(0), num(0) {
        memset(next, 0, sizeof(next));
    }
};

/**
 * Palindromic Tree (Eertree) implementation.
 *
 * The tree is built incrementally, one character at a time.
 * After processing the full string, call build() to propagate cnt values.
 */
class PalindromicTree {
public:
    /**
     * Construct the Eertree with optional capacity reservation.
     * @param maxn maximum number of distinct palindromes (input length + 2 is safe)
     */
    explicit PalindromicTree(int maxn = 500000) {
        nodes.reserve(maxn);
        init();
    }

    /**
     * Initialize / reset the tree.
     * Creates two root nodes:
     *   node 0: len = -1 (odd super-root, handles odd-length extensions)
     *   node 1: len =  0 (even root, empty string)
     * Sets last = 1 (the empty-string node).
     */
    void init() {
        nodes.clear();
        // node 0: odd-length root, len = -1, fail = 0
        nodes.emplace_back(-1);
        nodes[0].fail = 0;
        // node 1: even-length root, len = 0, fail = 0 (root of fail tree)
        nodes.emplace_back(0);
        nodes[1].fail = 0;

        last = 1;
        n = 0;
        memset(s, 0, sizeof(s));
    }

    /**
     * Find the failure link for the node at position `x`.
     *
     * We need the longest suffix palindrome of the current string that can be
     * extended by the character at position n-1.  This means finding the first
     * node on the fail chain where s[n - len - 1] == s[n-1].
     *
     * @param x  current node index
     * @return   index of the node whose palindrome can be extended by s[n-1]
     */
    int getFail(int x) {
        // Walk up the failure chain until the character before this palindrome
        // matches the new character.  For super-root (len=-1), the "before"
        // index is n (out of bounds), which is always a valid match.
        while (n - nodes[x].len - 1 < 0 || s[n - nodes[x].len - 1] != s[n - 1]) {
            x = nodes[x].fail;
        }
        return x;
    }

    /**
     * Append character c (0..25) to the growing string and update the tree.
     *
     * Algorithm:
     *   1. Find the longest suffix palindrome cur that can be extended by c
     *      (via getFail from last).
     *   2. If no edge cur --c--> exists, create a new node newNode.
     *      - Its length is nodes[cur].len + 2.
     *      - Its fail link is found by extending getFail(cur)'s fail link.
     *   3. Move last to the matched node.
     *   4. Increment that node's direct occurrence count.
     *
     * @param c  character code 0..25 (e.g., 'a' -> 0, 'z' -> 25)
     */
    void addChar(int c) {
        s[n++] = c;                    // store character in buffer
        int cur = getFail(last);       // longest extensible suffix palindrome

        if (nodes[cur].next[c] == 0) {
            // No existing child: create a new palindrome node.
            PTNode newNode(nodes[cur].len + 2);

            // Find the failure link for the new palindrome:
            // start from cur's fail and walk until we find a palindrome
            // whose next[c] transition exists.
            int failNode = getFail(nodes[cur].fail);
            newNode.fail = nodes[failNode].next[c];
            if (newNode.fail == 0) newNode.fail = 1;  // default to empty-string root

            // num = number of suffix palindromes ending at this position
            newNode.num = nodes[cur].num + 1;

            // Add the transition edge
            nodes[cur].next[c] = (int)nodes.size();
            nodes.push_back(newNode);
        }

        last = nodes[cur].next[c];
        nodes[last].cnt++;             // one more occurrence at this node
    }

    /**
     * Propagate occurrence counts up the failure link tree.
     *
     * After addChar(), only nodes that are the terminal (last) node get cnt++.
     * This method accumulates those counts so that each node's cnt equals the
     * total number of occurrences of its palindrome in the string.
     *
     * Nodes are naturally processed in reverse creation order (longer first),
     * so we iterate from largest index down to 2 (skipping the two roots).
     */
    void build() {
        for (int i = (int)nodes.size() - 1; i >= 2; --i) {
            nodes[nodes[i].fail].cnt += nodes[i].cnt;
        }
    }

    /** Total number of nodes including the two roots. */
    int getNodeCount() const { return (int)nodes.size(); }

    /** Number of distinct non-empty palindromes = total_nodes - 2. */
    int distinctCount() const { return max(0, (int)nodes.size() - 2); }

    /** Index of the node for the longest suffix palindrome of processed prefix. */
    int last = 1;

    /** Character buffer storing the processed string as integers. */
    int s[500000];

    /** Current length of the processed string. */
    int n = 0;

private:
    vector<PTNode> nodes;
};

// -------------------------------------------------------------------------
// Example usage
// -------------------------------------------------------------------------
#ifdef PT_MAIN

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string str = "abacaba";
    PalindromicTree pt((int)str.size() + 5);

    pt.init();
    for (char ch : str) {
        pt.addChar(ch - 'a');
    }
    pt.build();

    cout << "String: \"" << str << "\"\n";
    cout << "Total nodes (incl. roots): " << pt.getNodeCount() << "\n";
    cout << "Distinct non-empty palindromes: " << pt.distinctCount() << "\n";

    // Distinct palindromes in "abacaba":
    //   Single chars:   a(4), b(2), c(1)        -> 3
    //   Length 3:       aba(2), aca(1)          -> 2
    //   Length 5:       bacab(1), cabac(1)       -> 2
    //   Total distinct: 7
    // Occurrence breakdown (a,b,c,aba,aca,bacab,cabac):
    //   a=4, b=2, c=1, aba=2, aca=1, bacab=1, cabac=1

    // Reconstruct each distinct palindrome by walking the tree.
    // Here we just demonstrate the API.
    cout << "\nPalindrome occurrences:\n";
    // In a full implementation, each node would also store its string value.
    // The count is available via node.cnt after build().
    cout << "  (reconstruct palindrome strings from tree edges for full listing)\n";

    return 0;
}

#endif