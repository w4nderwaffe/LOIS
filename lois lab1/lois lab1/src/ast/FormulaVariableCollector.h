#pragma once

#include "FormulaNode.h"

#include <set>
#include <string>

class FormulaVariableCollector {
public:
    std::set<std::string> collect(const FormulaPtr& formula) const;

private:
    void collectRecursive(const FormulaPtr& formula, std::set<std::string>& variables) const;
};
