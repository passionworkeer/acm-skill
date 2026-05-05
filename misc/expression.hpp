#pragma once

#include <bits/stdc++.h>
using namespace std;

// ------------------------------------------------------------
// Expression evaluation: infix -> postfix -> evaluate
// Based on shunting-yard algorithm (Dijkstra)
//
// Supported operators:
//   +  -  *  /  ^  ( )
// Operands: integers (long long)
//
// Usage:
//   evaluate("3 + 4 * 2")               -> long long result
//   evaluate("3 + 4 * 2 / (1 - 5) ^ 2") -> long long result
//   toPostfix("3 + 4 * 2")              -> vector<string> postfix
//   evaluatePostfix(postfix)            -> long long result
// ------------------------------------------------------------
namespace Expression {

// ------------------------------------------------------------
// Helper functions
// ------------------------------------------------------------
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

bool isLeftAssociative(char op) {
    return op != '^';  // ^ is right-associative
}

// Convert number to string token
inline string numToStr(long long x) { return to_string(x); }

// ------------------------------------------------------------
// Tokenize infix expression into operand/operator tokens
// ------------------------------------------------------------
vector<string> tokenize(const string& s) {
    vector<string> tokens;
    int n = s.size();
    for (int i = 0; i < n; ) {
        if (isspace(s[i])) { ++i; continue; }
        if (isOperator(s[i]) || s[i] == '(' || s[i] == ')') {
            tokens.emplace_back(1, s[i]);
            ++i;
            continue;
        }
        if (isdigit(s[i])) {
            string t;
            while (i < n && (isdigit(s[i]) || s[i] == '.')) t.push_back(s[i++]);
            tokens.push_back(t);
            continue;
        }
        // treat any other char as part of identifier (for BigInt extension)
        string t;
        while (i < n && !isspace(s[i]) && !isOperator(s[i]) && s[i] != '(' && s[i] != ')') {
            t.push_back(s[i++]);
        }
        if (!t.empty()) tokens.push_back(t);
    }
    return tokens;
}

// ------------------------------------------------------------
// Infix to postfix (Reverse Polish Notation)
// Shunting-yard algorithm
// ------------------------------------------------------------
vector<string> toPostfix(const string& infix) {
    vector<string> tokens = tokenize(infix);
    vector<string> output;
    vector<char> opStack;

    for (const string& tok : tokens) {
        if (tok.size() == 1 && isdigit(tok[0])) {
            output.push_back(tok);
        } else if (tok.size() == 1 && isOperator(tok[0])) {
            char op = tok[0];
            while (!opStack.empty()) {
                char top = opStack.back();
                if (!isOperator(top)) break;
                bool topIsOp = true;
                if (topIsOp) {
                    if (isLeftAssociative(op)) {
                        if (precedence(top) >= precedence(op))
                            output.emplace_back(1, top), opStack.pop_back();
                        else break;
                    } else {
                        if (precedence(top) > precedence(op))
                            output.emplace_back(1, top), opStack.pop_back();
                        else break;
                    }
                }
            }
            opStack.push_back(op);
        } else if (tok == "(") {
            opStack.push_back('(');
        } else if (tok == ")") {
            while (!opStack.empty() && opStack.back() != '(') {
                output.emplace_back(1, opStack.back());
                opStack.pop_back();
            }
            if (!opStack.empty()) opStack.pop_back();  // pop '('
        } else {
            // operand (number or identifier)
            output.push_back(tok);
        }
    }
    while (!opStack.empty()) {
        output.emplace_back(1, opStack.back());
        opStack.pop_back();
    }
    return output;
}

// ------------------------------------------------------------
// Evaluate postfix expression
// Throws on division by zero
// ------------------------------------------------------------
long long evaluatePostfix(const vector<string>& postfix) {
    vector<long long> valStack;
    for (const string& tok : postfix) {
        if (tok.size() == 1 && isOperator(tok[0])) {
            if (valStack.size() < 2)
                throw std::runtime_error("Expression: not enough operands");
            long long b = valStack.back(); valStack.pop_back();
            long long a = valStack.back(); valStack.pop_back();
            long long r = 0;
            switch (tok[0]) {
                case '+': r = a + b; break;
                case '-': r = a - b; break;
                case '*': r = a * b; break;
                case '/':
                    if (b == 0) throw std::runtime_error("Expression: division by zero");
                    r = a / b; break;
                case '^': r = (long long)pow((long double)a, (int)b); break;
                default: throw std::runtime_error("Expression: unknown operator");
            }
            valStack.push_back(r);
        } else {
            // operand: parse as integer
            long long v = stoll(tok);
            valStack.push_back(v);
        }
    }
    if (valStack.size() != 1)
        throw std::runtime_error("Expression: malformed expression");
    return valStack.back();
}

// Convenience: direct infix evaluation
long long evaluate(const string& infix) {
    return evaluatePostfix(toPostfix(infix));
}

// ------------------------------------------------------------
// String representation of postfix (for debugging)
// ------------------------------------------------------------
string postfixToString(const vector<string>& pf) {
    string s;
    for (size_t i = 0; i < pf.size(); ++i) {
        s += pf[i];
        if (i + 1 < pf.size()) s += " ";
    }
    return s;
}

// ------------------------------------------------------------
// Extra: evaluate with variables (substitution map)
// ------------------------------------------------------------
using VarMap = unordered_map<string, long long>;

long long evaluateWithVars(const string& infix, const VarMap& vars) {
    vector<string> tokens = tokenize(infix);
    vector<string> output;
    vector<char> opStack;

    for (const string& tok : tokens) {
        if (tok.size() == 1 && isdigit(tok[0])) {
            output.push_back(tok);
        } else if (tok.size() == 1 && isOperator(tok[0])) {
            char op = tok[0];
            while (!opStack.empty()) {
                char top = opStack.back();
                if (!isOperator(top)) break;
                if (isLeftAssociative(op)) {
                    if (precedence(top) >= precedence(op))
                        output.emplace_back(1, top), opStack.pop_back();
                    else break;
                } else {
                    if (precedence(top) > precedence(op))
                        output.emplace_back(1, top), opStack.pop_back();
                    else break;
                }
            }
            opStack.push_back(op);
        } else if (tok == "(") {
            opStack.push_back('(');
        } else if (tok == ")") {
            while (!opStack.empty() && opStack.back() != '(') {
                output.emplace_back(1, opStack.back());
                opStack.pop_back();
            }
            if (!opStack.empty()) opStack.pop_back();
        } else if (vars.find(tok) != vars.end()) {
            output.push_back(to_string(vars[tok]));
        } else {
            output.push_back(tok);
        }
    }
    while (!opStack.empty()) {
        output.emplace_back(1, opStack.back());
        opStack.pop_back();
    }
    return evaluatePostfix(output);
}

// ------------------------------------------------------------
// Example / test harness
// ------------------------------------------------------------
#ifdef EXPRESSION_MAIN
int main() {
    vector<string> tests = {
        "3 + 4 * 2",
        "3 + 4 * 2 / (1 - 5) ^ 2",
        "(3 + 4) * 2",
        "2 ^ 3 ^ 2",         // right associative: 2^(3^2) = 2^9 = 512
        "10 / 2 - 3",
        "100 / (5 * (2 + 3))"
    };

    for (const string& expr : tests) {
        auto pf = toPostfix(expr);
        long long val = evaluatePostfix(pf);
        cout << expr << " = " << val << endl;
        cout << "  postfix: " << postfixToString(pf) << endl;
    }

    // Direct evaluate
    cout << endl << "Direct evaluate:" << endl;
    cout << "3 + 4 * 2 / (1 - 5) ^ 2 = "
         << evaluate("3 + 4 * 2 / (1 - 5) ^ 2") << endl;
    // 3 + 8 / (1-5)^2 = 3 + 8 / 16 = 3 + 0 = 3 (integer division)
    // But using long long division, should be 3

    // Variables
    cout << endl << "With variables:" << endl;
    VarMap vars = {{"x", 3}, {"y", 4}};
    cout << "x + y * 2 = "
         << evaluateWithVars("x + y * 2", vars) << endl;

    return 0;
}
#endif
}  // namespace Expression
