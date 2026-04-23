#include "TableauSolver.h"

#include "SignedFormula.h"
#include "TruthSign.h"

#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

Branch TableauSolver::makeInitialBranch(const FormulaPtr& formula) const {
    if (!formula) {
        throw std::invalid_argument("Cannot build tableau for null formula");
    }

    Branch branch;
    branch.addFormula(SignedFormula(TruthSign::False, formula));
    return branch;
}

std::map<std::string, bool> TableauSolver::completeAssignment(const Branch& branch,
                                                              const std::set<std::string>& variables) const {
    std::map<std::string, bool> assignment = branch.extractPartialAssignment();

    for (const std::string& variable : variables) {
        if (assignment.find(variable) == assignment.end()) {
            assignment[variable] = false;
        }
    }

    return assignment;
}

void TableauSolver::enforceLimits(const std::vector<Branch>& branches, std::size_t iterations) const {
    if (iterations > kMaxIterations) {
        throw std::runtime_error("Tableau iteration limit exceeded");
    }

    if (branches.size() > kMaxBranches) {
        throw std::runtime_error("Tableau branch limit exceeded");
    }

    for (const Branch& branch : branches) {
        if (branch.getFormulas().size() > kMaxFormulasInBranch) {
            throw std::runtime_error("Tableau branch size limit exceeded");
        }
    }
}

SolveResult TableauSolver::checkTautology(const FormulaPtr& formula, const std::set<std::string>& variables) const {
    RuleEngine ruleEngine;
    std::vector<Branch> branches;
    branches.push_back(makeInitialBranch(formula));

    bool progress = true;
    std::size_t iterations = 0;

    while (progress) {
        ++iterations;
        enforceLimits(branches, iterations);

        progress = false;
        std::vector<Branch> nextBranches;

        for (Branch& branch : branches) {
            if (branch.isClosed() || branch.isComplete()) {
                nextBranches.push_back(branch);
                continue;
            }

            std::optional<SignedFormula> target = branch.findNextAlpha();
            if (!target.has_value()) {
                target = branch.findNextBeta();
            }

            if (!target.has_value()) {
                nextBranches.push_back(branch);
                continue;
            }

            RuleApplicationResult result = ruleEngine.apply(*target);
            progress = true;

            if (!result.isBranching()) {
                Branch updated = branch;
                updated.markExpanded(*target);
                updated.addFormulas(result.getSameBranchFormulas());
                nextBranches.push_back(std::move(updated));
            } else {
                Branch left = branch;
                Branch right = branch;

                left.markExpanded(*target);
                right.markExpanded(*target);

                left.addFormulas(result.getLeftBranchFormulas());
                right.addFormulas(result.getRightBranchFormulas());

                nextBranches.push_back(std::move(left));
                nextBranches.push_back(std::move(right));
            }
        }

        branches = std::move(nextBranches);
    }

    for (const Branch& branch : branches) {
        if (branch.isOpen()) {
            return SolveResult(
                TableauStatus::NotTautology,
                "The formula is not a tautology.",
                completeAssignment(branch, variables)
            );
        }
    }

    return SolveResult(
        TableauStatus::Tautology,
        "The formula is a tautology."
    );
}
