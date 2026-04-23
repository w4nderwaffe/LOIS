#include "FormulaAnalyzer.h"

#include "../ast/FormulaPrinter.h"
#include "../ast/FormulaVariableCollector.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"
#include "../tableau/SolveResult.h"
#include "../tableau/TableauSolver.h"

#include <set>
#include <string>
#include <vector>

AnalysisResult FormulaAnalyzer::analyze(const std::string& input) const {
    Lexer lexer;
    Parser parser;
    FormulaPrinter printer;
    FormulaVariableCollector collector;
    TableauSolver solver;

    std::vector<Token> tokens = lexer.tokenize(input);
    FormulaPtr formula = parser.parse(tokens);
    std::set<std::string> variables = collector.collect(formula);

    SolveResult solveResult = solver.checkTautology(formula, variables);

    return AnalysisResult(
        printer.toString(formula),
        solveResult.getStatus(),
        solveResult.getMessage(),
        solveResult.getCounterexample()
    );
}
