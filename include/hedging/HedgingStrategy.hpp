#ifndef HEDGINGSTRATEGY_HPP
#define HEDGINGSTRATEGY_HPP

#include "../execution/Order.hpp"
using namespace std;

/*
 * Abstract base for hedging strategies.
 *
 * A concrete HedgingStrategy decides whether to place an order at a given tick.
 * It does not execute trades itself; it just returns an Order when required.
 */

namespace hedging {
    class HedgingStrategy {
    public:
        virtual ~HedgingStrategy() = default;

        /*
        * Decide whether to hedge and produce an order (true) or not (false).
        * - option_delta: current delta of the option (per contract)
        * - option_notional: number of shares per option contract
        * - current_pos: current underlying position (signed)
        * - out_order: filled with the intended order if returning true
        */
        virtual bool decide(double option_delta,
                            double option_notional,
                            double &current_pos,
                            ::execution::Order &out_order) = 0;
    };
} 

#endif