#include "common.hpp"

enum TokenType {
    NUMBER,
    OPERATOR,
    OPEN_PAREN,
    CLOSE_PAREN,
    VARIABLE,
    FUNCTION
};

enum class OperatorType : char {
    PLUS = '+',
    MINUS = '-',
    MULT = '*',
    DIV = '/',
    POW = '^',
    ASSIGN = '='
};

class Token {
    public:
    TokenType type;
    std::string name;
    virtual ~Token() {};
};

class Number : public Token {
    public:
    double value;
};

class Operator : public Token {
    public:
    OperatorType oper;
    int precedence; // 1,2, or 3
    bool LH; // left-associative; true when left-assoc, false when right-assoc

};

class Paren : public Token {
    public:
};

class Variable : public Token {
    public:
    std::string type;
};

class Scalar : public Variable {
    public:
    std::shared_ptr<double> contents;
};

class Vector : public Variable {
    public:
    std::shared_ptr<std::vector<double>> value;
};

class Function : public Token {
    public:
    int arity;
};

