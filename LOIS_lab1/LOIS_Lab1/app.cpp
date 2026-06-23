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

#include "app.h"
#include "parser.h"
#include "tableau.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

void App::printHeader() const {
    std::cout << "================================================================\n";
    std::cout << "  ЛОИС -- Проверка тавтологии методом аналитических таблиц\n";
    std::cout << "  Студент: Аникеенко Андрей Дмитриевич, гр. 421701, БГУИР\n";
    std::cout << "================================================================\n";
}

void App::printHelp() const {
    std::cout << "\nСинтаксис формул:\n";
    std::cout << "  Переменные : латинские буквы/цифры (A, B, p, q1 ...)\n";
    std::cout << "  !          : отрицание           пример: !A\n";
    std::cout << "  /\\         : конъюнкция          пример: A/\\B\n";
    std::cout << "  \\/         : дизъюнкция          пример: A\\/B\n";
    std::cout << "  ->         : импликация          пример: A->B\n";
    std::cout << "  ~          : эквивалентность     пример: A~B\n";
    std::cout << "  Скобки     : ( )                 пример: (A/\\B)->C\n";
    std::cout << "\nКоманды: help | exit\n\n";
}

void App::processFormula(const std::string& input) const {
    FormulaPtr formula = parseFormula(input);

    std::string canonical = FormulaPrinter::print(formula);
    std::cout << "\n  Формула    : " << canonical << "\n";

    auto vars = FormulaVariableCollector::collect(formula);
    std::cout << "  Переменные : ";
    bool first = true;
    for (auto& v : vars) {
        if (!first) std::cout << ", ";
        std::cout << v;
        first = false;
    }
    std::cout << "\n";

    TableauSolver solver;
    TableauResult result = solver.solve(formula);

    std::cout << "\n  Результат  : ";
    if (result.isTautology) {
        std::cout << "ТАВТОЛОГИЯ\n";
        std::cout << "  Формула истинна при любых значениях переменных.\n";
        std::cout << "  Все ветви аналитической таблицы закрыты.\n";
    } else {
        std::cout << "НЕ ТАВТОЛОГИЯ\n";
        std::cout << "  Контрпример (формула ложна при):\n";
        for (auto& v : vars) {
            bool val = false;
            auto it = result.counterexample.find(v);
            if (it != result.counterexample.end()) val = it->second;
            std::cout << "    " << v << " = " << (val ? "true" : "false") << "\n";
        }
    }
    std::cout << "\n";
}

void App::run() {
    printHeader();
    printHelp();

    std::string line;
    while (true) {
        std::cout << "Введите формулу: ";
        if (!std::getline(std::cin, line)) break;

        auto ltrim = [](std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                [](unsigned char c){ return !std::isspace(c); }));
        };
        auto rtrim = [](std::string& s) {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                [](unsigned char c){ return !std::isspace(c); }).base(), s.end());
        };
        ltrim(line); rtrim(line);

        if (line.empty()) {
            std::cout << "  (пустой ввод -- введите формулу или 'exit')\n\n";
            continue;
        }
        if (line == "exit" || line == "quit") {
            std::cout << "До свидания!\n";
            break;
        }
        if (line == "help") {
            printHelp();
            continue;
        }

        try {
            processFormula(line);
        } catch (const std::exception& e) {
            std::cout << "  ОШИБКА: " << e.what() << "\n\n";
        }
    }
}
