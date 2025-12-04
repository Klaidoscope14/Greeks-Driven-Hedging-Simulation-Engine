// tests/test_delta_hedger.cpp
#include <gtest/gtest.h>
#include <cmath>

#include "hedging/ThresholdHedging.hpp"
#include "hedging/DeltaHedger.hpp"
#include "execution/Order.hpp"

using namespace std;
using namespace hedging;

/*
 * Tests ThresholdHedging and DeltaHedger logic.
 */
TEST(DeltaHedgerTest, ThresholdProducesOrderAndDeltaHedgerTarget) {
    ThresholdHedging threshold(0.1);
    double current_pos = 0.0;
    ::execution::Order ord;

    double option_delta = 0.02;
    double option_notional = 100.0;

    bool wants = threshold.decide(option_delta, option_notional, current_pos, ord);
    ASSERT_TRUE(wants);
    ASSERT_EQ(ord.side, ::execution::OrderSide::Sell);
    ASSERT_NEAR(ord.qty, 2.0, 1e-12);

    DeltaHedger dh(0.1);
    double target = dh.target_quantity(option_delta, option_notional);
    ASSERT_NEAR(target, -2.0, 1e-12);
}