#include "FormulaPrinter.h"

#include <stdexcept>

std::string FormulaPrinter::toString(const FormulaPtr& formula) const {
    if (!formula) {
        throw std::invalid_argument("Cannot print null formula");
    }
    return print(formula);
}

std::string FormulaPrinter::print(const FormulaPtr& formula) const {
    if (formula->isVariable()) {
        return formula->getName();
    }

    switch (formula->getType()) {
        case FormulaType::Not:
            return "(!" + print(formula->getLeft()) + ")";
        case FormulaType::And:
            return "(" + print(formula->getLeft()) + "/\\" + print(formula->getRight()) + ")";
        case FormulaType::Or:
            return "(" + print(formula->getLeft()) + "\\/" + print(formula->getRight()) + ")";
        case FormulaType::Impl:
            return "(" + print(formula->getLeft()) + "->" + print(formula->getRight()) + ")";
        case FormulaType::Equiv:
            return "(" + print(formula->getLeft()) + "~" + print(formula->getRight()) + ")";
        case FormulaType::Variable:
            return formula->getName();
    }

    throw std::runtime_error("Unknown formula node type");
}
