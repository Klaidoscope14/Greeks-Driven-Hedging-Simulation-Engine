// src/hedging/DeltaHedger.cpp
#include "hedging/DeltaHedger.hpp"
#include <cmath>
#include <sstream>
#include "util/Logger.hpp"

/*
 * DeltaHedger implementation.
 *
 * We avoid `using namespace execution;` because `execution` collides with
 * `std::execution` on some systems. Instead we refer to ::execution::Order
 * and ::execution::OrderSide explicitly to remove ambiguity.
 */

using namespace std;
using namespace util;

namespace hedging {

DeltaHedger::DeltaHedger(double threshold) : threshold_(threshold) {}

/*
 * Compute target underlying quantity to neutralize delta.
 * If you are short one option and option_delta = 0.5 with notional 100,
 * target_quantity = -0.5 * 100 = -50 (i.e. short 50 shares).
 */
double DeltaHedger::target_quantity(double option_delta, double option_notional) {
    return -option_delta * option_notional;
}

/*
 * Decide whether to trade. If the absolute difference between target and
 * current_pos exceeds threshold_, produce an Order (filled into 'ord')
 * and return true. Caller is responsible for executing the order and
 * updating current_pos/cash.
 */
bool DeltaHedger::decide(double option_delta, double option_notional, double &current_pos, ::execution::Order &ord) {
    double target = target_quantity(option_delta, option_notional);
    double diff = target - current_pos;
    if (fabs(diff) < threshold_) return false; // no trade required

    // create a market order for the required difference
    if (diff > 0) {
        ord = ::execution::Order(::execution::OrderSide::Buy, fabs(diff), 0.0);
    } else {
        ord = ::execution::Order(::execution::OrderSide::Sell, fabs(diff), 0.0);
    }
    return true;
}

} // namespace hedging