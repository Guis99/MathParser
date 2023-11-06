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

    for (auto token : tokens) {
        std::cout<<token->name<<", "<<token->type<<std::endl;  
    }
}