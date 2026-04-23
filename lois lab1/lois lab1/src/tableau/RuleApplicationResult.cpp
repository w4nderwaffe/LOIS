#include "RuleApplicationResult.h"

RuleApplicationResult RuleApplicationResult::makeAlpha(std::vector<SignedFormula> formulas) {
    RuleApplicationResult result;
    result.branching_ = false;
    result.sameBranchFormulas_ = std::move(formulas);
    return result;
}

RuleApplicationResult RuleApplicationResult::makeBeta(std::vector<SignedFormula> leftBranch,
                                                      std::vector<SignedFormula> rightBranch) {
    RuleApplicationResult result;
    result.branching_ = true;
    result.leftBranchFormulas_ = std::move(leftBranch);
    result.rightBranchFormulas_ = std::move(rightBranch);
    return result;
}

bool RuleApplicationResult::isBranching() const {
    return branching_;
}

const std::vector<SignedFormula>& RuleApplicationResult::getSameBranchFormulas() const {
    return sameBranchFormulas_;
}

const std::vector<SignedFormula>& RuleApplicationResult::getLeftBranchFormulas() const {
    return leftBranchFormulas_;
}

const std::vector<SignedFormula>& RuleApplicationResult::getRightBranchFormulas() const {
    return rightBranchFormulas_;
}
