#ifndef MM_QUOTE_GENERATOR_HPP
#define MM_QUOTE_GENERATOR_HPP

#include <utility> 
#include <cmath>  
#include "pricing/OptionSpecification.hpp"
#include "pricing/BlackScholes.hpp"

namespace mm {
    /*
    * QuoteGenerator:
    * Produces bid/ask option quotes for the market maker.
    * 
    * Typical model:
    *   mid = theoretical_price
    *   bid = mid - spread/2
    *   ask = mid + spread/2
    * 
    * spread may be dynamic depending on sigma, inventory, etc.
    */
    class QuoteGenerator {
    public:
        QuoteGenerator(double base_spread = 0.5)
            : base_spread_(base_spread) {}

        // Generate bid/ask quotes for an option given spot S, r, sigma, and spec.
        std::pair<double, double> generate(
            double S,
            double r,
            double sigma,
            const pricing::OptionSpecification& opt
        ) const {
            const double mid = pricing::BlackScholes::price(S, r, sigma, opt);
            const double half = base_spread_ * 0.5;
            return { 
                mid - half, 
                mid + half 
            };
        }

    private:
        double base_spread_;
    };
}

#endif 