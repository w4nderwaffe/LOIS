#pragma once

#include "SignedFormula.h"

#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

class Branch {
public:
    Branch() = default;

    void addFormula(const SignedFormula& formula);
    void addFormulas(const std::vector<SignedFormula>& formulas);

    const std::vector<SignedFormula>& getFormulas() const;

    bool isClosed() const;
    bool isOpen() const;

    bool isExpanded(const SignedFormula& formula) const;
    void markExpanded(const SignedFormula& formula);

    std::optional<SignedFormula> findNextAlpha() const;
    std::optional<SignedFormula> findNextBeta() const;
    bool isComplete() const;

    std::map<std::string, bool> extractPartialAssignment() const;

private:
    std::vector<SignedFormula> formulas_;
    std::set<std::string> addedFormulas_;
    std::set<std::string> expandedFormulas_;
    std::set<std::string> trueAtoms_;
    std::set<std::string> falseAtoms_;
    bool closed_ = false;
};
