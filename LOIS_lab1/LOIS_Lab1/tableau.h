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
#include <vector>
#include <map>
#include <string>
#include <memory>

// Знак истинности формулы в tableau
enum class Sign { T, F };

// Подписанная формула: T(phi) или F(phi)
struct SignedFormula {
    Sign sign;
    FormulaPtr formula;
    SignedFormula(Sign s, FormulaPtr f) : sign(s), formula(std::move(f)) {}
    std::string toString() const;
};

// Ветвь аналитической таблицы
struct Branch {
    std::vector<SignedFormula> formulas;
    std::set<std::string>     trueAtoms;
    std::set<std::string>     falseAtoms;
    std::vector<bool>         expanded;
    bool                      closed;

    Branch() : closed(false) {}

    void addFormula(const SignedFormula& sf);
    bool hasUnexpanded() const;
    int  nextUnexpanded() const;
    std::map<std::string, bool> buildCounterexample() const;

private:
    void checkClosed();
    static bool isLiteral(const FormulaPtr& f);
};

// Результат работы решателя
struct TableauResult {
    bool isTautology;
    std::map<std::string, bool> counterexample;
};

// Решатель
class TableauSolver {
public:
    TableauResult solve(const FormulaPtr& formula);
private:
    std::vector<Branch> expand(Branch branch, int idx);
    bool   isClosed(Branch branch);
    Branch findOpenBranch(Branch branch);
};
