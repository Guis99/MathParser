#include "../MathParser.hpp"
using namespace MathParser;

int main() {
    MathParser::InitMaps();
    MathParser::SetVariable("x", QA(10,3.4));
    MathParser::SetVariable("y", QA(2.0));

    MathParser::variables["x"].print();
    MathParser::variables["y"].print();
    std::string str; std::string vars;

    std::cout<<"Enter expression to evaluate: ";
    std::getline(std::cin,str);
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    std::cout<<std::endl<<str<<std::endl;

    auto out = ParseText(str);
    out.print();

    while (true) {
        str = "";
        std::cout<<"input: ";
        std::getline(std::cin,str);
        str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
        std::cout<<std::endl;

        auto out = ParseText(str);
        out.print();
    }

}