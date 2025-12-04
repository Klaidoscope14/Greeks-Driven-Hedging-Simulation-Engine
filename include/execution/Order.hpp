#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

/*
 * Simple order struct representing a trade in the underlying.
 */

namespace execution {

enum class OrderSide { Buy, Sell };

struct Order {
    OrderSide side;
    double qty;    // positive quantity
    double price;  // limit price or executed price

    Order(OrderSide s = OrderSide::Buy, double q = 0.0, double p = 0.0)
        : side(s), qty(q), price(p) {}
};

} // namespace execution

#endif // ORDER_HPP