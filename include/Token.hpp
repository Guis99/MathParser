#include "common.hpp"
#include "QuickArray.hpp"

#pragma once

enum TokenType {
    NUMBER,
    OPERATOR,
    IDENTIFIER,
    OPEN_PAREN,
    CLOSE_PAREN,
    ASSIGN,
    CLOSE_ANGLE,
    OPEN_ANGLE
};

enum OperatorType {
    PLUS,
    MINUS,
    MULT,
    DIV,
    POW,
};

enum IdentifierType {
    VARIABLE,
    FUNCTION,
    CONSTANT
};

class Token {
    public:
    TokenType type;
    std::string name;
    virtual ~Token() {};
};

class Number : public Token {
    public:
    // TokenType type = TokenType::NUMBER;
    std::shared_ptr<MathParser::QuickArray> value;
};

class Operator : public Token {
    public:
    // TokenType type = TokenType::OPERATOR;
    OperatorType oper;
    int precedence; // 1,2, or 3
    bool LH; // left-associative; true when left-assoc, false when right-assoc
};

class Paren : public Token {
    public:
};

class Assign : public Token {
    public:
};

class AngleBracket : public Token {
    public:
};

class Identifier : public Token {
    public: 
    // TokenType type = TokenType::IDENTIFIER;
    IdentifierType idType;
};

class Variable : public Identifier {
    public:
    IdentifierType idType = IdentifierType::VARIABLE;
};

class Function : public Identifier {
    public:
    IdentifierType idType = IdentifierType::FUNCTION;
    int arity;
};

class Constant : public Identifier {
    public:
    IdentifierType idType = IdentifierType::CONSTANT;
};

