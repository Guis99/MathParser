#include "../MathParser.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace MathParser;

std::vector<std::shared_ptr<Token>> ReadText(std::string inputString) {
    bool isAssign = false; size_t stopIdx = 0;
    auto rawTokens = TokenizeString(inputString);
    auto tokens = TransformTokens(rawTokens, isAssign, stopIdx);

    // std::vector<std::shared_ptr<Token>> parsedTokens;
    // QA result;

    // if (!isAssign) {
    //     auto parsedTokens = ShuntingYard(tokens);
    //     for (auto token : parsedTokens) {
    //     // std::cout<<token->name<<", "<<token->type;
    //     if (token->type == TokenType::IDENTIFIER) {
    //         std::shared_ptr<Identifier> identifier = std::dynamic_pointer_cast<Identifier>(token);
    //         // std::cout<<identifier->idType<<std::endl;
    //     }  
    //     // std::cout<<std::endl;
    // }
    //     result = EvalReversePolishToks(parsedTokens)[0];
    // }
    // else {
    //     std::vector<std::shared_ptr<Token>> slicedTokens(tokens.begin() + stopIdx, tokens.end());

    //     auto parsedTokens = ShuntingYard(slicedTokens);
    //     result = EvalReversePolishToks(parsedTokens)[0];

    //     std::string varName = tokens[stopIdx-1]->name;
    //     SetVariable(varName, result);
    // }
    return tokens;  
}

int main() {
    // Open the file
    std::ifstream inputFile("firstScript.txt");

    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    // Read the file line by line and concatenate into a single string
    std::string result;
    std::string line;

    while (std::getline(inputFile, line)) {
        result += line;
    }

    // Close the file
    inputFile.close();

    // Now 'result' contains the content of the file as a single string
    std::cout << "Content of the file: " << result << std::endl;

    bool isAssign = false;
    size_t assignIdx = 0;

    auto rawTokens = TokenizeString(result);
    auto tokens = TransformTokens(rawTokens, isAssign, assignIdx);

    // for (auto token : rawTokens) {
    //     std::cout<<token.name<<", "<<token.type<<std::endl;  
    // }

    // std::cout<<"--------------"<<std::endl;
    // for (auto token : tokens) {
    //     std::cout<<token->name<<", "<<token->type<<std::endl;  
    // }

    size_t pos = 0;
    size_t prevPos = 0;
    InitMaps();

    while ((pos = result.find(';', prevPos)) != std::string::npos) {
        // Extract substring between semicolons
        std::string substring = result.substr(prevPos, pos - prevPos);

        // Output or process the substring
        auto out = ParseText(substring);
        std::cout<<"=> ";
        out.print();
        std::cout<<std::endl;

        // Move to the next semicolon position
        prevPos = pos + 1;
    }

    return 0;
}
