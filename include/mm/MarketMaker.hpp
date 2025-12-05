#ifndef MARKETMAKER_HPP
#define MARKETMAKER_HPP

#include "pricing/OptionSpecification.hpp"
#include <optional>
using namespace std;

/*
 * Simple MarketMaker: quotes mid = model_price and provides bid/ask by applying spread.
 * Keeps inventory of option contracts and underlying hedges.
 */

namespace mm {
    struct Quote {
        double bid;
        double ask;
    };

    class MarketMaker {
    public:
        MarketMaker(double spread_pct = 0.01);
        Quote quote(double model_price) const;

        void add_option_position(int qty);
        int option_inventory() const;

    private:
        double spread_pct_;
        int option_inventory_;
    };
} 

#endif 