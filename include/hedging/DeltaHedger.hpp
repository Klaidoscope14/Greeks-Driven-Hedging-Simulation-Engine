#ifndef DELTAHEDGER_HPP
#define DELTAHEDGER_HPP

#include "../pricing/OptionSpecification.hpp"
#include "../pricing/BinomialTree.hpp"
#include "../execution/Order.hpp"
#include <functional>

/*
 * DeltaHedger decides on the underlying trade required to neutralize delta.
 * It holds the current underlying position and returns an Order when it wants to trade.
 *
 * Strategy: threshold-based rebalancing (trade only when |target_delta - current| > threshold)
 */

namespace hedging {

class DeltaHedger {
public:
    DeltaHedger(double threshold = 0.01);

    // compute target underlying qty (signed) given option delta and option contract size
    double target_quantity(double option_delta, double option_notional = 1.0);

    // check and produce an order to move current position toward target
    // returns true if an order is produced and fills 'ord'
    // Note: use ::execution::Order to avoid ambiguity with std::execution
    bool decide(double option_delta, double option_notional, double &current_pos, ::execution::Order &ord);

private:
    double threshold_;
};

} // namespace hedging

#endif // DELTAHEDGER_HPP