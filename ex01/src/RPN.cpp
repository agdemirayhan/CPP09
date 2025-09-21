#include "../include/RPN.hpp"
#include <stack>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iostream>

RPN::RPN() {}
RPN::RPN(const RPN&) {}
RPN& RPN::operator=(const RPN&) { return *this; }
RPN::~RPN() {}

static bool isOperator(const std::string& tok) {
    return tok.size() == 1 && (tok[0] == '+' || tok[0] == '-' || tok[0] == '*' || tok[0] == '/');
}

static long long applyOp(long long a, long long b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0)
                throw std::runtime_error("division by zero");
            return a / b;
    }
    throw std::runtime_error("invalid operator");
}

long long RPN::evaluate(const std::string& expr) {
    std::istringstream iss(expr);
    std::string token;
    std::stack<long long> st;

    while (iss >> token) {
        if (isOperator(token)) {
            if (st.size() < 2)
                throw std::runtime_error("not enough operands");
            long long b = st.top(); st.pop();
            long long a = st.top(); st.pop();
            st.push(applyOp(a, b, token[0]));
        } else {
            if (token.size() != 1 || !std::isdigit(token[0]))
                throw std::runtime_error("invalid token");
            st.push(token[0] - '0');
        }
    }
    if (st.size() != 1)
        throw std::runtime_error("invalid expression");
    return st.top();
}
