#include "greeks/GreeksCalculator.hpp"
#include <functional>
#include <cmath>
#include <limits>

using namespace std;

namespace greeks {
    /*
    * Numeric Greeks wrapper.
    *
    * Note:
    * - pricer_fn is a simple lambda taking (S, sigma) and returning a price.
    * - For theta/rho you need a pricer that accepts time-to-maturity and r;
    *   those are left as placeholders here to keep the starter code simple.
    */
    Greeks GreeksCalculator::numeric_all(function<double(double,double)> pricer_fn,
                                        double S, double /*r*/, double sigma,
                                        const pricing::OptionSpecification& opt) {
        (void)opt; 

        Greeks g{};
        
        double dS = max(1e-4, 1e-2 * S);
        double dsig = 1e-4;

        // Price at baseline (pricer takes S and sigma as inputs via lambda)
        double base = pricer_fn(S, sigma);

        // Delta (central)
        double pu = pricer_fn(S + dS, sigma);
        double pd = pricer_fn(S - dS, sigma);
        g.delta = (pu - pd) / (2.0 * dS);

        // Gamma (central)
        g.gamma = (pu - 2.0 * base + pd) / (dS * dS);

        // Theta and Rho are non-trivial with this pricer_fn signature.
        // Set placeholders to 0.0. If you later extend pricer_fn to accept r/T,
        // implement central differences for theta and rho here.
        g.theta = 0.0;

        // Vega (central)
        double pv_up = pricer_fn(S, sigma + dsig);
        double pv_dn = pricer_fn(S, sigma - dsig);
        g.vega = (pv_up - pv_dn) / (2.0 * dsig);

        // Rho placeholder
        g.rho = 0.0;

        return g;
    }
}