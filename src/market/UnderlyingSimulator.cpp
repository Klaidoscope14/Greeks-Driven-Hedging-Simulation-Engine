#include "market/UnderlyingSimulator.hpp"
#include <cmath>

using namespace std;
using namespace util;

namespace market {

UnderlyingSimulator::UnderlyingSimulator(double S0, double mu, double sigma, uint64_t seed)
    : S0_(S0), mu_(mu), sigma_(sigma), rng_(seed) {}

vector<double> UnderlyingSimulator::simulate_path(int N, double dt) {
    vector<double> path;
    path.reserve(N + 1);
    path.push_back(S0_);

    double S = S0_;
    for (int i = 0; i < N; ++i) {
        double z = rng_.normal(0.0, 1.0);
        // Euler-Maruyama discretization for GBM in log space
        double dlogS = (mu_ - 0.5 * sigma_ * sigma_) * dt + sigma_ * sqrt(dt) * z;
        S = S * exp(dlogS);
        path.push_back(S);
    }

    return path;
}

void UnderlyingSimulator::reseed(uint64_t seed) {
    rng_.reseed(seed);
}

} 