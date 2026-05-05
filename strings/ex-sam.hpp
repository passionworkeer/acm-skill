/**
 * Extended Suffix Automaton (广义后缀自动机)
 *
 * Builds a Suffix Automaton (SAM) over multiple strings via a Trie.
 * This enables queries such as: "how many of the inserted strings
 * contain this substring?"
 *
 * Architecture:
 *   Trie       : stores all input strings; each node maps to a SAM state
 *   SAM        : recognizes all substrings of all inserted strings
 *   link tree  : a suffix-link DAG; traversing it upward enumerates
 *                all suffixes of every substring
 *
 * Key invariant:
 *   For every SAM state v, tr[v].len = longest string in its endpos class.
 *   For every character transition tr[v].next[c] = u, the string
 *   represented by u = any string of v + character c.
 *
 * Usage pattern:
 *   ExSAM::sam_init();
 *   ExSAM::trie_insert(s, n, id);    // repeat for each string, id = string index
 *   ExSAM::buildFromTrie();
 *   ExSAM::sam_count();
 *
 *   // Query: how many strings contain substring s?
 *   int v = ExSAM::follow("s");      // walk automaton; -1 if not found
 *   int ans = ExSAM::tr[v].cnt;      // number of strings
 *
 * Time complexity:  O(total_length) for construction
 * Space complexity: O(total_length)
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace ExSAM {

// ---------------------------------------------------------------------------
// Section 1: Configuration
// ---------------------------------------------------------------------------

const int MAXN = 500000;          // maximum total input length
const int MAXM = 2 * MAXN + 10;   // maximum SAM states (2n-1 is sufficient)

// ---------------------------------------------------------------------------
// Section 2: SAM Core
// ---------------------------------------------------------------------------

/**
 * SAM state.
 *
 * len : length of the longest string in the equivalence class of this state
 * link: suffix link (failure link); points to the state representing all
 *       proper suffixes of strings in this state
 * next: transition table; next[c] = state reached by appending char c (0..25)
 * cnt : number of input strings that have a substring corresponding to
 *       this state (valid after sam_count())
 */
struct SAMNode {
    int len;        // longest string length in this state
    int link;      // suffix link
    int next[26];  // transitions; -1 = undefined
    int cnt;       // number of strings containing a substring of this state

    SAMNode(int _len = 0) : len(_len), link(-1), cnt(0) {
        memset(next, -1, sizeof(next));
    }
};

SAMNode tr[MAXM];  // SAM states array
int samSize;       // current number of SAM states
int samLast;       // state of the longest suffix of the processed string

/**
 * Initialize the SAM with a single initial state (index 1; index 0 is unused
 * as a sentinel so that 0 can mean "null/undefined").
 *
 * After init, state 1 represents the empty string.
 */
void sam_init() {
    samSize = 1;              // state 1 is the first real state
    samLast = 1;              // empty string
    tr[samSize] = SAMNode(0); // state 1: length 0, link = -1
    tr[samSize].link = 0;     // root's suffix link points to "null" (0)
}

/**
 * Extend the SAM with character c from state `pre`, creating a new state.
 *
 * This is the standard SAM extension operation:
 *   1. Create cur with length = tr[pre].len + 1.
 *   2. Backtrack from pre, adding cur transitions for c until we find a
 *      state that already has a c-transition or reach the root (link=0).
 *   3. If we reached a state q with no c-transition: cur.link = 1.
 *   4. If q has a c-transition to r:
 *        - If tr[p].len + 1 == tr[r].len: cur.link = r.
 *        - Else: clone r (copy len, next, link; set cnt=0, len=p.len+1),
 *          redirect q's c-transition and its suffix-chain to the clone.
 *
 * @param c   character code 0..25
 * @param pre SAM state to extend from (the state representing the current
 *            string's suffix prefix in the Trie traversal)
 * @return    index of the newly created state (or an existing state if the
 *            transition already existed, which does not happen in the
 *            Trie-building scheme)
 */
int sam_extend(int c, int pre) {
    int cur = ++samSize;                    // new state
    tr[cur].len = tr[pre].len + 1;
    tr[cur].cnt = 1;                        // each new state represents one new endpos

    // Step 1: fill c-transitions backward through suffix links
    int p = pre;
    while (p != 0 && tr[p].next[c] == -1) {
        tr[p].next[c] = cur;
        p = tr[p].link;
    }

    if (p == 0) {
        // No state on the suffix chain had a c-transition:
        // the new substring is unique. Link to the empty-string state (1).
        tr[cur].link = 1;
        return cur;
    }

    int q = tr[p].next[c];
    if (tr[p].len + 1 == tr[q].len) {
        // Simple case: q is the direct extension of p.
        tr[cur].link = q;
        return cur;
    }

    // Complex case: need to clone q to preserve the invariant
    // that all transitions from states of length L lead to states with
    // length >= L+1.
    int clone = ++samSize;
    tr[clone] = tr[q];            // copy all fields from q
    tr[clone].len = tr[p].len + 1; // correct the length to p.len+1
    tr[clone].cnt = 0;            // clone does not represent a new endpos

    // Redirect all c-transitions from p and its suffix chain to the clone
    while (p != 0 && tr[p].next[c] == q) {
        tr[p].next[c] = clone;
        p = tr[p].link;
    }

    tr[q].link = tr[cur].link = clone;
    return cur;
}

/**
 * Propagate occurrence counts up through the suffix-link tree.
 *
 * After building, each state.cnt = 1 (each state is the terminal of one
 * substring in one input string).  This method adds those counts upward
 * so that tr[v].cnt = number of input strings that contain a substring
 * whose class is represented by state v.
 *
 * Implementation: bucket-sort states by len, then process in reverse order
 * (longest first), accumulating cnt to the suffix link.
 */
void sam_count() {
    // Find maximum length
    int maxLen = 0;
    for (int i = 1; i <= samSize; ++i) {
        if (tr[i].len > maxLen) maxLen = tr[i].len;
    }

    // Bucket sort by length
    vector<int> cnt(maxLen + 1, 0);
    for (int i = 1; i <= samSize; ++i) cnt[tr[i].len]++;
    for (int i = 1; i <= maxLen; ++i) cnt[i] += cnt[i - 1];

    // order[i] = state with length i (stable by index)
    vector<int> order(samSize + 1);
    for (int i = samSize; i >= 1; --i) {
        order[cnt[tr[i].len]--] = i;
    }

    // Process from longest to shortest (skip state 1 as it has link 0)
    for (int i = samSize; i >= 2; --i) {
        int v = order[i];
        tr[tr[v].link].cnt += tr[v].cnt;
    }
}

// ---------------------------------------------------------------------------
// Section 3: Trie
// ---------------------------------------------------------------------------

/**
 * Trie for storing multiple input strings.
 *
 * trie[pos][c] = index of the child node reached by character c (0..25),
 *                or -1 if no such child.
 * Each node also stores the SAM state index that corresponds to this prefix.
 *
 * Note: we use a flat array for the Trie (not vector of arrays) to match
 * the static array API expected by the problem.
 */
int trie[MAXM][26];  // trie[node][char] -> child node
int trieCnt;        // current number of Trie nodes
int trieSamState[MAXM];   // SAM state index for each Trie node
int trieStringId[MAXM];   // terminal string id for each Trie node

/**
 * Initialize the Trie with a single root node (index 0).
 */
void trie_init() {
    trieCnt = 0;
    memset(trie, -1, sizeof(trie));
    // Node 0 is the root; its SAM state will be set by buildFromTrie()
}

/**
 * Insert string s[1..n] into the Trie and mark it with identifier `pos`.
 *
 * The string is given as a C-style array with 1-based indexing (s[1]..s[n]).
 * `pos` is the index/id of this string (used in queries).
 *
 * @param s   character array, s[1]..s[n] are valid
 * @param n   length of the string
 * @param pos identifier for this string (0..k-1 or any integer)
 */
void trie_insert(char* s, int n, int pos) {
    int node = 0;  // start at root
    for (int i = 1; i <= n; ++i) {
        int c = s[i] - 'a';
        if (trie[node][c] == -1) {
            trie[node][c] = ++trieCnt;
            memset(trie[trieCnt], -1, sizeof(trie[0]));
        }
        node = trie[node][c];
    }
    trieStringId[node] = pos;  // mark terminal
}

// ---------------------------------------------------------------------------
// Section 4: Build SAM from Trie
// ---------------------------------------------------------------------------

/**
 * Build the SAM by traversing the Trie in BFS order.
 *
 * For each Trie node u, we have already computed its SAM state index
 * (trieSamState[parent]).  We extend the SAM from that state with the
 * character label on the edge parent --c--> u, obtaining a new SAM state
 * that we store as trieSamState[u].
 *
 * BFS order is critical: all suffix links of a node's children point to
 * states that were already created when we process the children.
 *
 * After all Trie nodes are processed, we call sam_count() to finalize
 * the occurrence counts.
 */
void buildFromTrie() {
    // BFS queue holds Trie node indices
    queue<int> q;
    q.push(0);

    // Root of Trie maps to SAM state 1 (empty string)
    trieSamState[0] = 1;

    while (!q.empty()) {
        int u = q.front(); q.pop();

        for (int c = 0; c < 26; ++c) {
            int v = trie[u][c];
            if (v == -1) continue;

            // Extend SAM from the parent state with character c
            int parentState = trieSamState[u];
            int newState = sam_extend(c, parentState);
            trieSamState[v] = newState;

            q.push(v);
        }
    }

    // After all states are created, propagate occurrence counts upward
    sam_count();
}

// ---------------------------------------------------------------------------
// Section 5: Query helpers
// ---------------------------------------------------------------------------

/**
 * Walk the SAM starting from state 1 (empty string) following the path
 * for string s (given as character codes 0..25).
 *
 * @param s    character array (1-indexed)
 * @param len  length of the query string
 * @return     SAM state reached, or -1 if the path does not exist
 */
int follow(int* s, int len) {
    int v = 1;  // start at root
    for (int i = 1; i <= len; ++i) {
        int c = s[i];
        if (c < 0 || c >= 26) return -1;
        if (tr[v].next[c] == -1) return -1;
        v = tr[v].next[c];
    }
    return v;
}

/**
 * Walk the SAM for a C-style string (null-terminated).
 * Returns -1 if the path does not exist.
 */
int follow(const char* s) {
    int v = 1;
    int i = 0;
    while (s[i]) {
        int c = s[i] - 'a';
        if (c < 0 || c >= 26) return -1;
        if (tr[v].next[c] == -1) return -1;
        v = tr[v].next[c];
        ++i;
    }
    return v;
}

/**
 * Query: how many of the inserted strings contain the substring
 * represented by SAM state v?
 *
 * @param v  SAM state (must be >= 1)
 * @return   number of strings that contain some string in state v's class
 */
int queryCount(int v) {
    if (v < 0) return 0;
    return tr[v].cnt;
}

}  // namespace ExSAM

// -------------------------------------------------------------------------
// Example usage: insert multiple strings and query containment
// -------------------------------------------------------------------------
#ifdef EXSAM_MAIN

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Build SAM over several strings
    ExSAM::sam_init();
    ExSAM::trie_init();

    // Insert strings (1-indexed char arrays for trie_insert)
    char s1[] = " abc";  // pad with dummy at index 0
    s1[0] = 0;
    strcpy(s1 + 1, "ababa");
    ExSAM::trie_insert(s1, 5, 0);

    char s2[] = " abc";
    strcpy(s2 + 1, "baba");
    ExSAM::trie_insert(s2, 4, 1);

    char s3[] = " abc";
    strcpy(s3 + 1, "aba");
    ExSAM::trie_insert(s3, 3, 2);

    char s4[] = " abc";
    strcpy(s4 + 1, "xyz");
    ExSAM::trie_insert(s4, 3, 3);

    // Build the SAM from the Trie
    ExSAM::buildFromTrie();

    // Queries: how many strings contain each substring?
    const char* queries[] = { "a", "ab", "aba", "ba", "bab", "xyz", "z" };
    cout << "Substring counts in the set {ababa, baca, aba, xyz}:\n";
    for (const char* q : queries) {
        int v = ExSAM::follow(q);
        if (v == -1) {
            cout << "  \"" << q << "\": not found in any string\n";
        } else {
            cout << "  \"" << q << "\": appears in " << ExSAM::queryCount(v)
                 << " string(s)\n";
        }
    }

    // Expected:
    //   "a":   3 (in s1,s2,s3)
    //   "ab":  3 (in s1,s2,s3)
    //   "aba": 3 (in s1,s2,s3)
    //   "ba":  3 (in s1,s2,s3)
    //   "bab": 1 (only in s1)
    //   "xyz": 1 (only in s4)
    //   "z":   not found

    return 0;
}

#endif