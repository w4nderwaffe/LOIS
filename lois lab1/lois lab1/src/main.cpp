#include "app/AnalysisResult.h"
#include "app/FormulaAnalyzer.h"
#include "common/Exceptions.h"
#include "tableau/TableauStatus.h"

#include <cctype>
#include <iostream>
#include <string>

namespace {
bool isBlank(const std::string& text) {
    for (unsigned char ch : text) {
        if (!std::isspace(ch)) {
            return false;
        }
    }
    return true;
}

void printHelp() {
    std::cout << "Supported operations:\n";
    std::cout << "  !   - negation\n";
    std::cout << "  /\\  - conjunction\n";
    std::cout << "  \\/  - disjunction\n";
    std::cout << "  ->  - implication\n";
    std::cout << "  ~   - equivalence\n";
    std::cout << "Commands:\n";
    std::cout << "  help - show this help\n";
    std::cout << "  exit - quit the program\n";
}
}

int main() {
    FormulaAnalyzer analyzer;

    std::cout << "Tautology checker\n";
    printHelp();
    std::cout << '\n';

    while (true) {
        try {
            std::cout << "Enter formula: ";
            std::string input;
            std::getline(std::cin, input);

            if (!std::cin) {
                std::cout << "\nInput stream closed. Exiting.\n";
                break;
            }

            if (input == "exit") {
                std::cout << "Exiting.\n";
                break;
            }

            if (input == "help") {
                printHelp();
                std::cout << '\n';
                continue;
            }

            if (isBlank(input)) {
                std::cout << "Empty input. Please enter a formula.\n\n";
                continue;
            }

            AnalysisResult result = analyzer.analyze(input);

            std::cout << "Parsed formula: " << result.getParsedFormula() << '\n';
            std::cout << result.getMessage() << '\n';

            if (result.getStatus() == TableauStatus::NotTautology && result.getCounterexample().has_value()) {
                std::cout << "Counterexample:\n";
                for (const auto& [name, value] : *result.getCounterexample()) {
                    std::cout << "  " << name << " = " << (value ? 1 : 0) << '\n';
                }
            }

            std::cout << '\n';
        } catch (const FormulaException& e) {
            std::cerr << "Formula error: " << e.what() << "\n\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n\n";
        }
    }

    return 0;
}
