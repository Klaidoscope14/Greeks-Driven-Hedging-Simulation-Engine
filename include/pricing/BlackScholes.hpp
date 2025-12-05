#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP

#include <cmath>
#include "pricing/OptionSpecification.hpp"

/*
 * Black-Scholes analytic formulas for European options.
 * Used as validation against the binomial pricer.
 */

namespace pricing {
    class BlackScholes {
    public:
        // returns price of European option
        static double price(double S, double r, double sigma, const OptionSpecification& opt);

        // returns analytic delta for European option (BS)
        static double delta(double S, double r, double sigma, const OptionSpecification& opt);

    private:
        static double norm_pdf(double x);
        static double norm_cdf(double x);
    };
} 

#endif 