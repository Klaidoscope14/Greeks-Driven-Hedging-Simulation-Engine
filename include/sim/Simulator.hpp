#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "../market/UnderlyingSimulator.hpp"
#include "../pricing/OptionSpecification.hpp"
#include "../pricing/BinomialTree.hpp"
#include "../pricing/BlackScholes.hpp"
#include "../greeks/GreeksCalculator.hpp"
#include "../hedging/DeltaHedger.hpp"
#include "../execution/ExecutionEngine.hpp"
#include "../mm/MarketMaker.hpp"
#include "../util/CSVWriter.hpp"
#include <string>
using namespace std;

/*
 * Simulator orchestrates:
 *  - generating underlying path
 *  - at each step: price option, compute Greeks, decide hedges, execute trades
 *  - track inventory and P&L
 *
 * This is a simple, single-option scenario orchestrator for demonstration.
 */

namespace sim {
    struct SimConfig {
        double S0;
        double mu;
        double sigma;
        double r;
        int steps;
        double T; 
        double option_notional; // how many shares per option contract (1 by default)
        double hedger_threshold;
        double tx_fixed;
        double tx_pct;
        uint64_t seed;
    };

    class Simulator {
    public:
        Simulator(const SimConfig& cfg);    
        void run(const pricing::OptionSpecification& opt, const std::string& csv_file);

    private:
        SimConfig cfg_;
    };
} 

#endif 