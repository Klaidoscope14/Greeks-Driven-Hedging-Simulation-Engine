#ifndef SIMULATIONCONFIG_HPP
#define SIMULATIONCONFIG_HPP

#include <cstdint>
#include <string>
using namespace std;

namespace sim {
    /*
    * SimulationConfig — configuration for a single simulator run.
    *
    * - steps: number of time steps in the simulated path. Larger values produce
    *   longer/more granular paths and larger CSV outputs (e.g. 10_000, 100_000).
    * - T: total time in years (maturity)
    *
    * Note: increase 'steps' to increase volume of input data (more rows).
    */
    struct SimulationConfig {
        double S0 = 100.0;         // initial spot
        double mu = 0.0;          // drift
        double sigma = 0.2;       // volatility
        double r = 0.01;          // risk-free rate
        double T = 1.0;           // time horizon / maturity (years)
        std::size_t steps = 10000; // <<< default increased to 10k (was 100)
        double option_notional = 1.0;
        double hedger_threshold = 0.5;
        double tx_fixed = 0.0;
        double tx_pct = 0.0;
        uint64_t seed = 42;

        SimulationConfig() = default;

        SimulationConfig(double S0_, double mu_, double sigma_, double r_, double T_, std::size_t steps_,
                double notional = 1.0, double hedger_threshold_ = 0.5,
                double tx_fixed_ = 0.0, double tx_pct_ = 0.0, uint64_t seed_ = 42)
            : S0(S0_), mu(mu_), sigma(sigma_), r(r_), T(T_), steps(steps_),
            option_notional(notional), hedger_threshold(hedger_threshold_),
            tx_fixed(tx_fixed_), tx_pct(tx_pct_), seed(seed_) {}
    };
} 

#endif 