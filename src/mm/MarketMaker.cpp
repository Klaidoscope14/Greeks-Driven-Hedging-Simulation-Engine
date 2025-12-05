#include "mm/MarketMaker.hpp"
#include <cmath>
using namespace std;

namespace mm {
    MarketMaker::MarketMaker(double spread_pct)
        : spread_pct_(spread_pct), option_inventory_(0) {}

    Quote MarketMaker::quote(double model_price) const {
        double half_spread = model_price * spread_pct_ * 0.5;
        return Quote{model_price - half_spread, model_price + half_spread};
    }

    void MarketMaker::add_option_position(int qty) {
        option_inventory_ += qty;
    }

    int MarketMaker::option_inventory() const {
        return option_inventory_;
    }
} 