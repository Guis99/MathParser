#include "common.hpp"

#ifndef QUICKARRAY
#define QUICKARRAY

namespace MathParser {

    class QuickArray {
        public:
        //Constructors
        QuickArray() {};

        QuickArray(std::vector<double> data) {
            this->data = data;
        }

        QuickArray(double data) {
            this->data = {data};
        }

        QuickArray(std::initializer_list<double> values) : data(values) {};

        QuickArray(size_t size, double value) : data(size, value) {};

        // std::vector methods I want
        size_t size() const {
            return data.size();
        };

        void reserve(size_t newCapacity) {
            data.reserve(newCapacity);
        }

        void push_back(double newElement) {
            data.push_back(newElement);
        }

        auto get_data() const {
            return this->data.data();
        }

        auto begin() const {
            return this->data.begin();
        }

        auto end() const {
            return this->data.end();
        }

        void print() const;

        QuickArray applyBinaryOp(std::function<QuickArray(QuickArray, QuickArray)> &op, QuickArray &other);

        QuickArray operator+(const QuickArray& other);
        QuickArray operator-(const QuickArray& other);
        QuickArray operator*(const QuickArray& other);
        QuickArray operator/(const QuickArray& other);
        QuickArray operator^(const QuickArray& other);
        
        double operator[](size_t idx) const;

        private:
        std::vector<double> data;

    };

    QuickArray QAofQA(const QuickArray& repeatArray, const QuickArray& inArray);

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