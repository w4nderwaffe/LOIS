// Лабораторная работа №1 по дисциплине Логические Основы Интеллектуальных Систем
// Выполнена студентом группы 421701 БГУИР Аникеенко Андреем Дмитриевичем
//
// Задание:
// Проверка формулы сокращённого языка логики высказываний на общезначимость (тавтологию)
//
// 14.05.2026 версия 1.0
//
// Использованные источники:
// Справочная система по дисциплине ЛОИС
// Логические основы интеллектуальных систем. Практикум
// «Семантическое следование и формальная выводимость» (Semantic entailment and formal derivability) / Эверт В. Бет (E. W. Beth)
// «Компиляторы: принципы, технологии и инструментарий» (Compilers: Principles, Techniques, and Tools). / Альфред Ахо, Моника Лам, Рави Сети, Джеффри Ульман (Alfred V. Aho, Monica S. Lam, Ravi Sethi, Jeffrey D. Ullman).

#include "parser.h"
#include <stdexcept>
#include <cctype>

// === ЛЕКСЕР ===

Lexer::Lexer(const std::string& input) : src(input), pos(0) {}

void Lexer::skipSpaces() {
    while (pos < src.size() && std::isspace((unsigned char)src[pos]))
        ++pos;
}

Token Lexer::readVariable() {
    std::string name;
    while (pos < src.size() && (std::isalnum((unsigned char)src[pos]) || src[pos] == '_'))
        name += src[pos++];
    return {TokenType::Variable, name};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < src.size()) {
        skipSpaces();
        if (pos >= src.size()) break;
        char c = src[pos];

        if (std::isalpha((unsigned char)c)) {
            tokens.push_back(readVariable());
            continue;
        }
        // Отрицание
        if (c == '!') {
            tokens.push_back({TokenType::Not, "!"});
            ++pos; continue;
        }
        // Конъюнкция /\ и дизъюнкция \/
        if (c == '/' && pos + 1 < src.size() && src[pos + 1] == '\\') {
            tokens.push_back({TokenType::And, "/\\"});
            pos += 2; continue;
        }
        if (c == '\\' && pos + 1 < src.size() && src[pos + 1] == '/') {
            tokens.push_back({TokenType::OrTok, "\\/"});
            pos += 2; continue;
        }
        // Импликация ->
        if (c == '-') {
            if (pos + 1 < src.size() && src[pos + 1] == '>') {
                tokens.push_back({TokenType::Implies, "->"});
                pos += 2;
            } else {
                throw std::runtime_error("Лексер: неожиданный символ '-' (ожидалось '->')");
            }
            continue;
        }
        // Эквивалентность ~
        if (c == '~') {
            tokens.push_back({TokenType::Equiv, "~"});
            ++pos; continue;
        }
        // Скобки
        if (c == '(') { tokens.push_back({TokenType::LParen, "("}); ++pos; continue; }
        if (c == ')') { tokens.push_back({TokenType::RParen, ")"}); ++pos; continue; }

        throw std::runtime_error(std::string("Лексер: недопустимый символ '") + c + "'");
    }
    tokens.push_back({TokenType::EndOfInput, ""});
    return tokens;
}

// === ПАРСЕР ===

Parser::Parser(const std::vector<Token>& toks) : tokens(toks), pos(0) {}

const Token& Parser::current() const { return tokens[pos]; }

void Parser::advance() {
    if (pos + 1 < tokens.size()) ++pos;
}

const Token& Parser::consume(TokenType expected) {
    if (current().type != expected)
        throw std::runtime_error("Парсер: неожиданный токен '" + current().value + "'");
    const Token& t = current();
    advance();
    return t;
}

FormulaPtr Parser::parse() {
    auto formula = parseEquiv();
    if (current().type != TokenType::EndOfInput)
        throw std::runtime_error("Парсер: лишние символы после формулы: '" + current().value + "'");
    return formula;
}

// Уровень 1: эквивалентность (~) — левоассоциативно
FormulaPtr Parser::parseEquiv() {
    auto left = parseImplication();
    while (current().type == TokenType::Equiv) {
        advance();
        auto right = parseImplication();
        left = std::make_shared<BinaryNode>(NodeType::Equivalence, std::move(left), std::move(right));
    }
    return left;
}

// Уровень 2: импликация (->) — правоассоциативно
FormulaPtr Parser::parseImplication() {
    auto left = parseOr();
    if (current().type == TokenType::Implies) {
        advance();
        auto right = parseImplication();
        return std::make_shared<BinaryNode>(NodeType::Implication, std::move(left), std::move(right));
    }
    return left;
}

// Уровень 3: дизъюнкция (\/) — левоассоциативно
FormulaPtr Parser::parseOr() {
    auto left = parseAnd();
    while (current().type == TokenType::OrTok) {
        advance();
        auto right = parseAnd();
        left = std::make_shared<BinaryNode>(NodeType::Disjunction, std::move(left), std::move(right));
    }
    return left;
}

// Уровень 4: конъюнкция (/\) — левоассоциативно
FormulaPtr Parser::parseAnd() {
    auto left = parseNot();
    while (current().type == TokenType::And) {
        advance();
        auto right = parseNot();
        left = std::make_shared<BinaryNode>(NodeType::Conjunction, std::move(left), std::move(right));
    }
    return left;
}

// Уровень 5: отрицание (!) — правоассоциативно
FormulaPtr Parser::parseNot() {
    if (current().type == TokenType::Not) {
        advance();
        auto operand = parseNot();
        return std::make_shared<UnaryNode>(NodeType::Negation, std::move(operand));
    }
    return parseAtom();
}

// Уровень 6: атом
FormulaPtr Parser::parseAtom() {
    if (current().type == TokenType::Variable) {
        std::string name = current().value;
        advance();
        return std::make_shared<VariableNode>(name);
    }
    if (current().type == TokenType::LParen) {
        advance();
        auto expr = parseEquiv();
        consume(TokenType::RParen);
        return expr;
    }
    throw std::runtime_error("Парсер: ожидалась переменная или '(', получено '" + current().value + "'");
}

// Вспомогательная обёртка
FormulaPtr parseFormula(const std::string& input) {
    if (input.empty())
        throw std::runtime_error("Ошибка: пустая формула");

    int balance = 0;
    for (char c : input) {
        if (c == '(') ++balance;
        if (c == ')') --balance;
        if (balance < 0)
            throw std::runtime_error("Ошибка: лишняя закрывающая скобка ')'");
    }
    if (balance != 0)
        throw std::runtime_error("Ошибка: незакрытая открывающая скобка '('");

    Lexer lexer(input);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    return parser.parse();
}
