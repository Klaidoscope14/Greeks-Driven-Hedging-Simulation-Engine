#include "execution/ExecutionEngine.hpp"
#include <cmath>

using namespace std;

namespace execution {

ExecutionEngine::ExecutionEngine(double fixed_cost, double pct_cost)
    : fixed_cost_(fixed_cost), pct_cost_(pct_cost) {}

double ExecutionEngine::execute(const Order& ord, double market_price) {
    double signed_qty = (ord.side == OrderSide::Buy) ? ord.qty : -ord.qty;
    double notional = fabs(signed_qty * market_price);
    double tx_cost = fixed_cost_ + pct_cost_ * notional;
    // cashflow: negative when buying, positive when selling (from perspective of cash)
    double cashflow = -signed_qty * market_price - tx_cost;
    return cashflow;
}

} // namespace execution