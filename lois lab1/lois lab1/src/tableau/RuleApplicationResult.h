#pragma once

#include "SignedFormula.h"

#include <vector>

class RuleApplicationResult {
public:
    RuleApplicationResult() = default;

    static RuleApplicationResult makeAlpha(std::vector<SignedFormula> formulas);
    static RuleApplicationResult makeBeta(std::vector<SignedFormula> leftBranch,
                                          std::vector<SignedFormula> rightBranch);

    bool isBranching() const;

    const std::vector<SignedFormula>& getSameBranchFormulas() const;
    const std::vector<SignedFormula>& getLeftBranchFormulas() const;
    const std::vector<SignedFormula>& getRightBranchFormulas() const;

private:
    bool branching_ = false;
    std::vector<SignedFormula> sameBranchFormulas_;
    std::vector<SignedFormula> leftBranchFormulas_;
    std::vector<SignedFormula> rightBranchFormulas_;
};
