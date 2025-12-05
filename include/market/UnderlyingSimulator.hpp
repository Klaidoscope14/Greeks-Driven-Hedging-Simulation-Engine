#ifndef UNDERLYINGSIMULATOR_HPP
#define UNDERLYINGSIMULATOR_HPP

#include "util/Random.hpp"
#include <vector>
using namespace std;

/*
 * Simple GBM underlying simulator.
 * Produces a vector of spot prices for given timesteps.
 */

namespace market {
    class UnderlyingSimulator {
    public:
        UnderlyingSimulator(double S0, double mu, double sigma, uint64_t seed = 42);
        std::vector<double> simulate_path(int N, double dt);

        void reseed(uint64_t seed);

    private:
        double S0_;
        double mu_;
        double sigma_;
        util::Random rng_;
    };
} 

#endif 