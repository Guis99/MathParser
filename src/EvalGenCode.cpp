#include "../include/MathParser.hpp"
#include "../include/BuiltIns.hpp"

namespace MathParser {
    std::unordered_map<std::string, std::function<QA(QA, QA)>> operators;
    std::unordered_map<std::string, mathFunc> mathFunctions;
}

void MathParser::RunMPEngine() {
    
}

QA MathParser::ParseText(std::string inputString) {
    bool isAssign = false; size_t stopIdx = 0;
    auto rawTokens = MathParser::TokenizeString(inputString);
    auto tokens = MathParser::TransformTokens(rawTokens, isAssign, stopIdx);

    std::vector<std::shared_ptr<Token>> parsedTokens;
    QA result;

    if (!isAssign) {
        auto parsedTokens = MathParser::ShuntingYard(tokens);
        for (auto token : parsedTokens) {
        // std::cout<<token->name<<", "<<token->type;
        if (token->type == TokenType::IDENTIFIER) {
            std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
            // std::cout<<identifier->idType<<std::endl;
        }  
        // std::cout<<std::endl;
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
    MathParser::functions["BCTOR"] = FunctionType::BCTOR;
    MathParser::functions["INDEX"] = FunctionType::INDEX;
    MathParser::functions["COMPOSE"] = FunctionType::COMPOSE;
    MathParser::functions["sum"] = FunctionType::SUM;
    MathParser::functions["norm"] = FunctionType::NORM;
    MathParser::functions["shape"] = FunctionType::SHAPE;
    MathParser::functions["range"] = FunctionType::RANGE;
    MathParser::functions["linspace"] = FunctionType::LINSPACE;

    // Load math functions
    MathParser::mathFunctions["exp"] = std::exp;
    MathParser::mathFunctions["log"] = std::log;
    MathParser::mathFunctions["log10"] = std::log10;
    MathParser::mathFunctions["log2"] = std::log2;
    MathParser::mathFunctions["sin"] = std::sin;
    MathParser::mathFunctions["cos"] = std::cos;
    MathParser::mathFunctions["tan"] = std::tan;
    MathParser::mathFunctions["asin"] = std::asin;
    MathParser::mathFunctions["acos"] = std::acos;
    MathParser::mathFunctions["atan"] = std::atan;
    MathParser::mathFunctions["sinh"] = std::sinh;
    MathParser::mathFunctions["cosh"] = std::cosh;
    MathParser::mathFunctions["tanh"] = std::tanh;
    MathParser::mathFunctions["asinh"] = std::asinh;
    MathParser::mathFunctions["acosh"] = std::acosh;
    MathParser::mathFunctions["atanh"] = std::atanh;
    MathParser::mathFunctions["erf"] = std::erf;
    MathParser::mathFunctions["erfc"] = std::erfc;
    MathParser::mathFunctions["tgamma"] = std::tgamma;
}

void MathParser::SetVariable(std::string variableName, const QA &variableValue) {
    MathParser::variables[variableName] = variableValue;
}

void MathParser::SetVariable(std::string variableName, const QA &&variableValue) { // Overload for rvalues
    MathParser::variables[variableName] = variableValue;
}

std::vector<QA> MathParser::EvalReversePolishToks(const std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<QA> output;
    for (auto token : tokens) {
        switch (token->type) {
            case NUMBER: {
                std::shared_ptr<Number> num = std::dynamic_pointer_cast<Number>(token);
                output.push_back(*(num->value));
                break;
            }
            case OPERATOR: {
                std::shared_ptr<Operator> oper = std::dynamic_pointer_cast<Operator>(token);
                auto backIdx = output.size();
                auto arg1 = output[backIdx-1];

                QA result;

                switch (oper->oper) {
                    case OperatorType::NEGATE: {
                        result = MathParser::NEGATE(arg1);
                        break;
                    }
                    case OperatorType::OF: {
                        auto arg2 = output[backIdx-2];
                        output.pop_back();
                        result = MathParser::COMPOSE(arg2,arg1);
                        break;
                    }
                    default: {
                        auto arg2 = output[backIdx-2];
                        output.pop_back();
                        result = arg2.applyBinaryOp(MathParser::operators[token->name], arg1);
                        break;
                    }
                }

                output.pop_back();
                output.push_back(result);
                break;
            }

            case IDENTIFIER: {
                auto idName = token->name;
                std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
                // std::cout<<idName<<", "<<identifier->idType<<std::endl;
                if (MathParser::variables.find(idName) != MathParser::variables.end()) {
                    // std::cout<<idName<<std::endl;
                    output.push_back(MathParser::variables[idName]);
                }
                else if (MathParser::mathFunctions.find(idName) != MathParser::mathFunctions.end()) {
                    auto prevNumber = output.back();
                    output.pop_back();
                    output.push_back(MathParser::evalMathFunction(prevNumber, MathParser::mathFunctions[idName]));
                }
                else {
                    // std::cout<<"here2"<<std::endl;
                    std::shared_ptr<Function> func = std::dynamic_pointer_cast<Function>(token);
                    // std::cout<<"here3"<<", "<<MathParser::functions[idName]<<std::endl;
                    switch (MathParser::functions[idName]) {
                        
                        case FunctionType::BCTOR: {
                            // std::cout<<"here4"<<", "<<MathParser::functions[idName]<<std::endl;                            
                            QA result;
                            auto backIdx = output.size();
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
                            break;
                        }
                        case FunctionType::INDEX: {
                            auto backIdx = output.size();
                            auto arg2 = output[backIdx-1];
                            auto arg1 = output[backIdx-2];
                            output.pop_back();
                            output.pop_back();
                            auto result = MathParser::INDEX(arg1,arg2);
                            output.push_back(result);
                            break;
                        }
                        case FunctionType::SUM: {
                            auto arg1 = output.back();
                            output.pop_back();
                            auto result = MathParser::SUM(arg1);
                            output.push_back(result);
                            break;
                        }
                        case FunctionType::NORM: {
                            auto backIdx = output.size();
                            auto arg2 = output[backIdx-1];
                            auto arg1 = output[backIdx-2];
                            output.pop_back();
                            output.pop_back();
                            auto result = MathParser::NORM(arg1,arg2);
                            output.push_back(result);
                            break;
                        }
                        case FunctionType::SHAPE: {
                            auto arg1 = output.back();
                            output.pop_back();
                            auto result = MathParser::SHAPE(arg1);
                            output.push_back(result);
                            break;
                        }
                        case FunctionType::RANGE: {
                            auto backIdx = output.size();
                            auto arg3 = output[backIdx-1];
                            auto arg2 = output[backIdx-2];
                            
                            QA result;
                            if (func->arity == 2) {
                                result = MathParser::RANGE(arg2,arg3);
                            }
                            else if (func->arity == 3) {
                                auto arg1 = output[backIdx-3];
                                result = MathParser::RANGE(arg1,arg2,arg3);
                                output.pop_back();
                            }

                            output.pop_back();
                            output.pop_back();
                            
                            output.push_back(result);
                            break;
                        }
                        case FunctionType::LINSPACE: {
                            auto backIdx = output.size();
                            auto arg3 = output[backIdx-1];
                            auto arg2 = output[backIdx-2];
                            auto arg1 = output[backIdx-3];
                            output.pop_back();
                            output.pop_back();
                            output.pop_back();
                            auto result = MathParser::LINSPACE(arg1,arg2,arg3);
                            output.push_back(result);
                            break;
                        }
                    }
                }
            }

        }
    }
    return output;
}