#pragma once

#include "../tableau/TableauStatus.h"

#include <map>
#include <optional>
#include <string>

class AnalysisResult {
public:
    AnalysisResult(std::string parsedFormula,
                   TableauStatus status,
                   std::string message,
                   std::optional<std::map<std::string, bool>> counterexample = std::nullopt);

    const std::string& getParsedFormula() const;
    TableauStatus getStatus() const;
    const std::string& getMessage() const;
    const std::optional<std::map<std::string, bool>>& getCounterexample() const;

private:
    std::string parsedFormula_;
    TableauStatus status_;
    std::string message_;
    std::optional<std::map<std::string, bool>> counterexample_;
};
