#ifndef GREEKS_CALCULATOR_HPP
#define GREEKS_CALCULATOR_HPP

#include <functional>   // <--- required for std::function
#include "pricing/OptionSpecification.hpp"

/*
 * GreeksCalculator: simple finite-difference wrappers around pricers.
 * This adapter can call any pricer (e.g., binomial or Black-Scholes)
 * by providing a lambda or std::function. For the starter, we will
 * implement a Blackboard-style static helper.
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
    // numerical bump sizes are configurable
    // pricer_fn: std::function<double(double S, double sigma)>
    static Greeks numeric_all(
        std::function<double(double, double)> pricer_fn,
        double S,
        double r,
        double sigma,
        const pricing::OptionSpecification& opt);
};

} // namespace greeks

#endif // GREEKS_CALCULATOR_HPP