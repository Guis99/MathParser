#include "common.hpp"

#ifndef QUICKARRAY
#define QUICKARRAY

namespace MathParser {
    namespace QuickArray {
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

        template <typename T>
        QuickArray<T> QuickArrayPlus(const QuickArray<T>& QA1, const QuickArray<T> QA2) {
            QuickArray<T> result;
            result.reserve(QA1.size());

            for (size_t i = 0; i < QA1.size(); i++) {
                result.push_back(QA1[i] + QA2[i]);
            }
            return result;
        }

        template <typename T>
        QuickArray<T> QuickArrayMinus(const QuickArray<T>& QA1, const QuickArray<T> QA2) {
            QuickArray<T> result;
            result.reserve(QA1.size());

            for (size_t i = 0; i < QA1.size(); i++) {
                result.push_back(QA1[i] - QA2[i]);
            }
            return result;
        }

        template <typename T>
        QuickArray<T> QuickArrayMult(const QuickArray<T>& QA1, const QuickArray<T> QA2) {
            QuickArray<T> result;
            result.reserve(QA1.size());

            for (size_t i = 0; i < QA1.size(); i++) {
                result.push_back(QA1[i] * QA2[i]);
            }
            return result;
        }

        template <typename T>
        QuickArray<T> QuickArrayDivide(const QuickArray<T>& QA1, const QuickArray<T> QA2) {
            QuickArray<T> result;
            result.reserve(QA1.size());

            for (size_t i = 0; i < QA1.size(); i++) {
                result.push_back(QA1[i] / QA2[i]);
            }
            return result;
        }

        template <typename T>
        QuickArray<T> QuickArrayPower(const QuickArray<T>& QA1, const QuickArray<T> QA2) {
            QuickArray<T> result;
            result.reserve(QA1.size());

            for (size_t i = 0; i < QA1.size(); i++) {
                result.push_back(pow(QA1[i], QA2[i]));
            }
            return result;
        }
    }
}

#endif