#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
using namespace std;

/*
 * Simple order struct representing a trade in the underlying.
 */

namespace execution {
    enum class OrderSide { 
        Buy, 
        Sell 
    };

    struct Order {
        OrderSide side;
        double qty;    
        double price; 

        Order(OrderSide s = OrderSide::Buy, double q = 0.0, double p = 0.0)
            : side(s), qty(q), price(p) {}
    };
} 

#endif