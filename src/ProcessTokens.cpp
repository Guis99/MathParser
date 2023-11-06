#include "..\include\MathParser.hpp"

namespace MathParser {
    std::unordered_map<std::string, QuickArray> constants;
    std::unordered_map<std::string, int> functions;
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

std::string MathParser::GetKeywords() {
    std::string out;
    out = "of";
    return out;
}

void MathParser::TokenizeString(std::string &inputString, std::vector<std::shared_ptr<Token>> &tokens, bool &isAssignment, size_t &stopIdx) {
    std::string operatorPattern = "[-+*/^]";
    std::string stringPattern = "[a-zA-Z]+";
    // std::string variablePattern = MathParser::GetCurrentVariables();
    // std::string mathPattern = "sin|cos|tan";
    std::string assignPattern = "=";
    std::string angleBracketPattern = "<|>";
    std::string keywordPattern = MathParser::GetKeywords();
    std::string commaPattern = ",";
    std::string regexString = R"((\d*\.?\d+)|()" + 
                                operatorPattern + 
                                R"()|(\()|(\))|()" +
                                assignPattern + R"()|()" +
                                angleBracketPattern + R"())" +
                                R"(|(\[|\])|()" +
                                keywordPattern + R"()|()" +
                                commaPattern + R"()|()" +
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
                        Token *tok = new Token();
                        std::shared_ptr<Token> token(tok);
                        token->type = TokenType::ASSIGN;
                        token->name = val;
                        tokens.push_back(token);
                        isAssignment = true;
                        stopIdx = currIdx;
                        break;
                    }
                    case 6: {
                        if (val == "<") {
                            // create identifier
                            Identifier *id = new Identifier;
                            std::shared_ptr<Identifier> token(id);
                            token->name = "bctor";
                            token->type = TokenType::IDENTIFIER;
                            // Create open paren
                            Paren *paren = new Paren();
                            std::shared_ptr<Paren> parenPtr(paren);
                            parenPtr->type = TokenType::OPEN_PAREN;
                            parenPtr->name = "(";
                            tokens.push_back(token);
                            tokens.push_back(parenPtr);
                        }
                        else {
                            Paren *paren = new Paren();
                            std::shared_ptr<Paren> token(paren);
                            token->type = TokenType::CLOSE_PAREN;
                            token->name = ")";
                            tokens.push_back(token);
                        }
                        break;
                    }
                    case 7: {
                        Token *tok = new Token();
                        std::shared_ptr<Token> token(tok);
                        token->type = val == "[" ? TokenType::OPEN_SQUARE : TokenType::CLOSE_SQUARE;
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 8: {
                        Token *tok = new Token();
                        std::shared_ptr<Token> token(tok);
                        if (val == "of") {
                            token->type = TokenType::OPERATOR;
                            token->name = val;
                        }
                    
                        tokens.push_back(token);
                        break;
                    }
                    case 9: {
                        Token *tok = new Token();
                        std::shared_ptr<Token> token(tok);
                        token->type = TokenType::COMMA;
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 10: {
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
                // std::cout<<idName<<std::endl;
                if (MathParser::functions.find(idName) != MathParser::functions.end()) {
                    // create new function object
                    Function *newfunc = new Function();
                    std::shared_ptr<Function> funcPtr(newfunc);
                    funcPtr->name = idName;
                    funcPtr->type = TokenType::IDENTIFIER;
                    funcPtr->idType = IdentifierType::FUNCTION;
                    funcPtr->functionName = FunctionType::BCONSTRUCT;
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
            // case TokenType::OPEN_ANGLE: {
            //     // Create function token
            //     Function *newfunc = new Function();
            //     std::shared_ptr<Function> token(newfunc);
            //     token->name = "bctor";
            //     // Create open paren
            //     Paren *paren = new Paren();
            //     std::shared_ptr<Paren> newParen(paren);
            //     newParen->type = TokenType::OPEN_PAREN;
            //     newParen->name = "(";

            //     opStack.push(token);
            //     opStack.push(newParen);
            // }
        }  
        // std::cout<<tok<<", "<<token->name<<", "<<token->type<<std::endl;
    }

    while (!opStack.empty()) {
        RpnVec.push_back(opStack.top());
        opStack.pop();
    }
    return RpnVec;
}