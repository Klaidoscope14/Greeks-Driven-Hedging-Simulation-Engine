#ifndef GREEKS_CALCULATOR_HPP
#define GREEKS_CALCULATOR_HPP

#include <functional>  
#include "pricing/OptionSpecification.hpp"
using namespace std;

/*
 * GreeksCalculator: simple finite-difference wrappers around pricers.
 * This adapter can call any pricer (e.g., binomial or Black-Scholes)
 * by providing a lambda or std::function. For starters, 
 * a Blackboard-style static helper is implemented.
 */

namespace greeks {
    struct Greeks {
        double delta;
        double gamma;
        double theta;
        double vega;
        double rho;
    };

    class GreeksCalculator {
    public:
        static Greeks numeric_all(
            std::function<double(double, double)> pricer_fn,
            double S,
            double r,
            double sigma,
            const pricing::OptionSpecification& opt);
    };
} 

#endif 