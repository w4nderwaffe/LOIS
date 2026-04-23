#pragma once

#include "../ast/FormulaNode.h"
#include "TruthSign.h"

#include <string>

class SignedFormula {
public:
    SignedFormula(TruthSign sign, FormulaPtr formula);

    TruthSign getSign() const;
    FormulaPtr getFormula() const;

    bool isLiteral() const;
    bool isAtomic() const;
    bool isAlpha() const;
    bool isBeta() const;

    std::string toString() const;

private:
    TruthSign sign_;
    FormulaPtr formula_;
};
