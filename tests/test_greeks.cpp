// tests/test_greeks.cpp
#include <gtest/gtest.h>

#include "greeks/GreeksCalculator.hpp"
#include "pricing/BlackScholes.hpp"
#include "pricing/OptionSpecification.hpp"

using namespace std;
using namespace greeks;
using namespace pricing;

/*
 * Test numerical greeks using BlackScholes as the pricer lambda.
 */
TEST(GreeksTest, NumericAllWithBlackScholes) {
    double S = 100.0;
    double r = 0.01;
    double sigma = 0.2;
    OptionSpecification opt(core::OptionType::Call, 100.0, 0.5);

    auto pricer = [&](double s, double sig)->double {
        return BlackScholes::price(s, r, sig, opt);
    };

    Greeks g = GreeksCalculator::numeric_all(pricer, S, r, sigma, opt);

    ASSERT_GE(g.delta, 0.0);
    ASSERT_LE(g.delta, 1.0);
    ASSERT_GE(g.gamma, 0.0);
}