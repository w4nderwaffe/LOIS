#pragma once

#include "../ast/FormulaNode.h"
#include "../lexer/Token.h"

#include <cstddef>
#include <vector>

class Parser {
public:
    FormulaPtr parse(const std::vector<Token>& tokens);

private:
    const std::vector<Token>* tokens_ = nullptr;
    std::size_t pos_ = 0;

    FormulaPtr parseEquiv();
    FormulaPtr parseImpl();
    FormulaPtr parseOr();
    FormulaPtr parseAnd();
    FormulaPtr parseUnary();
    FormulaPtr parsePrimary();

    const Token& current() const;
    bool match(TokenType type);
    const Token& consume(TokenType type, const char* message);
};
