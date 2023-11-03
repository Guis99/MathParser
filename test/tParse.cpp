#include "../MathParser.hpp"
using namespace MathParser;

int main() {

    InitMaps();

    MathParser::variables["x"] = QA(1, 3.4);
    MathParser::variables["y"] = QA(1, 1.0);

    std::string str;

    std::cout<<"Enter expression to evaluate: ";
    std::getline(std::cin,str);
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    std::cout<<std::endl<<str<<std::endl;

    auto out = EvalExpression(str);
}