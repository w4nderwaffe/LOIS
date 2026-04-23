#include "Branch.h"

void Branch::addFormula(const SignedFormula& formula) {
    if (closed_) {
        return;
    }

    const std::string key = formula.toString();

    if (addedFormulas_.find(key) != addedFormulas_.end()) {
        return;
    }

    addedFormulas_.insert(key);
    formulas_.push_back(formula);

    if (!formula.isLiteral()) {
        return;
    }

    const std::string name = formula.getFormula()->getName();

    if (formula.getSign() == TruthSign::True) {
        trueAtoms_.insert(name);
        if (falseAtoms_.find(name) != falseAtoms_.end()) {
            closed_ = true;
        }
    } else {
        falseAtoms_.insert(name);
        if (trueAtoms_.find(name) != trueAtoms_.end()) {
            closed_ = true;
        }
    }
}

void Branch::addFormulas(const std::vector<SignedFormula>& formulas) {
    for (const SignedFormula& formula : formulas) {
        addFormula(formula);
    }
}

const std::vector<SignedFormula>& Branch::getFormulas() const {
    return formulas_;
}

bool Branch::isClosed() const {
    return closed_;
}

bool Branch::isOpen() const {
    return !closed_;
}

bool Branch::isExpanded(const SignedFormula& formula) const {
    return expandedFormulas_.find(formula.toString()) != expandedFormulas_.end();
}

void Branch::markExpanded(const SignedFormula& formula) {
    expandedFormulas_.insert(formula.toString());
}

std::optional<SignedFormula> Branch::findNextAlpha() const {
    if (closed_) {
        return std::nullopt;
    }

    for (const SignedFormula& formula : formulas_) {
        if (!isExpanded(formula) && formula.isAlpha()) {
            return formula;
        }
    }

    return std::nullopt;
}

std::optional<SignedFormula> Branch::findNextBeta() const {
    if (closed_) {
        return std::nullopt;
    }

    for (const SignedFormula& formula : formulas_) {
        if (!isExpanded(formula) && formula.isBeta()) {
            return formula;
        }
    }

    return std::nullopt;
}

bool Branch::isComplete() const {
    return !findNextAlpha().has_value() && !findNextBeta().has_value();
}

std::map<std::string, bool> Branch::extractPartialAssignment() const {
    std::map<std::string, bool> result;

    for (const std::string& atom : trueAtoms_) {
        result[atom] = true;
    }

    for (const std::string& atom : falseAtoms_) {
        result[atom] = false;
    }

    return result;
}
