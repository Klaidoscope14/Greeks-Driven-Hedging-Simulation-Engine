#include "hedging/ThresholdHedging.hpp"
#include <cmath>

/*
 * ThresholdHedging.cpp
 *
 * Avoid using `using namespace execution;` which collides with std::execution.
 * Use fully-qualified ::execution::Order and ::execution::OrderSide explicitly.
 */

using namespace std;

namespace hedging {

ThresholdHedging::ThresholdHedging(double threshold) : threshold_(threshold) {}

bool ThresholdHedging::decide(double option_delta, double option_notional, double &current_pos, ::execution::Order &out_order) {
    double target = -option_delta * option_notional;
    double diff = target - current_pos;
    if (fabs(diff) < threshold_) {
        return false;
    }
    if (diff > 0) out_order = ::execution::Order(::execution::OrderSide::Buy, fabs(diff), 0.0);
    else out_order = ::execution::Order(::execution::OrderSide::Sell, fabs(diff), 0.0);
    return true;
}

} // namespace hedging