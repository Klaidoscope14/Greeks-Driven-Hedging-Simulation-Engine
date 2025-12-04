// tests/test_hedging_strategy.cpp
#include <gtest/gtest.h>

#include "hedging/ThresholdHedging.hpp"
#include "execution/Order.hpp"

using namespace hedging;

/*
 * Ensure ThresholdHedging behaves as expected (no main here).
 */
TEST(HedgingStrategyTest, ThresholdNoTradeWhenBelowThreshold) {
    ThresholdHedging strat(1.0);
    double current_pos = 0.0;
    ::execution::Order ord;
    bool wants = strat.decide(0.01, 10.0, current_pos, ord);
    ASSERT_FALSE(wants);
}

TEST(HedgingStrategyTest, ThresholdTradeWhenAboveThreshold) {
    ThresholdHedging strat(0.1);
    double current_pos = 0.0;
    ::execution::Order ord;
    bool wants = strat.decide(0.02, 100.0, current_pos, ord);
    ASSERT_TRUE(wants);
    ASSERT_EQ(ord.side, ::execution::OrderSide::Sell);
    ASSERT_NEAR(ord.qty, 2.0, 1e-12);
}