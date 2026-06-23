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
#include <string>
#include <memory>
#include <set>

// Типы узлов AST
enum class NodeType {
    Variable,
    Negation,       // отрицание        !
    Conjunction,    // конъюнкция       /\ (без слэша в конце комментария)
    Disjunction,    // дизъюнкция       /
    Implication,    // импликация       ->
    Equivalence     // эквивалентность  ~
};

// Базовый узел формулы
struct FormulaNode {
    NodeType type;
    virtual ~FormulaNode() = default;
    explicit FormulaNode(NodeType t) : type(t) {}
};

using FormulaPtr = std::shared_ptr<FormulaNode>;

// Переменная
struct VariableNode : FormulaNode {
    std::string name;
    explicit VariableNode(const std::string& n)
        : FormulaNode(NodeType::Variable), name(n) {}
};

// Унарная операция (отрицание)
struct UnaryNode : FormulaNode {
    FormulaPtr operand;
    UnaryNode(NodeType t, FormulaPtr op)
        : FormulaNode(t), operand(std::move(op)) {}
};

// Бинарная операция
struct BinaryNode : FormulaNode {
    FormulaPtr left;
    FormulaPtr right;
    BinaryNode(NodeType t, FormulaPtr l, FormulaPtr r)
        : FormulaNode(t), left(std::move(l)), right(std::move(r)) {}
};

// Утилиты: печать и сбор переменных
struct FormulaPrinter {
    static std::string print(const FormulaPtr& node);
};

struct FormulaVariableCollector {
    static std::set<std::string> collect(const FormulaPtr& node);
};
