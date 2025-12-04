#ifndef EXECUTIONENGINE_HPP
#define EXECUTIONENGINE_HPP

#include "execution/Order.hpp"
#include <functional>

/*
 * ExecutionEngine simulates immediate execution in the underlying at mid price
 * with configurable transaction cost (proportional) and fixed cost.
 *
 * For starters, we implement a simple market-taker execution: orders are filled
 * at the provided market price with cost applied; advanced LOB simulation can be added later.
 */

namespace execution {

class ExecutionEngine {
public:
    ExecutionEngine(double fixed_cost = 0.0, double pct_cost = 0.0);

    // execute an order at market_price, returns realized cashflow (negative = cash paid)
    double execute(const Order& ord, double market_price);

private:
    double fixed_cost_;
    double pct_cost_;
};

} // namespace execution

#endif // EXECUTIONENGINE_HPP