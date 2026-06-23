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

#pragma once
#include "ast.h"
#include <string>
#include <vector>
#include <stdexcept>

// Типы токенов лексера
enum class TokenType {
    Variable,
    Not,       // !
    And,       // конъюнкция /backslash
    OrTok,     // дизъюнкция \/
    Implies,   // ->
    Equiv,     // ~
    LParen,    // (
    RParen,    // )
    EndOfInput
};

struct Token {
    TokenType type;
    std::string value;
};

// Лексер
class Lexer {
public:
    explicit Lexer(const std::string& input);
    std::vector<Token> tokenize();
private:
    std::string src;
    size_t pos;
    void skipSpaces();
    Token readVariable();
};

// Парсер (рекурсивный спуск)
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    FormulaPtr parse();
private:
    std::vector<Token> tokens;
    size_t pos;
    const Token& current() const;
    const Token& consume(TokenType expected);
    void advance();

    FormulaPtr parseEquiv();
    FormulaPtr parseImplication();
    FormulaPtr parseOr();
    FormulaPtr parseAnd();
    FormulaPtr parseNot();
    FormulaPtr parseAtom();
};

// Вспомогательная функция: лексинг + парсинг
FormulaPtr parseFormula(const std::string& input);
