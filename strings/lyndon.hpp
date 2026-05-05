/**
 * Lyndon Decomposition (Lyndon 分解)
 *
 * A Lyndon word is a non-empty string that is strictly smaller in
 * lexicographic order than all of its non-trivial suffixes.
 * Example: "a", "ab", "abb", "abc" are Lyndon words.
 *
 * Key theorem (Lyndon factorization uniqueness):
 *   Any string s can be uniquely written as w_1 w_2 ... w_k
 *   where each w_i is a Lyndon word and w_1 >= w_2 >= ... >= w_k.
 *   This is called the standard (lexicographically minimal) factorization.
 *
 * Algorithms:
 *   - duval()       : O(n) Duval algorithm; returns decomposition as strings
 *   - duvalIndices(): O(n); returns {start, end) pairs (avoids string copies)
 *   - minimumRepresentation(): O(n) Booth's algorithm; smallest rotation
 *
 * Applications:
 *   - Suffix array construction
 *   - Minimum lexicographic rotation
 *   - Pattern matching
 *   - Computing repetitions and periods
 *
 * Time complexity:  O(n) for all operations
 * Space complexity: O(n) for output; O(1) extra for computation
 *
 * Reference: Duval, J.-P. "Factorizing Words over an Ordered Alphabet."
 *            Journal of Algorithms, 1983.
 */

#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace Lyndon {

// ---------------------------------------------------------------------------
// Section 1: Lyndon Factorization (Duval Algorithm)
// ---------------------------------------------------------------------------

/**
 * Compute the standard Lyndon factorization of string s using Duval's algorithm.
 *
 * Algorithm (Duval 1983):
 *   Maintain three pointers i, j, k over the string s (0-indexed):
 *     i : start of the current candidate block
 *     j : next position being examined (start at i+1)
 *     k : position within the candidate block being compared with s[j]
 *
 *   While j < n and s[k] <= s[j]:
 *     if s[k] < s[j]:  reset k = i     (new, strictly smaller suffix)
 *     else:            ++k             (equal prefix, advance k)
 *     ++j
 *
 *   After the inner loop: the block s[i..j-1] has period p = j - k.
 *   The block is a Lyndon word and should be output (j - i) / p times.
 *   Then advance i by p and repeat.
 *
 * @param s  Input string
 * @return   Vector of strings representing the standard Lyndon factorization
 *           (s = result[0] + result[1] + ... + result[k])
 *
 * Example:
 *   duval("abababc")  = ["ab", "a", "b", "abc"]
 *   duval("aaaa")    = ["a", "a", "a", "a"]
 *   duval("abcabc")  = ["abc", "abc"]
 *   duval("abac")    = ["abac"]  (the whole string is already minimal)
 */
vector<string> duval(const string& s) {
    int n = (int)s.size();
    vector<string> result;
    result.reserve(n);  // at most n factors

    int i = 0;
    while (i < n) {
        int j = i + 1;   // start comparing from the next character
        int k = i;       // comparison offset within the candidate block

        // Extend j while s[k] <= s[j]; the candidate is s[i..j-1]
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) {
                // Strictly smaller: reset k to the start of the block
                k = i;
            } else {
                // Equal: advance k within the block
                ++k;
            }
            ++j;
        }

        // At this point, s[i..j-1] is the maximal candidate block.
        // Its minimal period is p = j - k.
        // Output the period p until we cover the entire block.
        int period = j - k;
        while (i < j) {
            result.emplace_back(s.substr(i, period));
            i += period;
        }
    }

    return result;
}

/**
 * Same algorithm as duval(), but returns indices {l, r} (half-open intervals).
 * More efficient for large strings as it avoids string copies.
 *
 * @param s  Input string
 * @return   Vector of pairs {start, end) for each factor
 */
vector<pair<int,int>> duvalIndices(const string& s) {
    int n = (int)s.size();
    vector<pair<int,int>> result;
    result.reserve(n);

    int i = 0;
    while (i < n) {
        int j = i + 1;
        int k = i;

        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) {
                k = i;
            } else {
                ++k;
            }
            ++j;
        }

        int period = j - k;
        while (i < j) {
            result.emplace_back(i, i + period);
            i += period;
        }
    }

    return result;
}

/**
 * Check if a string is a Lyndon word.
 * A string w is a Lyndon word iff w is strictly smaller than all of
 * its non-trivial suffixes.
 */
bool isLyndon(const string& w) {
    int n = (int)w.size();
    for (int suf = 1; suf < n; ++suf) {
        if (w.compare(0, n, w, suf, n - suf) >= 0) return false;
    }
    return true;
}

/**
 * Number of factors in the standard Lyndon factorization of s.
 */
int factorCount(const string& s) {
    int n = (int)s.size();
    int cnt = 0;
    int i = 0;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else ++k;
            ++j;
        }
        int period = j - k;
        while (i <= k) {
            ++cnt;
            i += period;
        }
    }
    return cnt;
}

// ---------------------------------------------------------------------------
// Section 2: Minimum Lexicographic Rotation
// ---------------------------------------------------------------------------

/**
 * Booth's algorithm: find the index of the lexicographically smallest
 * rotation of string s in O(n) time and O(1) extra space.
 *
 * Algorithm:
 *   Concatenate s with itself: t = s + s (length 2n)
 *   Use two pointers i and j with offset k:
 *     i = candidate start position 1
 *     j = candidate start position 2
 *     k = offset within the rotation being compared
 *   At each step:
 *     if t[i+k] == t[j+k]: ++k
 *     else if t[i+k] > t[j+k]: i = i + k + 1, skip all positions <= j
 *     else:                  j = j + k + 1, skip all positions <= i
 *   Answer: min(i, j) mod n
 *
 * @param s  Input string of length n
 * @return   Index (0..n-1) of the starting position of the minimum rotation
 */
int booth(const string& s) {
    if (s.empty()) return 0;
    string t = s + s;
    int n = (int)s.size();

    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        char a = t[i + k];
        char b = t[j + k];
        if (a == b) {
            ++k;
        } else if (a > b) {
            // Rotation at i is larger: skip it and all positions up to j+k+1
            i = i + k + 1;
            if (i <= j) i = j + 1;
            k = 0;
        } else {  // a < b
            // Rotation at j is larger: skip it and all positions up to i+k+1
            j = j + k + 1;
            if (j <= i) j = i + 1;
            k = 0;
        }
    }
    return min(i, j) % n;
}

/**
 * Compute the lexicographically smallest rotation of s.
 * Wrapper around booth() that returns the rotated string.
 *
 * @param s  Input string
 * @return   Smallest rotation of s (same length as s)
 */
string minimumRepresentation(const string& s) {
    if (s.empty()) return s;
    int pos = booth(s);
    string t = s + s;
    return t.substr(pos, (int)s.size());
}

/**
 * Alternative: compute minimum rotation using Duval on s + s.
 * Run duvalIndices() on t = s + s; the first position in the minimal
 * suffix that is < s and starts before n gives the answer.
 * (This is O(n) but booth() is simpler.)
 */
string minimumRepresentation_viaDuval(const string& s) {
    if (s.empty()) return s;
    string t = s + s;
    int n = (int)s.size();

    // Find the lexicographically smallest suffix of t among positions < n
    int best = 0;
    for (int i = 1; i < n; ++i) {
        if (t.compare(i, n, t, best, n) < 0) {
            best = i;
        }
    }
    return t.substr(best, n);
}

// ---------------------------------------------------------------------------
// Section 3: Utilities
// ---------------------------------------------------------------------------

/**
 * Verify the standard Lyndon factorization:
 *   1. concatenation equals the original string
 *   2. each factor is a Lyndon word
 *   3. factors are non-increasing (w_1 >= w_2 >= ... >= w_k)
 */
bool verify(const string& s, const vector<string>& factors) {
    string concat;
    concat.reserve(s.size());
    for (const auto& f : factors) concat += f;
    if (concat != s) return false;
    for (const auto& f : factors) {
        if (!isLyndon(f)) return false;
    }
    for (size_t i = 1; i < factors.size(); ++i) {
        if (factors[i - 1] < factors[i]) return false;
    }
    return true;
}

/**
 * Total length of the factorization (always equals |s|).
 */
int totalLength(const vector<string>& factors) {
    int sum = 0;
    for (const auto& f : factors) sum += (int)f.size();
    return sum;
}

}  // namespace Lyndon

// -------------------------------------------------------------------------
// Example usage
// -------------------------------------------------------------------------
#ifdef LYNDON_MAIN

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> tests = {
        "abababc",
        "aaaa",
        "abcabc",
        "abac",
        "ababa",
        "banana",
    };

    cout << "Lyndon Factorization Examples:\n";
    for (const string& s : tests) {
        auto factors = Lyndon::duval(s);
        cout << "  \"" << s << "\" = [";
        for (size_t i = 0; i < factors.size(); ++i) {
            cout << "\"" << factors[i] << "\"";
            if (i + 1 < factors.size()) cout << ", ";
        }
        cout << "]  (verified: " << (Lyndon::verify(s, factors) ? "OK" : "FAIL") << ")\n";
    }

    cout << "\nMinimum Rotation:\n";
    for (const string& s : tests) {
        cout << "  \"" << s << "\" -> \"" << Lyndon::minimumRepresentation(s) << "\"\n";
    }

    // Booth's algorithm
    cout << "\nBooth's index for \"abcab\": "
         << Lyndon::booth("abcab") << "  (expected: 2, rotation = \"cabab\")\n";

    return 0;
}

#endif