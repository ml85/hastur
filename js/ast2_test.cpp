// SPDX-FileCopyrightText: 2022 Robin Lindén <dev@robinlinden.eu>
//
// SPDX-License-Identifier: BSD-2-Clause

#include "js/ast2.h"

#include "etest/etest.h"

using namespace js::ast2;
using etest::expect_eq;

int main() {
    etest::test("literals", [] {
        AstExecutor e;
        expect_eq(e.execute(NumericLiteral{5.}), Value{5.});
        expect_eq(e.execute(StringLiteral{"hello"}), Value{"hello"});
    });

    etest::test("binary expression, plus", [] {
        auto plus_expr = BinaryExpression{BinaryOperator::Plus,
                std::make_unique<Expression>(NumericLiteral{11.}),
                std::make_unique<Expression>(NumericLiteral{31.})};
        AstExecutor e;
        expect_eq(e.execute(plus_expr), Value{42.});
    });

    etest::test("binary expression, minus", [] {
        auto minus_expr = BinaryExpression{BinaryOperator::Minus,
                std::make_unique<Expression>(NumericLiteral{11.}),
                std::make_unique<Expression>(NumericLiteral{31.})};
        AstExecutor e;
        expect_eq(e.execute(minus_expr), Value{-20.});
    });

    return etest::run_all_tests();
}
