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

    QA SUM(const QA& inArray) {
        double out(0);

        for (int i=0; i<inArray.size(); i++) {
            out += inArray[i];
        }

        return out;
    }

    QA NORM(QA& inArray, QA& p) {
        QA out;

        for (int i=0; i<inArray.size(); i++) {
            out.push_back(std::pow(inArray[i],p[0]));
        }

        out = SUM(out);
        auto result = QA(std::pow(out[0],1/p[0]));

        return result;
    }

    QA SHAPE(const QA& inArray) {
        return QA(inArray.size());
    }

    QA RANGE(const QA& start, const QA& end) {
        QA out;
        auto curr = start[0];
        double incr = 1;
        auto last = end[0];

        auto estElem = (int)((last-curr)/incr) + 1;

        while (curr <= last) {
            out.push_back(curr);
            curr += incr;
        }
        return out;
    }

    QA RANGE(const QA& start, const QA& increment, const QA& end) {
        QA out;
        auto curr = start[0];
        auto incr = increment[0];
        auto last = end[0];

        auto estElem = (int)((last-curr)/incr) + 1;

        while (curr <= last) {
            out.push_back(curr);
            curr += incr;
        }
        return out;
    }

    QA LINSPACE(const QA& start, const QA& end, const QA& numElms) {
        QA out;
        out.reserve((int)numElms[0]);
        auto curr = start[0];
        auto last = end[0];
        auto incr = (last-curr)/(numElms[0]-1);
        while (curr <= last) {
            out.push_back(curr);
            curr += incr;
        }
        return out;
    }
    
}