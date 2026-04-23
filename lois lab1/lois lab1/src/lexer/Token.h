#pragma once

#include <string>

enum class TokenType {
    Variable,
    Not,
    And,
    Or,
    Impl,
    Equiv,
    LParen,
    RParen,
    End
};

class Token {
public:
    Token() = default;
    Token(TokenType type, std::string text)
        : type_(type), text_(std::move(text)) {}

    TokenType getType() const {
        return type_;
    }

    const std::string& getText() const {
        return text_;
    }

private:
    TokenType type_ = TokenType::End;
    std::string text_;
};
