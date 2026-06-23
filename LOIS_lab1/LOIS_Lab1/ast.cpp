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

#include "ast.h"
#include <stdexcept>

// === FormulaPrinter ===
std::string FormulaPrinter::print(const FormulaPtr& node) {
    if (!node) return "";
    switch (node->type) {
        case NodeType::Variable: {
            auto* v = static_cast<VariableNode*>(node.get());
            return v->name;
        }
        case NodeType::Negation: {
            auto* u = static_cast<UnaryNode*>(node.get());
            return "!" + print(u->operand);
        }
        case NodeType::Conjunction: {
            auto* b = static_cast<BinaryNode*>(node.get());
            return "(" + print(b->left) + "/\\" + print(b->right) + ")";
        }
        case NodeType::Disjunction: {
            auto* b = static_cast<BinaryNode*>(node.get());
            return "(" + print(b->left) + "\\/" + print(b->right) + ")";
        }
        case NodeType::Implication: {
            auto* b = static_cast<BinaryNode*>(node.get());
            return "(" + print(b->left) + "->" + print(b->right) + ")";
        }
        case NodeType::Equivalence: {
            auto* b = static_cast<BinaryNode*>(node.get());
            return "(" + print(b->left) + "~" + print(b->right) + ")";
        }
    }
    return "";
}

// === FormulaVariableCollector ===
std::set<std::string> FormulaVariableCollector::collect(const FormulaPtr& node) {
    std::set<std::string> vars;
    if (!node) return vars;
    switch (node->type) {
        case NodeType::Variable: {
            auto* v = static_cast<VariableNode*>(node.get());
            vars.insert(v->name);
            break;
        }
        case NodeType::Negation: {
            auto* u = static_cast<UnaryNode*>(node.get());
            auto sub = collect(u->operand);
            vars.insert(sub.begin(), sub.end());
            break;
        }
        case NodeType::Conjunction:
        case NodeType::Disjunction:
        case NodeType::Implication:
        case NodeType::Equivalence: {
            auto* b = static_cast<BinaryNode*>(node.get());
            auto left  = collect(b->left);
            auto right = collect(b->right);
            vars.insert(left.begin(),  left.end());
            vars.insert(right.begin(), right.end());
            break;
        }
    }
    return vars;
}
