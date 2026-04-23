#include "FormulaVariableCollector.h"

#include <stdexcept>

std::set<std::string> FormulaVariableCollector::collect(const FormulaPtr& formula) const {
    if (!formula) {
        throw std::invalid_argument("Cannot collect variables from null formula");
    }

    std::set<std::string> variables;
    collectRecursive(formula, variables);
    return variables;
}

void FormulaVariableCollector::collectRecursive(const FormulaPtr& formula, std::set<std::string>& variables) const {
    if (!formula) {
        return;
    }

    if (formula->getType() == FormulaType::Variable) {
        variables.insert(formula->getName());
        return;
    }

    collectRecursive(formula->getLeft(), variables);
    collectRecursive(formula->getRight(), variables);
}
