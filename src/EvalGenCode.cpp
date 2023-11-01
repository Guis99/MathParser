#include "../include/MathParser.hpp"

std::vector<double> MathParser::EvalReversePolishToks(std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<double> output;
    for (auto token : tokens) {
        switch (token->type) {
            case NUMBER:
                output.push_back(token->value);
                break;
            case OPERATOR:
                auto backIdx = output.size() - 1;
                
                break;
            case FUNCTION: // eval math expression
                auto backElm = output.back();

        }
    }
    return output;
}