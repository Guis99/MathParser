#include "Token.hpp"
#include "QuickArray.hpp"

#ifndef TOKEN_PROCESSING
#define TOKEN_PROCESSING

typedef MathParser::QuickArray QA;

namespace MathParser {
    // Declare all maps
    extern std::unordered_map<std::string, QuickArray> constants;
    extern std::unordered_map<std::string, double> functions;
    extern std::unordered_map<std::string, QuickArray> variables;
    extern std::unordered_map<std::string, std::function<QuickArray(QuickArray, QuickArray)>> operators;

    // Token processing - ProcessTokens.cpp
    std::vector<double> EvalExpression(std::string inputString, std::string vars);
    void TokenizeString(std::string &inputString, std::vector<std::shared_ptr<Token>> &tokens, std::string vars);    
    std::vector<std::shared_ptr<Token>> ShuntingYard(std::vector<std::shared_ptr<Token>> &tokens);

    // Evaluate intermediate representation - EvalGenCode.cpp
    std::vector<MathParser::QuickArray> EvalReversePolishToks(std::vector<std::shared_ptr<Token>> &tokens);
    void MathParser::InitMaps();

    // Exceptions
    class UnknownIdentifierException : public std::exception {
        public:
            UnknownIdentifierException(const std::string& message) : message(message) {}
            const char* what() const noexcept override {
                return message.c_str();
            }

        private:
            std::string message;
    };
}

#endif