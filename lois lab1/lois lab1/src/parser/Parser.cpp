#include "Parser.h"

#include "../common/Exceptions.h"

#include <memory>

FormulaPtr Parser::parse(const std::vector<Token>& tokens) {
    if (tokens.empty()) {
        throw ParserException("Token sequence is empty");
    }

    tokens_ = &tokens;
    pos_ = 0;

    FormulaPtr result = parseEquiv();

    if (current().getType() != TokenType::End) {
        throw ParserException("Unexpected token after end of formula: " + current().getText());
    }

    return result;
}

FormulaPtr Parser::parseEquiv() {
    FormulaPtr left = parseImpl();

    while (match(TokenType::Equiv)) {
        FormulaPtr right = parseImpl();
        left = std::make_shared<FormulaNode>(FormulaType::Equiv, "", left, right);
    }

    return left;
}

FormulaPtr Parser::parseImpl() {
    FormulaPtr left = parseOr();

    if (match(TokenType::Impl)) {
        FormulaPtr right = parseImpl();
        return std::make_shared<FormulaNode>(FormulaType::Impl, "", left, right);
    }

    return left;
}

FormulaPtr Parser::parseOr() {
    FormulaPtr left = parseAnd();

    while (match(TokenType::Or)) {
        FormulaPtr right = parseAnd();
        left = std::make_shared<FormulaNode>(FormulaType::Or, "", left, right);
    }

    return left;
}

FormulaPtr Parser::parseAnd() {
    FormulaPtr left = parseUnary();

    while (match(TokenType::And)) {
        FormulaPtr right = parseUnary();
        left = std::make_shared<FormulaNode>(FormulaType::And, "", left, right);
    }

    return left;
}

FormulaPtr Parser::parseUnary() {
    if (match(TokenType::Not)) {
        FormulaPtr child = parseUnary();
        return std::make_shared<FormulaNode>(FormulaType::Not, "", child, nullptr);
    }

    return parsePrimary();
}

FormulaPtr Parser::parsePrimary() {
    if (match(TokenType::Variable)) {
        const Token& token = (*tokens_)[pos_ - 1];
        return std::make_shared<FormulaNode>(FormulaType::Variable, token.getText(), nullptr, nullptr);
    }

    if (match(TokenType::LParen)) {
        FormulaPtr inside = parseEquiv();
        consume(TokenType::RParen, "Expected ')'");
        return inside;
    }

    throw ParserException("Expected variable, '!' or '('");
}

const Token& Parser::current() const {
    if (tokens_ == nullptr || tokens_->empty()) {
        throw ParserException("Parser has no tokens");
    }

    if (pos_ >= tokens_->size()) {
        return tokens_->back();
    }

    return (*tokens_)[pos_];
}

bool Parser::match(TokenType type) {
    if (current().getType() == type) {
        ++pos_;
        return true;
    }
    return false;
}

const Token& Parser::consume(TokenType type, const char* message) {
    if (current().getType() != type) {
        throw ParserException(message);
    }
    return (*tokens_)[pos_++];
}
