#include <gtest/gtest.h>
#include <filesystem>

#include "sim/SimulationConfig.hpp"
#include "sim/Simulator.hpp"
#include "pricing/OptionSpecification.hpp"

using namespace std;
namespace fs = std::filesystem;

/*
 * Smoke test for Simulator::run producing an output CSV.
 * No main() here; gtest_main handles test runner.
 */
TEST(SimulatorTest, RunProducesCSV) {
    sim::SimConfig cfg;
    cfg.S0 = 100.0;
    cfg.mu = 0.0;
    cfg.sigma = 0.15;
    cfg.r = 0.01;
    cfg.steps = 10;
    cfg.T = 0.1;
    cfg.option_notional = 1.0;
    cfg.hedger_threshold = 0.5;
    cfg.tx_fixed = 0.0;
    cfg.tx_pct = 0.0;
    cfg.seed = 42;

    sim::Simulator simulator(cfg);

    fs::path results_dir = fs::path("results");
    if (!fs::exists(results_dir)) fs::create_directory(results_dir);

    fs::path out = results_dir / "test_sim_out.csv";
    if (fs::exists(out)) fs::remove(out);

    pricing::OptionSpecification opt(core::OptionType::Call, 100.0, cfg.T);
    simulator.run(opt, out.string());

    ASSERT_TRUE(fs::exists(out));
    auto fsize = fs::file_size(out);
    ASSERT_GT(fsize, 10u);
    fs::remove(out);
}