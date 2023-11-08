#include "Token.hpp"
#include "QuickArray.hpp"

#ifndef TOKEN_PROCESSING
#define TOKEN_PROCESSING

typedef MathParser::QuickArray QA;
typedef double (*mathFunc)(double);

namespace MathParser {
    // Declare all maps
    extern std::unordered_map<std::string, FunctionType> functions;
    extern std::unordered_map<std::string, mathFunc> mathFunctions;
    extern std::unordered_map<std::string, QuickArray> variables;
    extern std::unordered_map<std::string, std::function<QuickArray(const QuickArray, const QuickArray)>> operators;

    // Token processing - ProcessTokens.cpp
    std::vector<RawToken> TokenizeString(std::string &inputString);
    std::vector<std::shared_ptr<Token>> TransformTokens(std::vector<RawToken> &rawTokens, 
                                                        bool &isAssignment, 
                                                        size_t &stopIdx);    
    std::vector<std::shared_ptr<Token>> ShuntingYard(const std::vector<std::shared_ptr<Token>> &tokens);
    std::string GetCurrentVariables();
    std::string GetKeywords();

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