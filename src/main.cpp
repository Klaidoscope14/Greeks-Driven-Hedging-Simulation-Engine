#include <iostream>
#include "sim/Simulator.hpp"
#include "pricing/OptionSpecification.hpp"
#include "util/Logger.hpp"

/*
 * Main: constructs a simple simulation config and runs one simulation.
 *
 * This file shows how to hook up components. For production you'd parse JSON
 * configs and wire together many more options.
 */

using namespace std;
using namespace sim;
using namespace pricing;
using namespace util;

int main() {
    log_info("Options Market Maker - Starter Simulation");

    SimConfig cfg;
    cfg.S0 = 100.0;
    cfg.mu = 0.0;
    cfg.sigma = 0.2;
    cfg.r = 0.01;
    cfg.steps = 100; // simulation steps
    cfg.T = 0.5; // not used directly here
    cfg.option_notional = 100.0; // each option contract = 100 shares (if desired)
    cfg.hedger_threshold = 0.5; // hedge when difference > 0.5 shares
    cfg.tx_fixed = 0.0;
    cfg.tx_pct = 0.0005;
    cfg.seed = 12345;

    Simulator simulator(cfg);

    OptionSpecification opt(core::OptionType::Call, 100.0, 0.5);

    simulator.run(opt, "results/simulation_output.csv");

    log_info("Done.");
}