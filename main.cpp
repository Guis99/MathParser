#include "./MathParser.hpp"

int main() {
    std::string str; std::string vars;
    std::cout<<"Submit expression to evaluate: ";
    std::getline(std::cin,str);
    std::cout<<std::endl<<"Specify independent variables: ";
    std::cin>>vars;
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    std::cout<<std::endl<<str<<std::endl;

    auto out = MathParser::EvalExpression(str, vars);
}