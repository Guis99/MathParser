#include "../MathParser.hpp"
using namespace MathParser;

int main() {

    InitMaps();

    MathParser::variables["x"] = QA(1, 3.4);
    MathParser::variables["y"] = QA(1, 1.0);

    std::string str;

    std::cout<<"Enter expression to evaluate: ";
    std::getline(std::cin,str);
    std::cout<<std::endl<<str<<std::endl;

    bool isAssign;
    size_t assignIdx;
    std::vector<std::shared_ptr<Token>> tokens;

    TokenizeString(str, tokens, isAssign, assignIdx);
    auto parsedTokens = ShuntingYard(tokens);
    for (auto token : parsedTokens) {
        // std::cout<<token->name<<", "<<token->type;
        if (token->type == TokenType::IDENTIFIER) {
            // std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
            // auto identifierObj = *identifier;
            
            // std::shared_ptr<Function> funcPtr = std::dynamic_pointer_cast<Function>(token);
            // std::cout<<", "<<funcPtr->arity<<", "<<funcPtr->idType<<std::endl;
            std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
            std::cout<<identifier->name<<", "<<identifier->idType<<std::endl;
            std::shared_ptr<Function> funcPtr = std::dynamic_pointer_cast<Function>(token);
            std::cout<<funcPtr->name<<", "<<funcPtr->arity<<", "<<funcPtr->idType<<std::endl;
        }  
        std::cout<<std::endl;
    }
}