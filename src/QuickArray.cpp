#include "../include/QuickArray.hpp"

using namespace MathParser;

void QuickArray::print() const {
    std::cout<<"<";
    for (int i=0; i<this->size()-1; i++) {
        std::cout<<*(this->data.data()+i)<<", ";
    }
    std::cout<<*(this->data.end()-1)<<">"<<std::endl;
}

QuickArray QuickArray::applyBinaryOp(std::function<QuickArray(QuickArray, QuickArray)>& op, QuickArray &other) {
    int s1 = this->size();
    int s2 = other.size();

    if (s1 == s2) {
        return op(*this, other);
    }

    else if (s1 == 1) {
        QuickArray expandedArray(s2, *(this->data.data()));
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

double QuickArray::operator[](size_t idx) const {
    return data[idx];
}
