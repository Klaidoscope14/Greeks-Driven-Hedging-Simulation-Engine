#ifndef THRESHOLDHEDGING_HPP
#define THRESHOLDHEDGING_HPP

#include "HedgingStrategy.hpp"
#include "../execution/Order.hpp"
using namespace std;

/*
 * Threshold-based hedging: only trade when difference between target and current
 * underlying position exceeds threshold. This is the common simple practical policy.
 */

namespace hedging {
    class ThresholdHedging : public HedgingStrategy {
    public:
        explicit ThresholdHedging(double threshold = 0.5);

        bool decide(double option_delta, double option_notional, double &current_pos, ::execution::Order &out_order) override;

    private:
        double threshold_;
    };
} 

#endif 