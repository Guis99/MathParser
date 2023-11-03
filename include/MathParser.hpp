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
    extern std::unordered_map<std::string, std::function<QuickArray(const QuickArray, const QuickArray)>> operators;

    // Token processing - ProcessTokens.cpp
    void TokenizeString(std::string &inputString, std::vector<std::shared_ptr<Token>> &tokens, bool &isAssignment, size_t &stopIdx);    
    std::vector<std::shared_ptr<Token>> ShuntingYard(const std::vector<std::shared_ptr<Token>> &tokens);
    std::string GetCurrentVariables();

    // Evaluate intermediate representation - EvalGenCode.cpp
    QuickArray ParseText(std::string inputString);
    QuickArray EvalExpression(std::string inputString);
    std::vector<MathParser::QuickArray> EvalReversePolishToks(const std::vector<std::shared_ptr<Token>> &tokens);
    void SetVariable(std::string variableName, const QuickArray &variableValue);
    void SetVariable(std::string variableName, const QuickArray &&variableValue);
    void InitMaps();

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