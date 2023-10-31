#include "common.hpp"

enum TokenType {
    NUMBER,
    OPERATOR,
    OPEN_PAREN,
    CLOSE_PAREN,
    INDEP_VARIABLE,
    MATH_FUNCTION
};

struct Token {
    TokenType type;
    std::string value;
    int precedence;
};