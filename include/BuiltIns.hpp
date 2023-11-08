#include "common.hpp"

#include "Token.hpp"
#include "QuickArray.hpp"

namespace MathParser {
    typedef QuickArray QA;
    typedef double (*mathFunc)(double);

    QA evalMathFunction(QA &input, mathFunc mathFuncToUse) {
        std::vector<double> outvec;
        outvec.reserve(input.size());

        for (int i=0; i<input.size(); i++) {
            outvec.emplace_back(mathFuncToUse(input[i]));
        }

        QA out(outvec);
        return out;
    }

    QA COMPOSE(const QA& repeatArray, const QuickArray& inArray) {
        MathParser::QuickArray result;

        auto arraySize = inArray.size();
        int repeat = (int)repeatArray[0];

        for (int i=0; i<repeat; i++) {
            for (int j=0; j<arraySize; j++) {
                result.push_back(inArray[j]);
            }
        }
        return result;
    }

    QA INDEX(const QA& inArray, const QuickArray& indexArray) {
        QA out;
        out.reserve(indexArray.size());

        for (int i=0; i<indexArray.size(); i++) {
            int idx = (int)(indexArray[i]);
            out.push_back(inArray[idx]);
        }
        return out;
    }
}