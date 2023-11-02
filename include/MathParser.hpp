#include "Token.hpp"

namespace MathParser {

    std::unordered_map<std::string, Variable> variables;
    std::unordered_map<std::string, double> constants;
    std::unordered_map<std::string, double> functions;

    std::vector<double> EvalExpression(std::string inputString, std::string vars);
    void TokenizeString(std::string &inputString, std::vector<std::shared_ptr<Token>> &tokens, std::string vars);    
    std::vector<std::shared_ptr<Token>> ShuntingYard(std::vector<std::shared_ptr<Token>> &tokens);

    std::vector<double> EvalReversePolishToks(std::vector<std::shared_ptr<Token>> &tokens);
}