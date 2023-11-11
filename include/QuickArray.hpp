#include "common.hpp"

#ifndef QUICKARRAY
#define QUICKARRAY

namespace MathParser {

    template <typename T>
    class QuickArray {
        public:
        //Constructors
        QuickArray() {};

        QuickArray(std::vector<T> &data) {
            this->data = data;
        }

        QuickArray(T data) {
            this->data = {data};
        }

        QuickArray(std::initializer_list<T> values) : data(values) {};

        QuickArray(size_t size, T value) : data(size, value) {};

        // std::vector methods I want
        size_t size() const {
            return data.size();
        };

        void reserve(size_t newCapacity) {
            data.reserve(newCapacity);
        }

        void push_back(T newElement) {
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

        void print() const {
            std::cout<<"<";
            for (int i=0; i<this->size()-1; i++) {
                std::cout<<*(this->data.data()+i)<<", ";
            }
            std::cout<<*(this->data.end()-1)<<">"<<std::endl;
        }

        QuickArray<T> applyBinaryOp(std::function<QuickArray<T>(QuickArray<T>, QuickArray<T>)>& op, QuickArray<T> &other) {
            int s1 = this->size();
            int s2 = other.size();

            if (s1 == s2) {
                return op(*this, other);
            }

            else if (s1 == 1) {
                QuickArray<T> expandedArray(s2, *(this->data.data()));
                return op(expandedArray, other);
            }

            else if (s2 == 1) {
                QuickArray<T> expandedArray(s1, other[0]);
                return op(*this, expandedArray);
            }

            else {
                throw IncompatibleArraySizesException();
                QuickArray<T> defaultreturn;
                return defaultreturn;
            }
        }

        QuickArray<T> operator+(const QuickArray<T>& other) {
            QuickArray<T> result;
            result.reserve(this->size());

            for (size_t i = 0; i < this->size(); i++) {
                result.push_back((*this)[i] + other[i]);
            }
            return result;
        }

        QuickArray<T> operator-(const QuickArray<T>& other) {
            QuickArray<T> result;
            result.reserve(this->size());

            for (size_t i = 0; i < this->size(); i++) {
                result.push_back((*this)[i] - other[i]);
            }
            return result;
        }

        QuickArray<T> operator*(const QuickArray<T>& other) {
            QuickArray<T> result;
            result.reserve(this->size());

            for (size_t i = 0; i < this->size(); i++) {
                result.push_back((*this)[i] * other[i]);
            }
            return result;
        }

        QuickArray<T> operator/(const QuickArray<T>& other) {
            QuickArray<T> result;
            result.reserve(this->size());

            for (size_t i = 0; i < this->size(); i++) {
                result.push_back((*this)[i] / other[i]);
            }
            return result;
        }

        QuickArray<T> operator^(const QuickArray<T>& other) {
            QuickArray<T> result;
            result.reserve(this->size());

            for (size_t i = 0; i < this->size(); i++) {
                result.push_back(std::pow((*this)[i], other[i]));
            }
            return result;
        }

        T operator[](size_t idx) const {
            return data[idx];
        }

        private:
        std::vector<T> data;

        class IncompatibleArraySizesException : public std::exception {
            public:
                IncompatibleArraySizesException() {
                    message = "Array sizes are incompatible.";
                };
                const char* what() const noexcept override {
                    return message.c_str();
                }

            private:
                std::string message;
        };
    };
}

#endif