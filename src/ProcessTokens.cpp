#include "..\include\MathParser.hpp"

namespace MathParser {
    std::unordered_map<std::string, QuickArray> constants;
    std::unordered_map<std::string, double> functions;
    std::unordered_map<std::string, QuickArray> variables;
}

std::vector<double> MathParser::EvalExpression(std::string inputString, std::string vars) {
    
    std::vector<double> out;
    std::vector<std::shared_ptr<Token>> tokens;
    MathParser::TokenizeString(inputString, tokens, vars);
    for (auto token : tokens) {
        std::cout<<token->name<<","<<token->type<<std::endl;
    }

    std::cout<<tokens.size()<<std::endl;
    std::cout<<"-----------"<<std::endl;
    auto parsedTokens = MathParser::ShuntingYard(tokens);
    for (auto token : parsedTokens) {
        std::cout<<token->name<<std::endl;
    }
    return out;
}

void MathParser::TokenizeString(std::string &inputString, std::vector<std::shared_ptr<Token>> &tokens, std::string vars) {
    // std::vector<std::shared_ptr<Token>> tokens;

    std::string operatorPattern = "[-+*/^]";
    std::string stringPattern = "[a-zA-Z]";
    std::string variablePattern = vars;
    std::string mathPattern = "sin|cos|tan";
    std::string regexString = R"((\d+)|()" + 
                                operatorPattern +
                                 R"()|(\()|(\))|()" + 
                                variablePattern + R"()|()" + 
                                mathPattern + R"())";
    
    std::cout<<regexString<<std::endl;
    std::regex regexPattern(regexString);
    std::smatch match;

    auto pos = inputString.cbegin();
    int tok = 0;
    while (std::regex_search(pos, inputString.cend(), match, regexPattern)) {
        tok++;
        for (size_t i = 1; i < match.size(); ++i) {
            if (!match[i].str().empty()) {
                std::string val = match[i].str();
                switch (i) {
                    case 1: {
                        Number *number = new Number();
                        std::shared_ptr<Number> token(number);
                        token->type = TokenType::NUMBER;
                        token->name = val;
                        token->value = std::stod(val);
                        tokens.push_back(token);
                        break;
                    }
                    case 2: {
                        Operator *oper = new Operator();
                        std::shared_ptr<Operator> token(oper);
                        token->type = TokenType::OPERATOR;
                        token->name = val;
                        if (val == "+") {
                            token->oper = OperatorType::PLUS;
                            token->LH = true;
                            token->precedence = 1;
                        }

                        else if (val == "-") {
                            token->oper = OperatorType::MINUS;
                            token->LH = true;
                            token->precedence = 1;
                        }

                        else if (val == "*") {
                            token->oper = OperatorType::MULT;
                            token->LH = true;
                            token->precedence = 2;
                        }

                        else if (val == "/") {
                            token->oper = OperatorType::DIV;
                            token->LH = true;
                            token->precedence = 2;
                        }

                        else if (val == "^") {
                            token->oper = OperatorType::POW;
                            token->LH = false;
                            token->precedence = 3;
                        }
                        tokens.push_back(token);
                        break;
                    }
                    case 3: {
                        Paren *paren = new Paren();
                        std::shared_ptr<Paren> token(paren);
                        token->type = TokenType::OPEN_PAREN;
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 4: {
                        Paren *paren = new Paren();
                        std::shared_ptr<Paren> token(paren);
                        token->type = TokenType::CLOSE_PAREN;
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 5: {
                        Identifier *identifier = new Identifier();
                        std::shared_ptr<Identifier> token(identifier);
                        token->type = TokenType::IDENTIFIER;
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 6: {
                        Identifier *identifier = new Identifier();
                        std::shared_ptr<Identifier> token(identifier);
                        token->type = TokenType::IDENTIFIER;
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                }
            }
        }
        pos = match.suffix().first;
    }
}

std::vector<std::shared_ptr<Token>> MathParser::ShuntingYard(std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<std::shared_ptr<Token>> RpnVec; RpnVec.reserve(tokens.size());
    std::stack<std::shared_ptr<Token>> opStack;
    int tok = 0;
    for (auto token : tokens) {
        std::cout<<tok++<<", "<<token->name<<", "<<token->type<<std::endl;
        switch (token->type) {
            case TokenType::NUMBER: {
                RpnVec.push_back(token);
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
                std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
                std::string idName = identifier->name;
                auto map = MathParser::functions;
                if (MathParser::functions.find(idName) != MathParser::functions.end()) {
                    identifier->idType = IdentifierType::FUNCTION;
                    opStack.push(identifier);
                    break;  
                }
                else if (MathParser::variables.find(idName) != MathParser::variables.end()) {
                    identifier->idType = IdentifierType::VARIABLE;
                    RpnVec.push_back(identifier);
                    break;
                }
                else if (MathParser::constants.find(idName) != MathParser::constants.end()) {
                    std::shared_ptr<Number> number = std::dynamic_pointer_cast<Number>(identifier);
                    identifier->idType = IdentifierType::CONSTANT;
                    RpnVec.push_back(identifier);
                    break;
                }
                else {
                    throw MathParser::UnknownIdentifierException("Variable or function does not exist");
                }
                break;
            }
            case TokenType::CLOSE_PAREN: {
                std::cout<<"here1"<<std::endl;
                while (!opStack.empty() && 
                    (opStack.top()->type != TokenType::OPEN_PAREN)) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop();
                }
                std::cout<<"here2"<<std::endl;
                opStack.pop();
                std::cout<<"here3"<<std::endl;
                if (!opStack.empty() && opStack.top()->type == TokenType::IDENTIFIER) {
                    std::cout<<"here4"<<std::endl;
                    std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(opStack.top());
                    if (identifier->idType == IdentifierType::FUNCTION) {
                        std::cout<<"here5"<<std::endl;
                        RpnVec.push_back(opStack.top());
                        std::cout<<"here6"<<std::endl;
                        opStack.pop();
                    }
                }
            }
                break; 
        }  
        std::cout<<tok<<", "<<token->name<<", "<<token->type<<std::endl;
    }

    while (!opStack.empty()) {
        RpnVec.push_back(opStack.top());
        opStack.pop();
    }
    return RpnVec;
}