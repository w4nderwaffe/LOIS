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

#include "tableau.h"
#include <stdexcept>
#include <stack>

// === SignedFormula ===

std::string SignedFormula::toString() const {
    return (sign == Sign::T ? "T" : "F") +
           std::string("(") + FormulaPrinter::print(formula) + ")";
}

// === Branch ===

// Является ли формула атомом (переменной)
static bool isVariable(const FormulaPtr& f) {
    return f->type == NodeType::Variable;
}

// Является ли формула литералом — переменной или её отрицанием
bool Branch::isLiteral(const FormulaPtr& f) {
    if (f->type == NodeType::Variable) return true;
    if (f->type == NodeType::Negation) {
        auto* u = static_cast<UnaryNode*>(f.get());
        return u->operand->type == NodeType::Variable;
    }
    return false;
}

void Branch::checkClosed() {
    for (auto& v : trueAtoms)
        if (falseAtoms.count(v)) { closed = true; return; }
}

void Branch::addFormula(const SignedFormula& sf) {
    formulas.push_back(sf);
    expanded.push_back(false);

    // Если это переменная — сразу регистрируем и помечаем раскрытой
    if (isVariable(sf.formula)) {
        auto* v = static_cast<VariableNode*>(sf.formula.get());
        if (sf.sign == Sign::T) trueAtoms.insert(v->name);
        else                    falseAtoms.insert(v->name);
        expanded.back() = true;
        checkClosed();
        return;
    }

    // Если это !X (отрицание переменной) — раскрываем немедленно,
    // добавляя противоположно знак переменной
    if (sf.formula->type == NodeType::Negation) {
        auto* u = static_cast<UnaryNode*>(sf.formula.get());
        if (isVariable(u->operand)) {
            auto* v = static_cast<VariableNode*>(u->operand.get());
            // T(!X) -> X ложна, F(!X) -> X истинна
            if (sf.sign == Sign::T) falseAtoms.insert(v->name);
            else                    trueAtoms.insert(v->name);
            expanded.back() = true;
            checkClosed();
        }
    }
}

bool Branch::hasUnexpanded() const {
    for (size_t i = 0; i < formulas.size(); ++i)
        if (!expanded[i])
            return true;
    return false;
}

int Branch::nextUnexpanded() const {
    for (size_t i = 0; i < formulas.size(); ++i)
        if (!expanded[i])
            return (int)i;
    return -1;
}

std::map<std::string, bool> Branch::buildCounterexample() const {
    std::map<std::string, bool> ce;
    for (auto& v : trueAtoms)  ce[v] = true;
    for (auto& v : falseAtoms) ce[v] = false;
    return ce;
}

// === TableauSolver: правила раскрытия ===
//
// alpha-правила (без ветвления):
//   T(A/\B)  -> T(A), T(B)
//   F(A\/B)  -> F(A), F(B)
//   F(A->B)  -> T(A), F(B)
//   T(!A)    -> F(A)           (обрабатывается в addFormula для атомов,
//   F(!A)    -> T(A)            для сложных — здесь)
//   T(A~B)   -> T(A->B), T(B->A)
//
// beta-правила (ветвление):
//   F(A/\B)  -> F(A) | F(B)
//   T(A\/B)  -> T(A) | T(B)
//   T(A->B)  -> F(A) | T(B)
//   F(A~B)   -> F(A->B) | F(B->A)

std::vector<Branch> TableauSolver::expand(Branch branch, int idx) {
    branch.expanded[idx] = true;
    const SignedFormula& sf = branch.formulas[idx];
    Sign     s = sf.sign;
    NodeType t = sf.formula->type;

    // Отрицание сложной формулы (не переменной — те обработаны в addFormula)
    if (t == NodeType::Negation) {
        auto* u = static_cast<UnaryNode*>(sf.formula.get());
        // Двойное отрицание или отрицание сложной подформулы
        Sign newSign = (s == Sign::T) ? Sign::F : Sign::T;
        branch.addFormula({newSign, u->operand});
        if (branch.closed) return {};
        return {branch};
    }

    // Конъюнкция
    if (t == NodeType::Conjunction) {
        auto* b = static_cast<BinaryNode*>(sf.formula.get());
        if (s == Sign::T) {
            // alpha: T(A/\B) -> T(A), T(B)
            branch.addFormula({Sign::T, b->left});
            if (!branch.closed) branch.addFormula({Sign::T, b->right});
            if (branch.closed) return {};
            return {branch};
        } else {
            // beta: F(A/\B) -> F(A) | F(B)
            Branch b1 = branch, b2 = branch;
            b1.addFormula({Sign::F, b->left});
            b2.addFormula({Sign::F, b->right});
            std::vector<Branch> result;
            if (!b1.closed) result.push_back(b1);
            if (!b2.closed) result.push_back(b2);
            return result;
        }
    }

    // Дизъюнкция
    if (t == NodeType::Disjunction) {
        auto* b = static_cast<BinaryNode*>(sf.formula.get());
        if (s == Sign::F) {
            // alpha: F(A\/B) -> F(A), F(B)
            branch.addFormula({Sign::F, b->left});
            if (!branch.closed) branch.addFormula({Sign::F, b->right});
            if (branch.closed) return {};
            return {branch};
        } else {
            // beta: T(A\/B) -> T(A) | T(B)
            Branch b1 = branch, b2 = branch;
            b1.addFormula({Sign::T, b->left});
            b2.addFormula({Sign::T, b->right});
            std::vector<Branch> result;
            if (!b1.closed) result.push_back(b1);
            if (!b2.closed) result.push_back(b2);
            return result;
        }
    }

    // Импликация
    if (t == NodeType::Implication) {
        auto* b = static_cast<BinaryNode*>(sf.formula.get());
        if (s == Sign::F) {
            // alpha: F(A->B) -> T(A), F(B)
            branch.addFormula({Sign::T, b->left});
            if (!branch.closed) branch.addFormula({Sign::F, b->right});
            if (branch.closed) return {};
            return {branch};
        } else {
            // beta: T(A->B) -> F(A) | T(B)
            Branch b1 = branch, b2 = branch;
            b1.addFormula({Sign::F, b->left});
            b2.addFormula({Sign::T, b->right});
            std::vector<Branch> result;
            if (!b1.closed) result.push_back(b1);
            if (!b2.closed) result.push_back(b2);
            return result;
        }
    }

    // Эквивалентность
    if (t == NodeType::Equivalence) {
        auto* b = static_cast<BinaryNode*>(sf.formula.get());
        auto ab = std::make_shared<BinaryNode>(NodeType::Implication, b->left,  b->right);
        auto ba = std::make_shared<BinaryNode>(NodeType::Implication, b->right, b->left);
        if (s == Sign::T) {
            // alpha: T(A~B) -> T(A->B), T(B->A)
            branch.addFormula({Sign::T, ab});
            if (!branch.closed) branch.addFormula({Sign::T, ba});
            if (branch.closed) return {};
            return {branch};
        } else {
            // beta: F(A~B) -> F(A->B) | F(B->A)
            Branch b1 = branch, b2 = branch;
            b1.addFormula({Sign::F, ab});
            b2.addFormula({Sign::F, ba});
            std::vector<Branch> result;
            if (!b1.closed) result.push_back(b1);
            if (!b2.closed) result.push_back(b2);
            return result;
        }
    }

    return {branch};
}

bool TableauSolver::isClosed(Branch branch) {
    if (branch.closed) return true;

    std::stack<Branch> stack;
    stack.push(branch);

    while (!stack.empty()) {
        Branch cur = stack.top(); stack.pop();
        if (cur.closed) continue;

        int idx = cur.nextUnexpanded();
        if (idx == -1) return false;  // открытая ветвь

        auto children = expand(cur, idx);
        if (children.empty()) continue;
        for (auto& ch : children)
            stack.push(ch);
    }
    return true;
}

Branch TableauSolver::findOpenBranch(Branch branch) {
    if (branch.closed)
        throw std::logic_error("Попытка найти открытую ветвь в закрытой таблице");

    std::stack<Branch> stack;
    stack.push(branch);

    while (!stack.empty()) {
        Branch cur = stack.top(); stack.pop();
        if (cur.closed) continue;

        int idx = cur.nextUnexpanded();
        if (idx == -1) return cur;

        auto children = expand(cur, idx);
        if (children.empty()) continue;
        for (auto& ch : children)
            stack.push(ch);
    }
    throw std::logic_error("Не найдено открытых ветвей");
}

TableauResult TableauSolver::solve(const FormulaPtr& formula) {
    Branch initial;
    initial.addFormula({Sign::F, formula});

    TableauResult result;
    result.isTautology = isClosed(initial);

    if (!result.isTautology) {
        Branch open = findOpenBranch(initial);
        result.counterexample = open.buildCounterexample();
    }

    return result;
}
