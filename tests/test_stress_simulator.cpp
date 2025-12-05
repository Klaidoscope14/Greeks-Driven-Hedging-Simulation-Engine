#include <gtest/gtest.h>
#include <filesystem>
#include "sim/SimulationConfig.hpp"
#include "sim/Simulator.hpp"
#include "pricing/OptionSpecification.hpp"

using namespace std;
namespace fs = filesystem;

TEST(StressSimulator, LongPathSmoke) {
    // This test is intended to be enabled only for manual runs; it is heavy.
    sim::SimConfig cfg;
    cfg.S0 = 100.0;
    cfg.mu = 0.0;
    cfg.sigma = 0.2;
    cfg.r = 0.01;
    cfg.T = 1.0;
    cfg.steps = 50000; // 50k steps for stress smoke-test
    cfg.option_notional = 1.0;
    cfg.hedger_threshold = 0.5;
    cfg.tx_fixed = 0.0;
    cfg.tx_pct = 0.0;
    cfg.seed = 42;

    sim::Simulator simulator(cfg);
    fs::path out = fs::path("results") / "test_stress_out.csv";
    if (!fs::exists("results")) fs::create_directory("results");
    if (fs::exists(out)) fs::remove(out);

    pricing::OptionSpecification opt(core::OptionType::Call, 100.0, cfg.T);
    simulator.run(opt, out.string());

    ASSERT_TRUE(fs::exists(out));
    auto sz = fs::file_size(out);
    ASSERT_GT(sz, 0u);

    fs::remove(out);
}