#include "..\include\MathParser.hpp"

std::vector<double> MathParser::EvalExpression(std::string inputString, std::string vars) {
    
    std::vector<double> out;
    std::cout<<"line1"<<std::endl;
    auto tokens = MathParser::TokenizeString(inputString, vars);
    std::cout<<"line2"<<std::endl;
    for (auto token : tokens) {
        std::cout<<token->name<<","<<token->type<<std::endl;
    }

    std::cout<<tokens.size()<<std::endl;
    std::cout<<"-----------"<<std::endl;
    std::cout<<"line3"<<std::endl;
    auto parsedTokens = MathParser::ShuntingYard(tokens);
    std::cout<<"line4"<<std::endl;
    for (auto token : parsedTokens) {
        std::cout<<token->name<<std::endl;
    }
    return out;
}

std::vector<std::shared_ptr<Token>> MathParser::TokenizeString(std::string &inputString, std::string vars) {
    std::vector<std::shared_ptr<Token>> tokens;

    std::string operatorPattern = "[-+*/^=]";
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
            // std::shared_ptr<Token> token;
            if (!match[i].str().empty()) {
                std::string val = match[i].str();
                switch (i) {
                    case 1: {
                        std::cout<<i<<", "<<tok<<", "<<val<<std::endl;
                        Number *number = new Number();
                        std::shared_ptr<Number> token(number);
                        token->name = val;
                        token->type = TokenType::NUMBER; 
                        token->value = std::stod(val);
                        tokens.push_back(token);
                        break;
                    }
                    case 2: {
                        std::cout<<i<<", "<<tok<<", "<<val<<std::endl;
                        Operator *operatorT = new Operator();
                        std::shared_ptr<Operator> token(operatorT);
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
                        std::cout<<i<<", "<<tok<<", "<<val<<std::endl;
                        Paren *paren = new Paren();
                        std::shared_ptr<Paren> token(paren);
                        token->type = TokenType::OPEN_PAREN; 
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 4: {
                        std::cout<<i<<", "<<tok<<", "<<val<<std::endl;
                        Paren *paren = new Paren();
                        std::shared_ptr<Paren> token(paren);
                        token->type = TokenType::CLOSE_PAREN; 
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                    case 5: {
                        std::cout<<i<<", "<<tok<<", "<<val<<std::endl;
                        Variable *variable = new Variable();
                        std::shared_ptr<Variable> token(variable);
                        token->type = VARIABLE; 
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }

                    case 6: {
                        std::cout<<i<<", "<<tok<<", "<<val<<std::endl;
                        Function *func = new Function();
                        std::shared_ptr<Function> token(func);
                        token->type = FUNCTION; 
                        token->name = val;
                        tokens.push_back(token);
                        break;
                    }
                }
            }
        }
        pos = match.suffix().first;
    }
    return tokens;
}

std::vector<std::shared_ptr<Token>> MathParser::ShuntingYard(std::vector<std::shared_ptr<Token>> &tokens) {
    std::vector<std::shared_ptr<Token>> RpnVec; RpnVec.reserve(tokens.size());
    std::stack<std::shared_ptr<Token>> opStack;

    for (auto token : tokens) {
        switch (token->type) {
            case TokenType::NUMBER: 
                RpnVec.push_back(token);
                break;
            case TokenType::VARIABLE: 
                RpnVec.push_back(token);
                break;
            case TokenType::OPEN_PAREN: 
                opStack.push(token);
                break;
            case TokenType::FUNCTION: 
                opStack.push(token);
                break;
            case TokenType::OPERATOR:
                while (!opStack.empty()) { 
                    std::shared_ptr<Operator> o1 = std::dynamic_pointer_cast<Operator>(token);
                    std::shared_ptr<Operator> o2 = std::dynamic_pointer_cast<Operator>(opStack.top());
                    if (o2->type != TokenType::OPEN_PAREN &&  
                    (o2->precedence >= o1->precedence)) {
                        RpnVec.push_back(opStack.top());
                        opStack.pop(); 
                    }
                    else break;
                }
                opStack.push(token);
                break;    
            case CLOSE_PAREN:
                while (!opStack.empty() && 
                    (opStack.top()->type != TokenType::OPEN_PAREN)) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop();
                }
                opStack.pop();
                if (!opStack.empty() && opStack.top()->type == TokenType::FUNCTION) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop();
                }
                break;
        }  
    }

    while (!opStack.empty()) {
        RpnVec.push_back(opStack.top());
        opStack.pop();
    }
    return RpnVec;
}