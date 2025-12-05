#ifndef SIMULATIONCONFIG_HPP
#define SIMULATIONCONFIG_HPP

/*
 * SimulationConfig: a small wrapper to load or represent simulation parameters.
 * Keeps a single struct shared between simulator and any config loader code.
 */

namespace sim {
    struct SimulationConfig {
        double S0 = 100.0;
        double mu = 0.0;
        double sigma = 0.2;
        double r = 0.01;
        int steps = 100;
        double T = 0.5;
        double option_notional = 100.0;
        double hedger_threshold = 0.5;
        double tx_fixed = 0.0;
        double tx_pct = 0.0005;
        unsigned long seed = 12345UL;
    };
} 

#endif