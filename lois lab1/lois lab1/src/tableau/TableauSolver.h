#pragma once

#include "../ast/FormulaNode.h"
#include "Branch.h"
#include "RuleEngine.h"
#include "SolveResult.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class TableauSolver {
public:
    SolveResult checkTautology(const FormulaPtr& formula, const std::set<std::string>& variables) const;

private:
    static constexpr std::size_t kMaxIterations = 10000;
    static constexpr std::size_t kMaxBranches = 10000;
    static constexpr std::size_t kMaxFormulasInBranch = 10000;

    Branch makeInitialBranch(const FormulaPtr& formula) const;
    std::map<std::string, bool> completeAssignment(const Branch& branch,
                                                   const std::set<std::string>& variables) const;
    void enforceLimits(const std::vector<Branch>& branches, std::size_t iterations) const;
};
