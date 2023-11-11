#include "common.hpp"
#include "QuickArray.hpp"

#pragma once

enum TokenType {
    NUMBER=1,
    OPERATOR=2,
    ASSIGN=3,
    OPEN_PAREN=4,
    CLOSE_PAREN=5,
    OPEN_ANGLE=6,
    CLOSE_ANGLE=7,
    OPEN_SQUARE=8,
    CLOSE_SQUARE=9,
    KEYWORD=10,
    COMMA=11,
    NEWLINE=12,
    IDENTIFIER=13,
};

enum OperatorType {
    PLUS,
    MINUS,
    MULT,
    DIV,
    POW,
    OF,
    NEGATE
};

enum IdentifierType {
    VARIABLE,
    FUNCTION,
    CONSTANT
};

enum FunctionType {
    BCTOR,
    INDEX,
    COMPOSE,
    SUM,
    NORM,
    SHAPE,
    RANGE,
    LINSPACE
};

struct RawToken {
    std::string name;
    int type;
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
    std::shared_ptr<MathParser::QuickArray<double>> value;
};

class Operator : public Token {
    public:
    // TokenType type = TokenType::OPERATOR;
    OperatorType oper;
    int precedence; // 1,2,3, or 4
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
    void setIdType(IdentifierType idType) {
        this->idType = idType;
    }
    void setFuncName(IdentifierType idType) {
        this->idType = idType;
    }
    void setArity(IdentifierType idType) {
        this->idType = idType;
    }

    IdentifierType idType;
    FunctionType functionName;
    int arity;
};
