#include "../MathParser.hpp"

using namespace MathParser;

int main() {

    // initialize QA
    QuickArray arr1 = {1,2,3};

    for (int i=0; i<100; i++) {
        arr1.push_back(i);
    }

    QuickArray arr2(1.2);
    std::vector<double> v1 = {1,2,3};
    QuickArray arr3(v1);

    QuickArray reparr(10,2);

    arr1.print();
    arr2.print();
    arr3.print();
    reparr.print();

    // Do operations on QA

    QuickArray arr4 = {1,2,3,4,5};
    QuickArray arr5 = {2,3,4,5,6};
    QuickArray scal = {3};

    auto testArr = arr4+arr5;
    testArr.print();
    testArr = arr4-arr5;
    testArr.print();
    testArr = arr4*arr5;
    testArr.print();
    testArr = arr4/arr5;
    testArr.print();
    testArr = arr4^arr5;
    testArr.print();
    std::function<QuickArray(QuickArray, QuickArray)> add = QuickArray::operator+;
    std::function<QuickArray(QuickArray, QuickArray)> sub = QuickArray::operator-;
    std::function<QuickArray(QuickArray, QuickArray)> mult = QuickArray::operator*;
    std::function<QuickArray(QuickArray, QuickArray)> div = QuickArray::operator/;
    std::function<QuickArray(QuickArray, QuickArray)> pow = QuickArray::operator^;
    

    auto prod1 = arr4.applyBinaryOp(mult, arr5);
    auto prod2 = arr4.applyBinaryOp(mult, scal);
    auto prod3 = scal.applyBinaryOp(mult, arr5);

    auto sum1 = arr4.applyBinaryOp(sub, arr5);
    auto sum2 = arr5.applyBinaryOp(sub, scal);
    auto sum3 = scal.applyBinaryOp(sub, arr4);


    prod1.print();
    prod2.print();
    prod3.print();

    sum1.print();
    sum2.print();
    sum3.print();


}