// src/tools/calculator_eval.cpp — YOU IMPLEMENT (spec §4.6).
//
// calc_evaluate: evaluate a well-formed token list with two stacks
// (one for numbers, one for operators and '('). See spec §4.6 and the
// worked trace in Appendix C. std::stack or std::vector are both fine.

#include "tools/calculator.h"

#include <stack>

static bool apply(std::stack<double>& values, std::stack<CalcToken::Kind>& ops, std::string* error) {
    if (ops.empty() || values.size() < 2) {
        *error = "invalid expression";
        return false;
    }
    CalcToken::Kind op = ops.top();
    ops.pop();
    double right = values.top();
    values.pop();
    double left = values.top();
    values.pop();

    if (op == CalcToken::Kind::Plus) {
        values.push(left + right);
    } else if (op == CalcToken::Kind::Minus) {
        values.push(left - right);
    } else if (op == CalcToken::Kind::Times) {
        values.push(left * right);
    } else if (op == CalcToken::Kind::Divide) {
        if (right == 0.0) {
            *error = "division by zero";
            return false;
        }
        values.push(left / right);
    } else {
        *error = "invalid expression";
        return false;
    }
    return true;
}

bool calc_evaluate(const std::vector<CalcToken>& tokens, double* out, std::string* error) {
    std::stack<double> values;
    std::stack<CalcToken::Kind> ops;

    for (const CalcToken& t : tokens) {
        if (t.kind == CalcToken::Kind::Number) {
            values.push(t.value);
        } else if (t.kind == CalcToken::Kind::LParen) {
            ops.push(t.kind);
        } else if (t.kind == CalcToken::Kind::RParen) {
            while (!ops.empty() && ops.top() != CalcToken::Kind::LParen) {
                if (!apply(values, ops, error)) {
                    return false;
                }
            }
            if (ops.empty()) {
                *error = "invalid expression";
                return false;
            }
            ops.pop();
        } else {
            int current = 1;
            if (t.kind == CalcToken::Kind::Times || t.kind == CalcToken::Kind::Divide) {
                current = 2;
            }
            while (!ops.empty() && ops.top() != CalcToken::Kind::LParen) {
                int waiting = 1;
                if (ops.top() == CalcToken::Kind::Times || ops.top() == CalcToken::Kind::Divide) {
                    waiting = 2;
                }
                if (waiting < current) {
                    break;
                }
                if (!apply(values, ops, error)) {
                    return false;
                }
            }
            ops.push(t.kind);
        }
    }

    while (!ops.empty()) {
        if (ops.top() == CalcToken::Kind::LParen) {
            *error = "invalid expression";
            return false;
        }
        if (!apply(values, ops, error)) {
            return false;
        }
    }

    if (values.size() != 1) {
        *error = "invalid expression";
        return false;
    }
    *out = values.top();
    return true;
}