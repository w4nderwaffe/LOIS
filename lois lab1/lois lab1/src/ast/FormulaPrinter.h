#pragma once

#include "FormulaNode.h"
#include <string>

class FormulaPrinter {
public:
    std::string toString(const FormulaPtr& formula) const;

private:
    std::string print(const FormulaPtr& formula) const;
};
