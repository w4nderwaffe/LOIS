#pragma once

#include "RuleApplicationResult.h"

class RuleEngine {
public:
    RuleApplicationResult apply(const SignedFormula& formula) const;
};
