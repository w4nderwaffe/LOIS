#pragma once

#include "Token.h"
#include <string>
#include <vector>

class Lexer {
public:
    std::vector<Token> tokenize(const std::string& input) const;

private:
    static bool isVariableStart(char ch);
    static bool isVariableChar(char ch);
};
