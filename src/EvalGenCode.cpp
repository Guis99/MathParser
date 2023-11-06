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
        auto parsedTokens = MathParser::ShuntingYard(tokens);
        for (auto token : parsedTokens) {
        std::cout<<token->name<<", "<<token->type;
        if (token->type == TokenType::IDENTIFIER) {
            std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
            std::cout<<identifier->idType<<std::endl;
        }  
        std::cout<<std::endl;
    }
        result = MathParser::EvalReversePolishToks(parsedTokens)[0];
    }
    else {
        std::vector<std::shared_ptr<Token>> slicedTokens(tokens.begin() + stopIdx, tokens.end());

        auto parsedTokens = MathParser::ShuntingYard(slicedTokens);
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
    MathParser::functions["sin"] = 0; 
    MathParser::functions["cos"] = 1;
    MathParser::functions["tan"] = 2;
    MathParser::functions["bctor"] = 3;
    MathParser::functions["idx"] = 4;
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

                MathParser::QuickArray result;

                if (token->name != "of") {
                    result = arg2.applyBinaryOp(MathParser::operators[token->name], arg1);
                }
                else {
                    result = MathParser::QAofQA(arg2,arg1);
                }

                output.pop_back();
                output.pop_back();
                output.push_back(result);
                break;
            }

            case IDENTIFIER: {
                auto idName = token->name;
                std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
                std::cout<<idName<<", "<<identifier->idType<<std::endl;
                if (MathParser::variables.find(idName) != MathParser::variables.end()) {
                    std::cout<<"here1"<<std::endl;
                    output.push_back(MathParser::variables[idName]);
                }
                else {
                    std::cout<<"here2"<<std::endl;
                    std::shared_ptr<Function> func = std::dynamic_pointer_cast<Function>(token);
                    std::cout<<"here3"<<", "<<MathParser::functions[idName]<<std::endl;
                    switch (MathParser::functions[idName]) {
                        case FunctionType::BCONSTRUCT: {
                            std::cout<<"here4"<<", "<<MathParser::functions[idName]<<std::endl;
                            MathParser::QuickArray result;
                            auto backIdx = output.size();
                            std::cout<<"here5"<<", "<<func->name<<std::endl;
                            std::cout<<func->arity<<", "<<backIdx<<std::endl;
                            for (int i=backIdx - func->arity; i<backIdx; i++) {
                                auto feedArray = output[i];
                                for (const auto &element : feedArray) {
                                    result.push_back(element);
                                }
                            }
                            for (int i=0; i<func->arity; i++) {
                                output.pop_back();
                            }
                            output.push_back(result);
                        }
                    }
                }
            }

        }
    }
    return output;
}