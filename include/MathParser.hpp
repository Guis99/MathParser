#include "Token.hpp"

namespace MathParser {
    std::vector<double> EvalExpression(std::string inputString, std::string vars);
    std::vector<Token> TokenizeString(std::string &inputString, std::string vars);
    std::vector<Token> ShuntingYard(std::vector<Token> &tokens);
    std::vector<double> EvalReversePolishToks(std::vector<Token> &tokens);
}