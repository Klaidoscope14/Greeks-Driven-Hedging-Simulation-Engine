#include <iostream>
#include <string>
#include "sim/SimulationConfig.hpp"
#include "sim/Simulator.hpp"
#include "pricing/OptionSpecification.hpp"
#include <chrono>
using namespace std;

int main(int argc, char** argv) {
    // Usage: ./stress_run [steps] [out.csv]
    size_t steps = 100000; // default to 100k steps for heavy volume
    string out = "results/stress_simulation.csv";

    if (argc > 1) steps = static_cast<size_t>(stoull(argv[1]));
    if (argc > 2) out = argv[2];

    sim::SimConfig cfg;
    cfg.S0 = 100.0;
    cfg.mu = 0.0;
    cfg.sigma = 0.2;
    cfg.r = 0.01;
    cfg.T = 1.0;
    cfg.steps = steps;
    cfg.option_notional = 1.0;
    cfg.hedger_threshold = 0.5;
    cfg.tx_fixed = 0.0;
    cfg.tx_pct = 0.0;
    cfg.seed = static_cast<uint64_t>(chrono::system_clock::now().time_since_epoch().count() & 0xffffffff);

    sim::Simulator sim(cfg);
    pricing::OptionSpecification opt(core::OptionType::Call, 100.0, cfg.T);

    cout << "[stress_run] Running simulation with steps=" << steps << " -> " << out << "\n";
    sim.run(opt, out);
    cout << "[stress_run] Done.\n";
}