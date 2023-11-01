#include "Token.hpp"

namespace MathParser {
    std::vector<double> EvalExpression(std::string inputString, std::string vars);
    std::vector<std::shared_ptr<Token>> TokenizeString(std::string &inputString, std::string vars);
    std::vector<std::shared_ptr<Token>> ShuntingYard(std::vector<std::shared_ptr<Token>> &tokens);
    
    std::vector<double> EvalReversePolishToks(std::vector<Token> &tokens);
}