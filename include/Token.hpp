#include "common.hpp"
#include "QuickArray.hpp"

#pragma once

enum TokenType {
    NUMBER,
    OPERATOR,
    OPEN_PAREN,
    CLOSE_PAREN,
    ASSIGN,
    OPEN_ANGLE,
    CLOSE_ANGLE,
    OPEN_SQUARE,
    CLOSE_SQUARE,
    KEYWORD,
    COMMA,
    IDENTIFIER,
};

enum OperatorType {
    PLUS,
    MINUS,
    MULT,
    DIV,
    POW,
    OF,
};

enum IdentifierType {
    VARIABLE,
    FUNCTION,
    CONSTANT
};

enum FunctionType {
    BCONSTRUCT=3,
    INDEX=4,
    SINE=0,
    COSINE=1,
    TANGENT=2,
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
