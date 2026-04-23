#include "SignedFormula.h"

#include "../ast/FormulaPrinter.h"

#include <stdexcept>

SignedFormula::SignedFormula(TruthSign sign, FormulaPtr formula)
    : sign_(sign), formula_(std::move(formula)) {
    if (!formula_) {
        throw std::invalid_argument("SignedFormula cannot contain null formula");
    }
}

TruthSign SignedFormula::getSign() const {
    return sign_;
}

FormulaPtr SignedFormula::getFormula() const {
    return formula_;
}

bool SignedFormula::isLiteral() const {
    return formula_->getType() == FormulaType::Variable;
}

bool SignedFormula::isAtomic() const {
    return formula_->getType() == FormulaType::Variable;
}

bool SignedFormula::isAlpha() const {
    switch (formula_->getType()) {
        case FormulaType::Variable:
            return false;
        case FormulaType::Not:
            return true;
        case FormulaType::And:
            return sign_ == TruthSign::True;
        case FormulaType::Or:
            return sign_ == TruthSign::False;
        case FormulaType::Impl:
            return sign_ == TruthSign::False;
        case FormulaType::Equiv:
            return false;
    }

    return false;
}

bool SignedFormula::isBeta() const {
    switch (formula_->getType()) {
        case FormulaType::Variable:
            return false;
        case FormulaType::Not:
            return false;
        case FormulaType::And:
            return sign_ == TruthSign::False;
        case FormulaType::Or:
            return sign_ == TruthSign::True;
        case FormulaType::Impl:
            return sign_ == TruthSign::True;
        case FormulaType::Equiv:
            return true;
    }

    return false;
}

std::string SignedFormula::toString() const {
    FormulaPrinter printer;
    const std::string prefix = sign_ == TruthSign::True ? "T(" : "F(";
    return prefix + printer.toString(formula_) + ")";
}
