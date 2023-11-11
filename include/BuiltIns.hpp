#include "common.hpp"

#include "Token.hpp"
#include "QuickArray.hpp"

typedef MathParser::QuickArray<double> QA;
typedef double (*mathFunc)(double);

namespace MathParser {
    // functions
    QA evalMathFunction(QA &input, mathFunc mathFuncToUse) {
        std::vector<double> outvec;
        outvec.reserve(input.size());

        for (int i=0; i<input.size(); i++) {
            outvec.emplace_back(mathFuncToUse(input[i]));
        }

        QA out(outvec);
        return out;
    }

    QA COMPOSE(const QA& repeatArray, const QA& inArray) {
        QA result;

        auto arraySize = inArray.size();
        int repeat = (int)repeatArray[0];

        for (int i=0; i<repeat; i++) {
            for (int j=0; j<arraySize; j++) {
                result.push_back(inArray[j]);
            }
        }
        return result;
    }

    QA INDEX(const QA& inArray, const QA& indexArray) {
        QA out;
        out.reserve(indexArray.size());

        int offset = 0;
        auto low = 0; auto high = inArray.size();

        for (int i=0; i<indexArray.size(); i++) {
            int idx = (int)(indexArray[i]);
            if (idx - offset >= high || idx - offset < 0) {
                throw InvalidArrayIndexException();
            }
            out.push_back(inArray[idx - offset]);
        }
        return out;
    }

    QA NEGATE(const QA& inArray) {
        QA out;
        out.reserve(inArray.size());

        for (int i=0; i<inArray.size(); i++) {
            out.push_back(-1*inArray[i]);
        }
        return out;
    }
}