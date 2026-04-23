#include "AnalysisResult.h"

AnalysisResult::AnalysisResult(std::string parsedFormula,
                               TableauStatus status,
                               std::string message,
                               std::optional<std::map<std::string, bool>> counterexample)
    : parsedFormula_(std::move(parsedFormula)),
      status_(status),
      message_(std::move(message)),
      counterexample_(std::move(counterexample)) {}

const std::string& AnalysisResult::getParsedFormula() const {
    return parsedFormula_;
}

TableauStatus AnalysisResult::getStatus() const {
    return status_;
}

const std::string& AnalysisResult::getMessage() const {
    return message_;
}

const std::optional<std::map<std::string, bool>>& AnalysisResult::getCounterexample() const {
    return counterexample_;
}
