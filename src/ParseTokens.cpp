#include "..\include\MathParser.hpp"

namespace MathParser {
    std::unordered_map<std::string, FunctionType> functions;
    std::unordered_map<std::string, QuickArray> variables;
}

std::string MathParser::GetCurrentVariables() {
    std::string out;
    auto length = MathParser::variables.size();
    auto it = MathParser::variables.begin();

    for (int i=0; i<length-1; i++) {
        out += it->first + "|";
        it++;
    }
    out += it->first;

    return out;
}

std::vector<std::shared_ptr<Token>> MathParser::ShuntingYard(const std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<std::shared_ptr<Token>> RpnVec; RpnVec.reserve(tokens.size());
    std::stack<std::shared_ptr<Token>> opStack;
    std::stack<int> argCount;
    std::stack<int> wereValues;

    int tok = 0;
    for (int i=0; i<tokens.size(); i++) {
        auto token = tokens[i];
        // std::cout<<tok++<<", "<<token->name<<", "<<token->type<<std::endl;
        switch (token->type) {
            case TokenType::NUMBER: {
                RpnVec.push_back(token);
                if (!wereValues.empty()) {
                    wereValues.pop();
                    wereValues.push(true);
                }
                break;
            }
            case TokenType::OPEN_PAREN: {
                opStack.push(token);
                break;
            }
            case TokenType::OPERATOR: {
                while (!opStack.empty() && opStack.top()->type == TokenType::OPERATOR) { 
                    std::shared_ptr<Operator> o1 = std::dynamic_pointer_cast<Operator>(token);
                    std::shared_ptr<Operator> o2 = std::dynamic_pointer_cast<Operator>(opStack.top()); 
                    if ((o2->precedence > o1->precedence) || 
                        (o2->precedence == o1->precedence && o1->LH)) {
                        RpnVec.push_back(opStack.top());
                        opStack.pop(); 
                        }
                    else break;
                }
                opStack.push(token);
                break;
            }
            case TokenType::IDENTIFIER: {
                std::string idName = token->name;
                if ((MathParser::mathFunctions.find(idName) != MathParser::mathFunctions.end()) ||
                    (MathParser::functions.find(idName) != MathParser::functions.end())) {
                    // create new function object
                    Function *newfunc = new Function();
                    std::shared_ptr<Function> funcPtr(newfunc);
                    funcPtr->name = idName;
                    funcPtr->type = TokenType::IDENTIFIER;
                    funcPtr->idType = IdentifierType::FUNCTION;
                    funcPtr->functionName = MathParser::functions[idName];
                    funcPtr->arity = 0;
                    opStack.push(funcPtr);
                    argCount.push(0);
                    if (!wereValues.empty()) {
                        wereValues.pop();
                        wereValues.push(true);
                    }
                    wereValues.push(false);
                    // std::cout<<funcPtr->name<<", "<<funcPtr->idType<<", "<<funcPtr->functionName<<", "<<funcPtr->arity<<std::endl;
                    break;  
                }
                else if (MathParser::variables.find(idName) != MathParser::variables.end()) {
                    // Create new variable object
                    Variable *var = new Variable();
                    std::shared_ptr<Variable> varPtr(var);
                    varPtr->name = idName;
                    varPtr->type = TokenType::IDENTIFIER;
                    varPtr->idType = IdentifierType::VARIABLE;
                    RpnVec.push_back(varPtr);
                    if (!wereValues.empty()) {
                        wereValues.pop();
                        wereValues.push(true);
                    }
                    break;
                }
                else {
                    throw MathParser::UnknownIdentifierException("Variable or function does not exist");
                }
                break;
            }
            case TokenType::CLOSE_PAREN: {
                while (!opStack.empty() && 
                    (opStack.top()->type != TokenType::OPEN_PAREN)) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop();
                }
                opStack.pop();
                if (!opStack.empty() && opStack.top()->type == TokenType::IDENTIFIER) {
                    std::shared_ptr<Function> identifier = std::dynamic_pointer_cast<Function>(opStack.top());
                    // std::cout<<opStack.top()->name<<", "<<identifier->idType<<", "<<identifier->functionName<<std::endl;
                    if (identifier->idType == IdentifierType::FUNCTION) {
                        std::shared_ptr<Function> funcPtr = std::dynamic_pointer_cast<Function>(opStack.top());
                        // std::cout<<funcPtr->name<<", "<<funcPtr->idType<<", "<<funcPtr->functionName<<std::endl;
                        // std::cout<<opStack.top()->name<<", "<<funcPtr->functionName<<", "<<funcPtr->idType;
                        funcPtr->idType = IdentifierType::FUNCTION;
                        int a = argCount.top();
                        bool w = wereValues.top();
                        if (w) {
                            a++;
                        }
                        // std::cout<<"current a: "<<a<<std::endl;
                        funcPtr->arity = a;
                        // std::cout<<funcPtr->name<<", "<<funcPtr->idType<<", "<<funcPtr->functionName<<", "<<funcPtr->arity<<std::endl;
                        RpnVec.push_back(funcPtr);
                        opStack.pop();
                        wereValues.pop();
                        argCount.pop();
                    }
                }
                break;
            } 
            case TokenType::COMMA: {
                while (!opStack.empty() && opStack.top()->type != TokenType::OPEN_PAREN) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop();
                } 
                bool w = wereValues.top();
                wereValues.pop();
                if (w) {
                    int a = argCount.top() + 1;
                    argCount.pop();
                    argCount.push(a);
                }  
                wereValues.push(false);
            }
        }  
    }

    while (!opStack.empty()) {
        RpnVec.push_back(opStack.top());
        opStack.pop();
    }
    return RpnVec;
}