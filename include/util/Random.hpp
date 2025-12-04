#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

/*
 * RNG wrapper for reproducible simulations.
 * Uses std::mt19937_64 seeded with a user-specified seed.
 */

namespace util {

class Random {
public:
    explicit Random(uint64_t seed = 42ULL) : rng(seed) {}

    double normal(double mean = 0.0, double stddev = 1.0) {
        std::normal_distribution<double> dist(mean, stddev);
        return dist(rng);
    }

    double uniform(double a = 0.0, double b = 1.0) {
        std::uniform_real_distribution<double> dist(a, b);
        return dist(rng);
    }

    uint64_t seed() const { return seed_val; }

    void reseed(uint64_t s) { seed_val = s; rng.seed(s); }

private:
    uint64_t seed_val = 42ULL;
    std::mt19937_64 rng;
};

} // namespace util

#endif // RANDOM_HPP