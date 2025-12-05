#include <gtest/gtest.h>
#include <cmath>

#include "execution/ExecutionEngine.hpp"
#include "execution/Order.hpp"

using namespace std;

/*
 * Tests ExecutionEngine: buy and sell produce expected cashflows including transaction cost.
 * No custom main() — gtest_main provides the runner.
 */
TEST(ExecutionEngineTest, BuySellCashflowAndCosts) {
    ::execution::ExecutionEngine engine(0.5, 0.001);

    ::execution::Order buy(::execution::OrderSide::Buy, 10.0, 0.0);
    double market_price = 50.0;
    double cashflow_buy = engine.execute(buy, market_price);

    double expected_cost = 0.5 + 0.001 * (10.0 * market_price);
    double expected_cashflow = - (10.0 * market_price) - expected_cost;
    ASSERT_NEAR(cashflow_buy, expected_cashflow, 1e-9);

    ::execution::Order sell(::execution::OrderSide::Sell, 5.0, 0.0);
    double cashflow_sell = engine.execute(sell, market_price);
    double expected_sell_cost = 0.5 + 0.001 * (5.0 * market_price);
    double expected_cashflow_sell = -(-5.0 * market_price) - expected_sell_cost;
    ASSERT_NEAR(cashflow_sell, expected_cashflow_sell, 1e-9);
}