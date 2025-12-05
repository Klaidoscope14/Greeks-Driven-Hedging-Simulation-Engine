#ifndef EXECUTIONENGINE_HPP
#define EXECUTIONENGINE_HPP

#include "execution/Order.hpp"
#include <functional>
using namespace std;

/*
 * ExecutionEngine simulates immediate execution in the underlying at mid price
 * with configurable transaction cost (proportional) and fixed cost.
 *
 * For starters, simple market-taker execution is implemented: orders are filled
 * at the provided market price with cost applied.
 */

namespace execution {

class ExecutionEngine {
public:
    ExecutionEngine(double fixed_cost = 0.0, double pct_cost = 0.0);

    // executes an order at market_price, returns realized cashflow (negative = cash paid)
    double execute(const Order& ord, double market_price);

private:
    double fixed_cost_;
    double pct_cost_;
};

} 

#endif 