#include "..\include\MathParser.hpp"

std::vector<double> MathParser::EvalExpression(std::string inputString, std::string vars) {
    
    std::vector<double> out;
    auto tokens = MathParser::TokenizeString(inputString, vars);

    for (auto str : tokens) {
        std::cout<<str.value<<","<<str.type<<std::endl;
    }

    std::cout<<tokens.size()<<std::endl;
    std::cout<<"-----------"<<std::endl;
    auto parsedTokens = MathParser::ShuntingYard(tokens);
    for (auto str : parsedTokens) {
        std::cout<<str.value<<std::endl;
    }
    return out;
}

std::vector<Token> MathParser::TokenizeString(std::string &inputString, std::string vars) {
    std::vector<Token> tokens;
    std::string variablePattern = "[" + vars + "]";
    std::string mathPattern = "sin|cos|tan";
    std::string regexString = R"((\d+)|([-+*/^])|([\(\{\[])|([\)\}\]])|()" + 
                                variablePattern + R"()|()" + 
                                mathPattern + R"())";
    
    std::cout<<regexString<<std::endl;
    std::regex regexPattern(regexString);
    std::smatch match;

    auto pos = inputString.cbegin();
    while (std::regex_search(pos, inputString.cend(), match, regexPattern)) {
        for (size_t i = 1; i < match.size(); ++i) {
            if (!match[i].str().empty()) {
                Token token;
                token.value = match[i].str();
                switch (i) {
                    case 1: token.type = NUMBER; break;
                    case 2: token.type = OPERATOR; 
                        if (token.value == "^") {
                            token.precedence = 3;
                        }
                        else if (token.value == "*" | token.value == "/") {
                            token.precedence = 2;
                        }
                        else if (token.value == "+" | token.value == "-") {
                            token.precedence = 1;
                        }
                        break;
                    case 3: token.type = OPEN_PAREN; break;
                    case 4: token.type = CLOSE_PAREN; break;
                    case 5: token.type = INDEP_VARIABLE; break;
                    case 6: token.type = MATH_FUNCTION; break;
                }
                tokens.push_back(token);
            }
        }
        pos = match.suffix().first;
    }
    return tokens;
}

std::vector<Token> MathParser::ShuntingYard(std::vector<Token> &tokens) {
    std::vector<Token> RpnVec; RpnVec.reserve(tokens.size());
    std::stack<Token> opStack;

    for (auto token : tokens) {
        switch (token.type) {
            case NUMBER: 
                RpnVec.push_back(token);
                break;
            case INDEP_VARIABLE: 
                RpnVec.push_back(token);
                break;
            case OPEN_PAREN: 
                opStack.push(token);
                break;
            case MATH_FUNCTION: 
                opStack.push(token);
                break;
            case OPERATOR: 
                while (!opStack.empty() && 
                    (opStack.top().type != OPEN_PAREN) & 
                    (opStack.top().precedence >= token.precedence)) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop(); 
                }
                opStack.push(token);
                break;    
            case CLOSE_PAREN:
                while (!opStack.empty() && 
                    (opStack.top().type != OPEN_PAREN)) {
                    RpnVec.push_back(opStack.top());
                    opStack.pop();
                }
                opStack.pop();
                if (!opStack.empty() && opStack.top().type == MATH_FUNCTION) {
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

// std::vector<double> MathParser::EvalReversePolishToks(std::vector<Token> &tokens) {
//     std::vector<double> output;
//     for (auto token : tokens) {
//         switch (token.type) {
//             case NUMBER:
//                 output.push_back(std::stod(token.value));
//                 break;
//             case OPERATOR:
//                 auto backIdx = output.size() - 1;
                
//                 break;
//             case MATH_FUNCTION: // eval math expression
//                 auto backElm = output.back();

//         }
//     }
//     return output;
// }