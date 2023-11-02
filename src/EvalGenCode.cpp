#include "../include/MathParser.hpp"

namespace MathParser {
    std::unordered_map<std::string, std::function<QuickArray(QuickArray, QuickArray)>> operators;
}

void MathParser::InitMaps() {
    std::function<QA(QA, QA)> add = QA::operator+;
    std::function<QA(QA, QA)> sub = QA::operator-;
    std::function<QA(QA, QA)> mult = QA::operator*;
    std::function<QA(QA, QA)> div = QA::operator/;
    std::function<QA(QA, QA)> pow = QA::operator^;

    MathParser::variables["x"] = QA(1, 3.4);

    MathParser::constants["pi"] = QA(1, 3.1415926535);
    MathParser::constants["e"] = QA(1, 2.7182818284);

    MathParser::functions["sin"] = 1.0; 
    MathParser::functions["cos"] = 1.0;
    MathParser::functions["tan"] = 1.0;

    MathParser::operators["+"] = add;
    MathParser::operators["-"] = sub;
    MathParser::operators["*"] = mult;
    MathParser::operators["/"] = div;
    MathParser::operators["^"] = pow;
}

std::vector<MathParser::QuickArray> MathParser::EvalReversePolishToks(std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<MathParser::QuickArray> output;
    for (auto token : tokens) {
        switch (token->type) {
            case NUMBER: {
                std::shared_ptr<Number> num = std::dynamic_pointer_cast<Number>(token);
                output.push_back(MathParser::QuickArray(1,num->value));
                break;
            }
            case OPERATOR: {
                auto backIdx = output.size();
                auto arg1 = output[backIdx-1];
                auto arg2 = output[backIdx-2];

                auto result = arg1.applyBinaryOp(MathParser::operators[token->name], arg2);

                output.pop_back();
                output.pop_back();
                output.push_back(result);
                break;
            }
            // case FUNCTION: // eval math expression
            //     // auto backElm = output.back();
            //     break;
            // case VARIABLE:
            //     break;

        }
    }
    return output;
}