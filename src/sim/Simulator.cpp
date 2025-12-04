#include "sim/Simulator.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>
#include "util/Logger.hpp"

using namespace std;
using namespace util;
using namespace pricing;
using namespace hedging;
using namespace mm;
using namespace market;
using namespace greeks;

namespace sim {

/*
 * Simulator::run
 *
 * Orchestrates a simple tick loop:
 *  - generate path
 *  - price option each tick
 *  - compute delta (using BlackScholes for smoothness)
 *  - decide hedging order via hedger
 *  - execute order via ExecutionEngine
 *  - update underlying position and cash
 *  - record row to CSV
 *
 * Notes:
 *  - All types from the project's `execution` namespace are explicitly
 *    qualified as ::execution::... to avoid ambiguity with std::execution.
 */

Simulator::Simulator(const SimConfig& cfg) : cfg_(cfg) {}

void Simulator::run(const OptionSpecification& opt, const string& csv_file) {
    log_info("Starting simulation...");

    // create components
    UnderlyingSimulator usim(cfg_.S0, cfg_.mu, cfg_.sigma, cfg_.seed);
    auto path = usim.simulate_path(cfg_.steps, opt.maturity / cfg_.steps);
    BinomialTree bt(cfg_.S0, cfg_.r, cfg_.sigma, 100); // fixed steps for tree pricing
    DeltaHedger hedger(cfg_.hedger_threshold);

    // Use fully-qualified execution types to avoid name collisions with std::execution
    ::execution::ExecutionEngine exec(cfg_.tx_fixed, cfg_.tx_pct);
    MarketMaker mmaker(0.01);
    CSVWriter csv(csv_file);
    csv.open(csv_file);
    csv.write_header({"time_step", "spot", "option_price", "delta", "underlying_pos", "cash", "pnl"});

    double underlying_pos = 0.0;
    double cash = 0.0;
    double pnl = 0.0;

    double option_notional = cfg_.option_notional;

    for (int t = 0; t <= cfg_.steps; ++t) {
        double spot = path[t];
        // build dynamic pricer centered at current spot
        BinomialTree dynamic_bt(spot, cfg_.r, cfg_.sigma, 200);
        double model_price = dynamic_bt.price(opt, false);
        double delta = BlackScholes::delta(spot, cfg_.r, cfg_.sigma, opt);

        // Hedging decision
        ::execution::Order ord; // fully-qualified
        bool wants_trade = hedger.decide(delta, option_notional, underlying_pos, ord);
        if (wants_trade) {
            // Execute at mid price (simple model)
            double exec_price = spot;
            double cashflow = exec.execute(ord, exec_price);
            double signed_qty = (ord.side == ::execution::OrderSide::Buy) ? ord.qty : -ord.qty;
            underlying_pos += signed_qty;
            cash += cashflow;
            ostringstream ss;
            ss << "Executed order at step " << t << " side=" 
               << ((ord.side == ::execution::OrderSide::Buy) ? "Buy" : "Sell")
               << " qty=" << ord.qty << " price=" << exec_price;
            log_info(ss.str());
        }

        // mark-to-market P&L: underlying position + cash - option liability (assume 1 short option)
        double option_mt = model_price;
        pnl = underlying_pos * spot + cash - option_mt;

        // write a vector<string> explicitly (CSVWriter::write_row expects vector<T>)
        vector<string> row{
            to_string(t),
            to_string(spot),
            to_string(option_mt),
            to_string(delta),
            to_string(underlying_pos),
            to_string(cash),
            to_string(pnl)
        };
        csv.write_row(row);
    }

    log_info("Simulation finished. Results written to " + csv_file);
}

} // namespace sim