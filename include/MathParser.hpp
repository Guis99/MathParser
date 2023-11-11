#include "Token.hpp"
#include "QuickArray.hpp"

#ifndef TOKEN_PROCESSING
#define TOKEN_PROCESSING

typedef MathParser::QuickArray<double> QA;
typedef double (*mathFunc)(double);

namespace MathParser {
    // Declare all maps
    extern std::unordered_map<std::string, FunctionType> functions;
    extern std::unordered_map<std::string, mathFunc> mathFunctions;
    extern std::unordered_map<std::string, QA> variables;
    extern std::unordered_map<std::string, std::function<QA(const QA, const QA)>> operators;

    // Token processing - ProcessTokens.cpp
    std::vector<RawToken> TokenizeString(std::string &inputString);
    std::vector<std::shared_ptr<Token>> TransformTokens(std::vector<RawToken> &rawTokens, 
                                                        bool &isAssignment, 
                                                        size_t &stopIdx);    
    std::vector<std::shared_ptr<Token>> ShuntingYard(const std::vector<std::shared_ptr<Token>> &tokens);
    std::string GetCurrentVariables();
    std::string GetKeywords();

    // Evaluate intermediate representation - EvalGenCode.cpp
    QA ParseText(std::string inputString);
    QA EvalExpression(std::string inputString);
    std::vector<QA> EvalReversePolishToks(const std::vector<std::shared_ptr<Token>> &tokens);

    // Run eval engine   
    void RunMPEngine();
    void SetVariable(std::string variableName, const QA &variableValue);
    void SetVariable(std::string variableName, const QA &&variableValue);
    void InitMaps();

    // Exceptions
    class UnknownIdentifierException : public std::exception {
        public:
            UnknownIdentifierException() {
                message = "Variable or function does not exist.";
            }
            const char* what() const noexcept override {
                return message.c_str();
            }

        private:
            std::string message;
    };

    class InvalidArrayIndexException : public std::exception {
        public:
            InvalidArrayIndexException() {
                message = "Array index out of bounds.";
            };
            const char* what() const noexcept override {
                return message.c_str();
            }

        private:
            std::string message;
    };
}


#endif