#include "RuleEngine.h"

#include <stdexcept>
#include <vector>

RuleApplicationResult RuleEngine::apply(const SignedFormula& signedFormula) const {
    FormulaPtr formula = signedFormula.getFormula();
    TruthSign sign = signedFormula.getSign();

    switch (formula->getType()) {
        case FormulaType::Variable:
            throw std::logic_error("Cannot apply tableau rule to literal");

        case FormulaType::Not: {
            TruthSign newSign = sign == TruthSign::True ? TruthSign::False : TruthSign::True;
            return RuleApplicationResult::makeAlpha({
                SignedFormula(newSign, formula->getLeft())
            });
        }

        case FormulaType::And: {
            if (sign == TruthSign::True) {
                return RuleApplicationResult::makeAlpha({
                    SignedFormula(TruthSign::True, formula->getLeft()),
                    SignedFormula(TruthSign::True, formula->getRight())
                });
            }

            return RuleApplicationResult::makeBeta(
                { SignedFormula(TruthSign::False, formula->getLeft()) },
                { SignedFormula(TruthSign::False, formula->getRight()) }
            );
        }

        case FormulaType::Or: {
            if (sign == TruthSign::False) {
                return RuleApplicationResult::makeAlpha({
                    SignedFormula(TruthSign::False, formula->getLeft()),
                    SignedFormula(TruthSign::False, formula->getRight())
                });
            }

            return RuleApplicationResult::makeBeta(
                { SignedFormula(TruthSign::True, formula->getLeft()) },
                { SignedFormula(TruthSign::True, formula->getRight()) }
            );
        }

        case FormulaType::Impl: {
            if (sign == TruthSign::False) {
                return RuleApplicationResult::makeAlpha({
                    SignedFormula(TruthSign::True, formula->getLeft()),
                    SignedFormula(TruthSign::False, formula->getRight())
                });
            }

            return RuleApplicationResult::makeBeta(
                { SignedFormula(TruthSign::False, formula->getLeft()) },
                { SignedFormula(TruthSign::True, formula->getRight()) }
            );
        }

        case FormulaType::Equiv: {
            if (sign == TruthSign::True) {
                return RuleApplicationResult::makeBeta(
                    {
                        SignedFormula(TruthSign::True, formula->getLeft()),
                        SignedFormula(TruthSign::True, formula->getRight())
                    },
                    {
                        SignedFormula(TruthSign::False, formula->getLeft()),
                        SignedFormula(TruthSign::False, formula->getRight())
                    }
                );
            }

            return RuleApplicationResult::makeBeta(
                {
                    SignedFormula(TruthSign::True, formula->getLeft()),
                    SignedFormula(TruthSign::False, formula->getRight())
                },
                {
                    SignedFormula(TruthSign::False, formula->getLeft()),
                    SignedFormula(TruthSign::True, formula->getRight())
                }
            );
        }
    }

    throw std::logic_error("Unknown formula type");
}
