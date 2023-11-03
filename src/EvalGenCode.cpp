#include "../include/MathParser.hpp"

namespace MathParser {
    std::unordered_map<std::string, std::function<QuickArray(QuickArray, QuickArray)>> operators;
}

MathParser::QuickArray MathParser::ParseText(std::string inputString) {
    bool isAssign = false; size_t stopIdx = 0;
    std::vector<std::shared_ptr<Token>> tokens;
    MathParser::TokenizeString(inputString, tokens, isAssign, stopIdx);

    std::vector<std::shared_ptr<Token>> parsedTokens;
    MathParser::QuickArray result;

    if (!isAssign) {
        parsedTokens = MathParser::ShuntingYard(tokens);
        result = MathParser::EvalReversePolishToks(parsedTokens)[0];
    }
    else {
        std::vector<std::shared_ptr<Token>> slicedTokens(tokens.begin() + stopIdx, tokens.end());

        parsedTokens = MathParser::ShuntingYard(slicedTokens);
        result = MathParser::EvalReversePolishToks(parsedTokens)[0];

        std::string varName = tokens[stopIdx-1]->name;
        MathParser::SetVariable(varName, result);
    }
    return result;  
}

void MathParser::InitMaps() {
    // Load operators
    std::function<QA(QA, QA)> add = QA::operator+;
    std::function<QA(QA, QA)> sub = QA::operator-;
    std::function<QA(QA, QA)> mult = QA::operator*;
    std::function<QA(QA, QA)> div = QA::operator/;
    std::function<QA(QA, QA)> pow = QA::operator^;

    MathParser::operators["+"] = add;
    MathParser::operators["-"] = sub;
    MathParser::operators["*"] = mult;
    MathParser::operators["/"] = div;
    MathParser::operators["^"] = pow;

    // Load constants
    MathParser::variables["pi"] = QA(1, 3.1415926535);
    MathParser::variables["e"] = QA(1, 2.7182818284);

    // Load functions
    MathParser::functions["sin"] = 1.0; 
    MathParser::functions["cos"] = 1.0;
    MathParser::functions["tan"] = 1.0;
}

void MathParser::SetVariable(std::string variableName, const MathParser::QuickArray &variableValue) {
    MathParser::variables[variableName] = variableValue;
}

void MathParser::SetVariable(std::string variableName, const MathParser::QuickArray &&variableValue) { // Overload for rvalues
    MathParser::variables[variableName] = variableValue;
}

std::vector<MathParser::QuickArray> MathParser::EvalReversePolishToks(const std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<MathParser::QuickArray> output;
    for (auto token : tokens) {
        switch (token->type) {
            case NUMBER: {
                std::shared_ptr<Number> num = std::dynamic_pointer_cast<Number>(token);
                output.push_back(*(num->value));
                break;
            }
            case OPERATOR: {
                auto backIdx = output.size();
                auto arg1 = output[backIdx-1];
                auto arg2 = output[backIdx-2];

                auto result = arg2.applyBinaryOp(MathParser::operators[token->name], arg1);

                output.pop_back();
                output.pop_back();
                output.push_back(result);
                break;
            }

            case IDENTIFIER: {
                auto idName = token->name;
                std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
                if (identifier->idType != IdentifierType::FUNCTION) {
                    output.push_back(MathParser::variables[idName]);
                }
                else {

                }
            }

        }
    }
    return output;
}