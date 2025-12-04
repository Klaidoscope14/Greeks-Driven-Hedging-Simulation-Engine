#ifndef BINOMIALTREE_HPP
#define BINOMIALTREE_HPP

#include <vector>
#include "pricing/OptionSpecification.hpp"

/*
 * Simple recombining binomial tree pricer for European & American options.
 * - N: number of steps
 * - computes price via backward induction
 * - provides delta and gamma computed from first/second node values
 */

namespace pricing {

class BinomialTree {
public:
    BinomialTree(double S0, double r, double sigma, int steps);

    double price(const OptionSpecification& opt, bool american = false) const;

    // Basic Greeks extracted from the tree at t=0
    double delta(const OptionSpecification& opt, bool american = false) const;
    double gamma(const OptionSpecification& opt, bool american = false) const;

private:
    double S0_;
    double r_;
    double sigma_;
    int N_;
};

} // namespace pricing

#endif // BINOMIALTREE_HPP