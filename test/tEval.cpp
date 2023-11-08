#include "../MathParser.hpp"
using namespace MathParser;

int main() {
    MathParser::InitMaps();
    MathParser::SetVariable("x", QA(10,3.4));
    MathParser::SetVariable("y", QA(2.0));

    // MathParser::variables["x"].print();
    // MathParser::variables["y"].print();
    std::string str; std::string vars;

    std::cout<<"Enter expression to evaluate: ";
    std::getline(std::cin,str);
    std::cout<<std::endl<<str<<std::endl;

    auto out = ParseText(str);
    std::cout<<"=> ";
    out.print();
    std::cout<<std::endl;

    while (true) {
        str = "";
        std::cout<<"input: ";
        std::getline(std::cin,str);
        std::cout<<std::endl;

        auto out = ParseText(str);
        std::cout<<"=> ";
        out.print();
        std::cout<<std::endl;
    }

}