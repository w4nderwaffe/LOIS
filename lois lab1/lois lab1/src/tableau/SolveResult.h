#pragma once

#include "TableauStatus.h"

#include <map>
#include <optional>
#include <string>

class SolveResult {
public:
    SolveResult(TableauStatus status,
                std::string message,
                std::optional<std::map<std::string, bool>> counterexample = std::nullopt);

    TableauStatus getStatus() const;
    const std::string& getMessage() const;
    const std::optional<std::map<std::string, bool>>& getCounterexample() const;

private:
    TableauStatus status_;
    std::string message_;
    std::optional<std::map<std::string, bool>> counterexample_;
};
