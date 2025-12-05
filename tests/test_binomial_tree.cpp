#include <gtest/gtest.h>
#include <cmath>

#include "pricing/BinomialTree.hpp"
#include "pricing/BlackScholes.hpp"
#include "pricing/OptionSpecification.hpp"
#include "core/MathUtils.hpp"

using namespace std;
using namespace pricing;
using namespace core;

/*
 * Check convergence of BinomialTree to Black-Scholes for a European Call
 * with a modest tolerance (this is a smoke/convergence test, not a tight numerical check).
 */
TEST(BinomialTreeTest, ConvergesToBlackScholes) {
    double S = 100.0;
    double r = 0.01;
    double sigma = 0.2;
    OptionSpecification opt(core::OptionType::Call, 100.0, 0.5);

    double bs_price = BlackScholes::price(S, r, sigma, opt);

    // test for a few N values and ensure the error is reasonable
    for (int N : {20, 50, 100}) {
        BinomialTree bt(S, r, sigma, N);
        double bt_price = bt.price(opt, false);
        double diff = fabs(bt_price - bs_price);
        // allow a loose tolerance; the goal is to ensure code path runs and is roughly correct
        ASSERT_LT(diff, 2.0) << "Binomial price diverged too much from Black-Scholes for N=" << N;
    }
}