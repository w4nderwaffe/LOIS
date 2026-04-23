#include "Lexer.h"
#include "../common/Exceptions.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <vector>

bool Lexer::isVariableStart(char ch) {
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

bool Lexer::isVariableChar(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

std::vector<Token> Lexer::tokenize(const std::string& input) const {
    std::vector<Token> tokens;
    std::size_t i = 0;

    while (i < input.size()) {
        char ch = input[i];

        if (std::isspace(static_cast<unsigned char>(ch))) {
            ++i;
            continue;
        }

        if (isVariableStart(ch)) {
            std::size_t start = i;
            ++i;
            while (i < input.size() && isVariableChar(input[i])) {
                ++i;
            }
            tokens.emplace_back(TokenType::Variable, input.substr(start, i - start));
            continue;
        }

        if (ch == '!') {
            tokens.emplace_back(TokenType::Not, "!");
            ++i;
            continue;
        }

        if (ch == '/') {
            if (i + 1 < input.size() && input[i + 1] == '\\') {
                tokens.emplace_back(TokenType::And, "/\\");
                i += 2;
                continue;
            }
            throw LexerException("Invalid token: expected '/\\\\' for conjunction");
        }

        if (ch == '\\') {
            if (i + 1 < input.size() && input[i + 1] == '/') {
                tokens.emplace_back(TokenType::Or, "\\/");
                i += 2;
                continue;
            }
            throw LexerException("Invalid token: expected '\\/' for disjunction");
        }

        if (ch == '~') {
            tokens.emplace_back(TokenType::Equiv, "~");
            ++i;
            continue;
        }

        if (ch == '(') {
            tokens.emplace_back(TokenType::LParen, "(");
            ++i;
            continue;
        }

        if (ch == ')') {
            tokens.emplace_back(TokenType::RParen, ")");
            ++i;
            continue;
        }

        if (ch == '-') {
            if (i + 1 < input.size() && input[i + 1] == '>') {
                tokens.emplace_back(TokenType::Impl, "->");
                i += 2;
                continue;
            }
            throw LexerException("Invalid token: expected '->'");
        }

        throw LexerException(std::string("Invalid character: '") + ch + "'");
    }

    tokens.emplace_back(TokenType::End, "");
    return tokens;
}
