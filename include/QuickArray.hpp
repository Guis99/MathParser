#include "common.hpp"

#ifndef QUICKARRAY
#define QUICKARRAY

namespace MathParser {

    class QuickArray : public std::vector<double> {
        public:
        // QuickArray(size_t size, double value) : std::vector<double>(size, value) {};
        using std::vector<double>::vector;

        void print();

        QuickArray applyBinaryOp(std::function<QuickArray(QuickArray, QuickArray)> &op, QuickArray &other);

        QuickArray operator+(const QuickArray& other);
        QuickArray operator-(const QuickArray& other);
        QuickArray operator*(const QuickArray& other);
        QuickArray operator/(const QuickArray& other);
        QuickArray operator^(const QuickArray& other);
    };
    
    class IncompatibleArraySizesException : public std::exception {
        public:
            IncompatibleArraySizesException(const std::string& message) : message(message) {}

            // Override the what() function to provide a description of the exception.
            const char* what() const noexcept override {
                return message.c_str();
            }

        private:
            std::string message;
    };
}

#endif