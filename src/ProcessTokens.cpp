#include "..\include\MathParser.hpp"

namespace MathParser {
    std::unordered_map<std::string, QuickArray> constants;
    std::unordered_map<std::string, double> functions;
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

void MathParser::TokenizeString(std::string &inputString, std::vector<std::shared_ptr<Token>> &tokens, bool &isAssignment, size_t &stopIdx) {
    // std::vector<std::shared_ptr<Token>> tokens;

    std::string operatorPattern = "[-+*/^]";
    std::string stringPattern = "[a-zA-Z]+";
    std::string variablePattern = MathParser::GetCurrentVariables();
    std::string mathPattern = "sin|cos|tan";
    std::string assignPattern = "=";
    std::string angleBracketPattern = "<|>";
    std::string regexString = R"((\d*\.?\d+)|()" + 
                                operatorPattern +
                                 R"()|(\()|(\))|()" + 
                                variablePattern + R"()|()" + 
                                mathPattern + R"()|()" + 
                                assignPattern + R"()|()" +
                                angleBracketPattern + R"()|()" +
                                stringPattern + R"())";
    
    // std::cout<<regexString<<std::endl;
    std::regex regexPattern(regexString);
    std::smatch match;

    auto pos = inputString.cbegin();
    size_t currIdx = 0;
    while (std::regex_search(pos, inputString.cend(), match, regexPattern)) {
        for (size_t i = 1; i < match.size(); ++i) {
            if (!match[i].str().empty()) {
                std::string val = match[i].str();
                // std::cout<<val<<", "<<i<<"|"<<std::endl;
                switch (i) {
                    case 1: {
                        // Initialize smart pointer to number token
                        Number *number = new Number();
                        std::shared_ptr<Number> token(number);
                        // Initialize smart pointer to QuickArray
                        QuickArray *QA = new MathParser::QuickArray(std::stod(val));
                        std::shared_ptr<MathParser::QuickArray> QuickArrayPtr(QA);
                        // Set token attributes
                        token->type = TokenType::NUMBER;
                        token->name = val;
                        token->value = QuickArrayPtr;
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
                    case 7: {
                        Token *tok = new Token();
                        std::shared_ptr<Token> token(tok);
                        token->type = TokenType::ASSIGN;
                        token->name = val;
                        tokens.push_back(token);
                        isAssignment = true;
                        // std::cout<<token->name<<"," <<currIdx<<", "<<stopIdx<<std::endl;
                        stopIdx = currIdx;
                        // std::cout<<currIdx<<", "<<stopIdx<<std::endl;
                        break;
                    }
                    case 8: {
                        Token *tok = new Token();
                        std::shared_ptr<Token> token(tok);
                        token->type = val == "<" ? TokenType::OPEN_ANGLE : TokenType::CLOSE_ANGLE;
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 9: {
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
        currIdx++;
    }
}

std::vector<std::shared_ptr<Token>> MathParser::ShuntingYard(const std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<std::shared_ptr<Token>> RpnVec; RpnVec.reserve(tokens.size());
    std::stack<std::shared_ptr<Token>> opStack;
    int tok = 0;
    for (int i=0; i<tokens.size(); i++) {
        auto token = tokens[i];
        // std::cout<<tok++<<", "<<token->name<<", "<<token->type<<std::endl;
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
                while (!opStack.empty() && 
                    (opStack.top()->type != TokenType::OPEN_PAREN)) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop();
                }
                opStack.pop();
                if (!opStack.empty() && opStack.top()->type == TokenType::IDENTIFIER) {
                    std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(opStack.top());
                    if (identifier->idType == IdentifierType::FUNCTION) {
                        RpnVec.push_back(opStack.top());
                        opStack.pop();
                    }
                }
                break;
            } 
            case TokenType::OPEN_ANGLE: {
                i++;
                // Create number token
                Number *number = new Number();
                std::shared_ptr<Number> token(number);
                // Create QuickArray
                QuickArray *QA = new MathParser::QuickArray();
                std::shared_ptr<MathParser::QuickArray> QuickArrayPtr(QA);
                while (tokens[i]->type != CLOSE_ANGLE) {
                    QuickArrayPtr->push_back(std::stod(tokens[i]->name));
                    i++;
                }
                token->value = QuickArrayPtr;
                token->name = "QAConstruct";
                RpnVec.push_back(token);
            }
        }  
        // std::cout<<tok<<", "<<token->name<<", "<<token->type<<std::endl;
    }

    while (!opStack.empty()) {
        RpnVec.push_back(opStack.top());
        opStack.pop();
    }
    return RpnVec;
}