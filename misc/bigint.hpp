#pragma once

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// BigInt: arbitrary-precision decimal integer
//
// Storage: vector<int> digits, little-endian (digits[0] = LSD)
//          each digit in [0, 9]
// Max precision: MAXN base-10 digits (~4000 decimal digits is
//                more than enough for typical ACM use)
//
// Supports: + - * / %  comparison  stream I/O  string conversion
// ------------------------------------------------------------
class BigInt {
public:
    static const int BASE = 10;
    static const int MAXN = 1000;   // decimal digits

    vector<int> d;   // digits, d[0] = least significant
    int sign;         // 1 = positive, -1 = negative, 0 = zero

    // --------------------------------------------------------
    // Constructors
    // --------------------------------------------------------
    BigInt() : sign(0) { d.assign(1, 0); }

    explicit BigInt(long long x) { init(x); }

    explicit BigInt(int x) { init((long long)x); }

    explicit BigInt(const string& s) { init(s); }

    explicit BigInt(const char* s) { init(string(s)); }

    void init(long long x) {
        sign = (x == 0) ? 0 : (x > 0 ? 1 : -1);
        x = (x < 0) ? -x : x;
        d.clear();
        if (x == 0) { d.push_back(0); return; }
        while (x > 0) {
            d.push_back(int(x % BASE));
            x /= BASE;
        }
    }

    void init(const string& s) {
        d.clear();
        sign = 1;
        string t = s;
        if (!t.empty() && t[0] == '-') {
            sign = -1;
            t = t.substr(1);
        }
        if (!t.empty() && t[0] == '+') {
            sign = (t[0] == '-') ? -1 : 1;
            t = t.substr(1);
        }
        // remove leading zeros
        while (t.size() > 1 && t[0] == '0') t.erase(t.begin());
        if (t == "0" || t.empty()) {
            sign = 0; d.push_back(0); return;
        }
        for (int i = (int)t.size() - 1; i >= 0; --i) {
            d.push_back(t[i] - '0');
        }
    }

    // --------------------------------------------------------
    // clean(): remove leading zeros, maintain sign invariant
    // --------------------------------------------------------
    void clean() {
        while (!d.empty() && d.back() == 0) d.pop_back();
        if (d.empty()) { d.assign(1, 0); sign = 0; }
    }

    // --------------------------------------------------------
    // Comparison helpers
    // --------------------------------------------------------
    int compareAbs(const BigInt& o) const {
        if (d.size() != o.d.size()) {
            return d.size() < o.d.size() ? -1 : 1;
        }
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            if (d[i] != o.d[i])
                return d[i] < o.d[i] ? -1 : 1;
        }
        return 0;
    }

    bool isZero() const { return sign == 0; }

    // --------------------------------------------------------
    // Basic operators
    // --------------------------------------------------------
    BigInt operator+(const BigInt& o) const {
        BigInt r;
        if (sign == 0) { r = o; return r; }
        if (o.sign == 0) { r = *this; return r; }
        if (sign == o.sign) {
            r.sign = sign;
            r.d.clear();
            int n = max(d.size(), o.d.size());
            int carry = 0;
            for (size_t i = 0; i < n || carry; ++i) {
                int a = i < d.size() ? d[i] : 0;
                int b = i < o.d.size() ? o.d[i] : 0;
                int sum = a + b + carry;
                r.d.push_back(sum % BASE);
                carry = sum / BASE;
            }
        } else {
            // different signs: subtract the one with larger abs
            int cmp = compareAbs(o);
            if (cmp >= 0) {
                r = *this; r.sign = sign;
                r.subAbs(o);
            } else {
                r = o; r.sign = o.sign;
                r.subAbs(*this);
            }
        }
        r.clean();
        return r;
    }

    BigInt operator-(const BigInt& o) const {
        BigInt r;
        if (o.sign == 0) { r = *this; return r; }
        if (sign != o.sign) {
            r = o; r.sign = -o.sign;
            r.addAbs(o);
            return r;
        }
        int cmp = compareAbs(o);
        if (cmp >= 0) {
            r = *this; r.sign = sign;
            r.subAbs(o);
        } else {
            r = o; r.sign = -sign;
            r.subAbs(*this);
        }
        r.clean();
        return r;
    }

    BigInt operator*(const BigInt& o) const {
        BigInt r;
        if (sign == 0 || o.sign == 0) { r.sign = 0; r.d.assign(1,0); return r; }
        r.sign = (sign == o.sign) ? 1 : -1;
        r.d.assign(d.size() + o.d.size(), 0);
        for (size_t i = 0; i < d.size(); ++i) {
            for (size_t j = 0; j < o.d.size(); ++j) {
                r.d[i + j] += d[i] * o.d[j];
            }
        }
        // normalize
        int carry = 0;
        for (size_t i = 0; i < r.d.size(); ++i) {
            int cur = r.d[i] + carry;
            r.d[i] = cur % BASE;
            carry = cur / BASE;
        }
        while (carry) {
            r.d.push_back(carry % BASE);
            carry /= BASE;
        }
        r.clean();
        return r;
    }

    // Division: returns quotient, remainder via modDiv
    // Simple O(n^2) schoolbook division
    BigInt divMod(const BigInt& o, BigInt& rem) const {
        BigInt q, r;
        q.sign = (o.sign == 0) ? 0 : (sign == o.sign ? 1 : -1);
        r.sign = 0;
        if (o.isZero()) throw std::runtime_error("BigInt: division by zero");
        if (isZero()) { q.sign = 0; q.d.assign(1,0); rem.sign = 0; rem.d.assign(1,0); return q; }
        int cmp = compareAbs(o);
        if (cmp < 0) {
            q.sign = 0; q.d.assign(1,0);
            rem = *this;
            return q;
        }
        if (cmp == 0) {
            q.sign = (sign == o.sign) ? 1 : -1;
            q.d.assign(1,1);
            rem.sign = 0; rem.d.assign(1,0);
            return q;
        }
        // Long division: for each digit from most significant
        r.d.clear();
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            r.d.insert(r.d.begin(), d[i]);
            r.clean();
            // find digit
            int lo = 0, hi = BASE - 1;
            while (lo <= hi) {
                int mid = (lo + hi + 1) >> 1;
                BigInt t = o * BigInt(mid);
                if (t.compareAbs(r) <= 0) {
                    lo = mid;
                } else {
                    hi = mid - 1;
                }
            }
            q.d.insert(q.d.begin(), lo);
            BigInt sub = o * BigInt(lo);
            r = r - sub;
        }
        q.clean();
        rem = r;
        rem.sign = (q.sign == 0) ? 0 : 1;  // remainder sign follows |dividend|
        return q;
    }

    BigInt operator/(const BigInt& o) const {
        BigInt rem;
        return divMod(o, rem);
    }

    BigInt operator%(const BigInt& o) const {
        BigInt rem;
        divMod(o, rem);
        return rem;
    }

    // --------------------------------------------------------
    // Assignment operators
    // --------------------------------------------------------
    BigInt& operator+=(const BigInt& o) { return *this = *this + o; }
    BigInt& operator-=(const BigInt& o) { return *this = *this - o; }
    BigInt& operator*=(const BigInt& o) { return *this = *this * o; }
    BigInt& operator/=(const BigInt& o) { return *this = *this / o; }
    BigInt& operator%=(const BigInt& o) { return *this = *this % o; }

    // Unary minus
    BigInt operator-() const {
        BigInt r = *this;
        if (r.sign != 0) r.sign = -r.sign;
        return r;
    }

    // --------------------------------------------------------
    // Comparison operators
    // --------------------------------------------------------
    bool operator<(const BigInt& o) const {
        if (sign != o.sign) return sign < o.sign;
        int cmp = compareAbs(o);
        return cmp < 0;
    }
    bool operator>(const BigInt& o) const { return o < *this; }
    bool operator<=(const BigInt& o) const { return !(o < *this); }
    bool operator>=(const BigInt& o) const { return !(*this < o); }
    bool operator==(const BigInt& o) const { return sign == o.sign && compareAbs(o) == 0; }
    bool operator!=(const BigInt& o) const { return !(*this == o); }

    // Cast to bool
    explicit operator bool() const { return sign != 0; }

    // --------------------------------------------------------
    // String conversion
    // --------------------------------------------------------
    string str() const {
        if (isZero()) return "0";
        string s;
        if (sign == -1) s.push_back('-');
        for (int i = (int)d.size() - 1; i >= 0; --i) s.push_back(char('0' + d[i]));
        return s;
    }

    // --------------------------------------------------------
    // Stream operators
    // --------------------------------------------------------
    friend istream& operator>>(istream& is, BigInt& x) {
        string s;
        is >> s;
        x = BigInt(s);
        return is;
    }

    friend ostream& operator<<(ostream& os, const BigInt& x) {
        return os << x.str();
    }

    // --------------------------------------------------------
    // Power: x^p (p >= 0)
    // --------------------------------------------------------
    BigInt pow(long long p) const {
        if (p < 0) throw std::runtime_error("BigInt pow: negative exponent");
        BigInt base = *this;
        BigInt res(1);
        while (p > 0) {
            if (p & 1) res *= base;
            base *= base;
            p >>= 1;
        }
        return res;
    }

    // --------------------------------------------------------
    // Factorial: n! as BigInt
    // --------------------------------------------------------
    static BigInt factorial(int n) {
        BigInt res(1);
        for (int i = 2; i <= n; ++i) res *= BigInt(i);
        return res;
    }

    // --------------------------------------------------------
    // Helper arithmetic (internal, unsigned)
    // --------------------------------------------------------
    void addAbs(const BigInt& o) {
        int n = max(d.size(), o.d.size());
        while (d.size() < n) d.push_back(0);
        int carry = 0;
        for (size_t i = 0; i < n || carry; ++i) {
            int a = i < d.size() ? d[i] : 0;
            int b = i < o.d.size() ? o.d[i] : 0;
            int sum = a + b + carry;
            if (i < d.size()) d[i] = sum % BASE;
            else d.push_back(sum % BASE);
            carry = sum / BASE;
        }
    }

    void subAbs(const BigInt& o) {
        // assumes |this| >= |o|
        int carry = 0;
        for (size_t i = 0; i < d.size(); ++i) {
            int a = d[i];
            int b = i < o.d.size() ? o.d[i] : 0;
            int diff = a - b - carry;
            if (diff < 0) { diff += BASE; carry = 1; }
            else carry = 0;
            d[i] = diff;
        }
    }
};

// ------------------------------------------------------------
// Example / test harness
// ------------------------------------------------------------
#ifdef BIGINT_MAIN
int main() {
    BigInt a("123456789012345678901234567890");
    BigInt b(987654321);
    BigInt c = a * b;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a * b = " << c << endl;
    cout << "a + b = " << (a + b) << endl;
    cout << "a - b = " << (a - b) << endl;

    BigInt rem;
    BigInt q = a.divMod(b, rem);
    cout << "a / b = " << q << ", a % b = " << rem << endl;

    // factorial
    BigInt f = BigInt::factorial(100);
    cout << "100! = " << f << endl;

    // power
    BigInt p = BigInt(2).pow(100);
    cout << "2^100 = " << p << endl;

    // compare
    cout << boolalpha;
    cout << "a > b: " << (a > b) << endl;
    cout << "a < b: " << (a < b) << endl;

    // stream I/O
    BigInt x;
    cout << "Enter a BigInt: ";
    cin >> x;
    cout << "You entered: " << x << endl;

    return 0;
}
#endif
