#include "../include/MathParser.hpp"

std::vector<double> MathParser::EvalReversePolishToks(std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<double> output;
    for (auto token : tokens) {
        switch (token->type) {
            case NUMBER:
                std::shared_ptr<Number> num = std::dynamic_pointer_cast<Number>(token);
                output.push_back(num->value);
                break;
            case OPERATOR:
                auto backIdx = output.size() - 1;
                
                break;
            // case FUNCTION: // eval math expression
            //     // auto backElm = output.back();
            //     break;
            // case VARIABLE:
            //     break;

        }
    }
    return output;
}