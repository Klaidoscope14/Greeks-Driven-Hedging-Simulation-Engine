#ifndef UNDERLYINGSIMULATOR_HPP
#define UNDERLYINGSIMULATOR_HPP

#include "util/Random.hpp"
#include <vector>

/*
 * Simple GBM underlying simulator.
 * Produces a vector of spot prices for given timesteps.
 */

namespace market {

class UnderlyingSimulator {
public:
    UnderlyingSimulator(double S0, double mu, double sigma, uint64_t seed = 42);

    // simulate N steps of dt (in years), returns vector of size N+1 including S0
    std::vector<double> simulate_path(int N, double dt);

    void reseed(uint64_t seed);

private:
    double S0_;
    double mu_;
    double sigma_;
    util::Random rng_;
};

} // namespace market

#endif // UNDERLYINGSIMULATOR_HPP