#include "../include/MathParser.hpp"

std::string MathParser::GetKeywords() {
    std::string out;
    out = "of";
    return out;
}

std::vector<RawToken> MathParser::TokenizeString(std::string &inputString) {
    std::vector<RawToken> tokens;
    std::string operatorPattern = "[-+*/^]";
    std::string stringPattern = "[a-zA-Z]+";
    // std::string variablePattern = MathParser::GetCurrentVariables();
    // std::string mathPattern = "sin|cos|tan";
    std::string assignPattern = "=";
    std::string angleBracketPattern = "<|>";
    std::string keywordPattern = MathParser::GetKeywords();
    std::string commaPattern = ",";
    std::string regexString = R"((\d*\.?\d+)|()" + 
                                operatorPattern + R"()|()" +
                                assignPattern + R"()|)" +
                                R"((\()|(\))|)" +
                                R"((<)|(>)|)" + 
                                R"((\[)|(\])|()" +
                                keywordPattern + R"()|()" +
                                commaPattern + R"()|()" +
                                R"(;)" + R"()|()" +
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
                RawToken token;
                token.name = val; token.type = i;
                tokens.push_back(token);
            }
        }
        pos = match.suffix().first;
        currIdx++;
    }

    return tokens;
}

std::vector<std::shared_ptr<Token>> MathParser::TransformTokens(std::vector<RawToken> &rawTokens, 
                                                                bool &isAssignment, 
                                                                size_t &stopIdx) {
    std::vector<std::shared_ptr<Token>> tokens;
    for (int i=0; i<rawTokens.size(); i++) {
        auto currToken = rawTokens[i];
        auto val = currToken.name; auto type = currToken.type;
        // std::cout<<val<<", "<<type<<std::endl;
        switch (type) {
            case TokenType::NUMBER : {
                // Initialize smart pointer to number token
                Number *number = new Number();
                std::shared_ptr<Number> token(number);
                // Initialize smart pointer to QuickArray
                QA *qa = new QA(std::stod(val));
                std::shared_ptr<QA> QuickArrayPtr(qa);
                // Set token attributes
                token->type = TokenType::NUMBER;
                token->name = val;
                token->value = QuickArrayPtr;
                tokens.push_back(token);
                break;
            }
            case TokenType::OPERATOR: {
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
                    if (i == 0 || tokens[i-1]->type == TokenType::OPERATOR ||
                        tokens[i-1]->type == TokenType::OPEN_PAREN ||
                        tokens[i-1]->type == TokenType::ASSIGN) {
                        token->oper = OperatorType::NEGATE;
                        token->LH = true;
                        token->name = "NEGATE";
                        token->precedence = 5;
                    }
                    else {
                        token->oper = OperatorType::MINUS;
                        token->LH = true;
                        token->precedence = 1;
                    }
                }

                else if (val == "*") {
                    token->oper = OperatorType::MULT;
                    token->LH = true;
                    token->precedence = 3;
                }

                else if (val == "/") {
                    token->oper = OperatorType::DIV;
                    token->LH = true;
                    token->precedence = 3;
                }

                else if (val == "^") {
                    token->oper = OperatorType::POW;
                    token->LH = false;
                    token->precedence = 4;
                }
                tokens.push_back(token);
                break;
            }

            case TokenType::ASSIGN: {
                Token *tok = new Token();
                std::shared_ptr<Token> token(tok);
                token->type = TokenType::ASSIGN;
                token->name = val;
                tokens.push_back(token);
                isAssignment = true;
                stopIdx = i;
                break;
            }

            case TokenType::OPEN_PAREN: {
                Paren *paren = new Paren();
                std::shared_ptr<Paren> token(paren);
                token->type = TokenType::OPEN_PAREN;
                token->name = val;
                tokens.push_back(token);
                break;
            }
            case TokenType::CLOSE_PAREN : {
                Paren *paren = new Paren();
                std::shared_ptr<Paren> token(paren);
                token->type = TokenType::CLOSE_PAREN;
                token->name = val;
                tokens.push_back(token);
                break;
            }
            case TokenType::OPEN_ANGLE: {
                    // create identifier
                Identifier *id = new Identifier;
                std::shared_ptr<Identifier> token(id);
                token->name = "BCTOR";
                token->type = TokenType::IDENTIFIER;
                token->idType = IdentifierType::FUNCTION;
                // Create open paren
                Paren *paren = new Paren();
                std::shared_ptr<Paren> parenPtr(paren);
                parenPtr->type = TokenType::OPEN_PAREN;
                parenPtr->name = "(";
                tokens.push_back(token);
                tokens.push_back(parenPtr);
                break;
            }
            case TokenType::CLOSE_ANGLE: {
                Paren *paren = new Paren();
                std::shared_ptr<Paren> token(paren);
                token->type = TokenType::CLOSE_PAREN;
                token->name = ")";
                tokens.push_back(token);
                break;
            }
            case TokenType::OPEN_SQUARE: {
                Identifier *id = new Identifier;
                std::shared_ptr<Identifier> token(id);
                token->name = "INDEX";
                token->type = TokenType::IDENTIFIER;
                token->idType = IdentifierType::FUNCTION;
                // Create open paren
                Paren *paren = new Paren();
                std::shared_ptr<Paren> parenPtr(paren);
                parenPtr->type = TokenType::OPEN_PAREN;
                parenPtr->name = "(";
                // pop previous token - the variable - off the output into temp var
                auto prevToken = tokens.back();
                tokens.pop_back();
                // push tokens in right order
                tokens.push_back(token);
                tokens.push_back(parenPtr);
                tokens.push_back(prevToken);
                break;
            }
            case TokenType::CLOSE_SQUARE: {
                Token *tok = new Token();
                std::shared_ptr<Token> token(tok);
                token->type = TokenType::CLOSE_PAREN;
                token->name = val;
                tokens.push_back(token);
                break;
            }

            case TokenType::KEYWORD: {
                Token *tok = new Token();
                std::shared_ptr<Token> token(tok);
                if (val == "of") {
                    Operator *oper = new Operator();
                    std::shared_ptr<Operator> operPtr(oper);
                    operPtr->type = TokenType::OPERATOR;
                    operPtr->name = "COMPOSE";
                    operPtr->precedence = 2;
                    operPtr->LH = false;
                    operPtr->oper = OperatorType::OF;
                    tokens.push_back(operPtr);
                }            
                break;
            }
            case TokenType::COMMA: {
                Token *tok = new Token();
                std::shared_ptr<Token> token(tok);
                token->type = TokenType::COMMA;
                token->name = "SEP";
                tokens.push_back(token);
                break;
            }
            case TokenType::NEWLINE: {
                Token *tok = new Token();
                std::shared_ptr<Token> token(tok);
                token->type = TokenType::NEWLINE;
                token->name = "NEWLINE";
                tokens.push_back(token);
                break;
            }
            case TokenType::IDENTIFIER: {
                Identifier *identifier = new Identifier();
                std::shared_ptr<Identifier> token(identifier);
                token->type = TokenType::IDENTIFIER;
                token->name = val;
                tokens.push_back(token);
                break;
            }
        }
    }
    return tokens;
}