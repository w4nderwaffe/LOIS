#include "SolveResult.h"

SolveResult::SolveResult(TableauStatus status,
                         std::string message,
                         std::optional<std::map<std::string, bool>> counterexample)
    : status_(status),
      message_(std::move(message)),
      counterexample_(std::move(counterexample)) {}

TableauStatus SolveResult::getStatus() const {
    return status_;
}

const std::string& SolveResult::getMessage() const {
    return message_;
}

const std::optional<std::map<std::string, bool>>& SolveResult::getCounterexample() const {
    return counterexample_;
}
