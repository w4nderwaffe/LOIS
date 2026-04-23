#pragma once

#include "AnalysisResult.h"

#include <string>

class FormulaAnalyzer {
public:
    AnalysisResult analyze(const std::string& input) const;
};
