#include "../include/QuickArray.hpp"

using namespace MathParser;

void QuickArray::print() {
    for (int i=0; i<this->size(); i++) {
        if (i < this->size() - 1) {
            std::cout<<*(this->data()+i)<<", ";
        }
        else {
            std::cout<<*(this->data()+i)<<std::endl;
        }
    }
}

QuickArray QuickArray::applyBinaryOp(std::function<QuickArray(QuickArray, QuickArray)>& op, QuickArray &other) {
    int s1 = this->size();
    int s2 = other.size();

    if (s1 == s2) {
        return op(*this, other);
    }

    else if (s1 == 1) {
        QuickArray expandedArray(s2, *(this->data()));
        return op(expandedArray, other);
    }

    else if (s2 == 1) {
        QuickArray expandedArray(s1, other[0]);
        return op(*this, expandedArray);
    }

    else {
        throw IncompatibleArraySizesException("Array sizes are incompatible.");
    }
}

QuickArray QuickArray::operator+(const QuickArray& other) {
    QuickArray result;
    result.reserve(this->size());

    for (size_t i = 0; i < this->size(); i++) {
        result.push_back((*this)[i] + other[i]);
    }
    return result;
}

QuickArray QuickArray::operator-(const QuickArray& other) {
    QuickArray result;
    result.reserve(this->size());

    for (size_t i = 0; i < this->size(); i++) {
        result.push_back((*this)[i] - other[i]);
    }
    return result;
}

QuickArray QuickArray::operator*(const QuickArray& other) {
    QuickArray result;
    result.reserve(this->size());

    for (size_t i = 0; i < this->size(); i++) {
        result.push_back((*this)[i] * other[i]);
    }
    return result;
}

QuickArray QuickArray::operator/(const QuickArray& other) {
    QuickArray result;
    result.reserve(this->size());

    for (size_t i = 0; i < this->size(); i++) {
        result.push_back((*this)[i] / other[i]);
    }
    return result;
}

QuickArray QuickArray::operator^(const QuickArray& other) {
    QuickArray result;
    result.reserve(this->size());

    for (size_t i = 0; i < this->size(); i++) {
        result.push_back(std::pow((*this)[i], other[i]));
    }
    return result;
}
