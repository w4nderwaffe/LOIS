#pragma once

#include <stdexcept>
#include <string>

class FormulaException : public std::runtime_error {
public:
    explicit FormulaException(const std::string& message)
        : std::runtime_error(message) {}
};

class LexerException : public FormulaException {
public:
    explicit LexerException(const std::string& message)
        : FormulaException(message) {}
};

class ParserException : public FormulaException {
public:
    explicit ParserException(const std::string& message)
        : FormulaException(message) {}
};
