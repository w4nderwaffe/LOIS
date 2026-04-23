#pragma once

#include <memory>
#include <string>

enum class FormulaType {
    Variable,
    Not,
    And,
    Or,
    Impl,
    Equiv
};

class FormulaNode;
using FormulaPtr = std::shared_ptr<FormulaNode>;

class FormulaNode {
public:
    FormulaNode(FormulaType type, std::string name = "", FormulaPtr left = nullptr, FormulaPtr right = nullptr)
        : type_(type), name_(std::move(name)), left_(std::move(left)), right_(std::move(right)) {}

    FormulaType getType() const {
        return type_;
    }

    const std::string& getName() const {
        return name_;
    }

    FormulaPtr getLeft() const {
        return left_;
    }

    FormulaPtr getRight() const {
        return right_;
    }

    bool isVariable() const {
        return type_ == FormulaType::Variable;
    }

    bool isUnary() const {
        return type_ == FormulaType::Not;
    }

    bool isBinary() const {
        return type_ == FormulaType::And ||
               type_ == FormulaType::Or ||
               type_ == FormulaType::Impl ||
               type_ == FormulaType::Equiv;
    }

private:
    FormulaType type_;
    std::string name_;
    FormulaPtr left_;
    FormulaPtr right_;
};
