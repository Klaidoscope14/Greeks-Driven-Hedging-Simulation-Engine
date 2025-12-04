#include "pricing/BinomialTree.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

namespace pricing {

BinomialTree::BinomialTree(double S0, double r, double sigma, int steps)
    : S0_(S0), r_(r), sigma_(sigma), N_(steps) {}

/*
 * Build recombining tree values at maturity and perform backward induction.
 * For American options, allow early exercise at each node.
 */
double BinomialTree::price(const OptionSpecification& opt, bool american) const {
    int N = N_;
    double dt = opt.maturity / static_cast<double>(N);
    double u = exp(sigma_ * sqrt(dt));
    double d = 1.0 / u;
    double disc = exp(-r_ * dt);
    double p = (exp(r_ * dt) - d) / (u - d);

    // Option values at maturity
    vector<double> vals(N + 1);
    for (int i = 0; i <= N; ++i) {
        double ST = S0_ * pow(u, N - i) * pow(d, i);
        if (opt.type == core::OptionType::Call) vals[i] = max(0.0, ST - opt.strike);
        else vals[i] = max(0.0, opt.strike - ST);
    }

    // Backward induction
    for (int step = N - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            double cont = disc * (p * vals[i] + (1.0 - p) * vals[i + 1]);
            if (american) {
                double ST = S0_ * pow(u, step - i) * pow(d, i);
                double exercise = 0.0;
                if (opt.type == core::OptionType::Call) exercise = max(0.0, ST - opt.strike);
                else exercise = max(0.0, opt.strike - ST);
                vals[i] = max(cont, exercise);
            } else {
                vals[i] = cont;
            }
        }
    }
    return vals[0];
}

/*
 * Delta approximation from first upward and downward nodes:
 * Delta = (V_up - V_down) / (S*u - S*d)
 *
 * Note: avoid creating unused locals to satisfy -Werror.
 */
double BinomialTree::delta(const OptionSpecification& opt, bool american) const {
    if (N_ < 1) return 0.0;
    int N = N_;
    double dt = opt.maturity / static_cast<double>(N);
    double u = exp(sigma_ * sqrt(dt));
    double d = 1.0 / u;

    // compute option values at the immediate up and down nodes by constructing
    // small subtrees rooted at S0*u and S0*d with N-1 remaining steps.
    BinomialTree subtree_up(S0_ * u, r_, sigma_, N - 1);
    BinomialTree subtree_down(S0_ * d, r_, sigma_, N - 1);
    double val_up = subtree_up.price(opt, american);
    double val_down = subtree_down.price(opt, american);

    double denom = S0_ * (u - d);
    if (denom == 0.0) return 0.0;
    return (val_up - val_down) / denom;
}

/*
 * Gamma via finite difference on delta: bump S0 slightly and recompute delta.
 */
double BinomialTree::gamma(const OptionSpecification& opt, bool american) const {
    // finite difference: bump S0 by small eps percent
    double eps = 1e-4;
    double S_p = S0_ * (1.0 + eps);
    double S_m = S0_ * (1.0 - eps);
    BinomialTree bt_p(S_p, r_, sigma_, N_);
    BinomialTree bt_m(S_m, r_, sigma_, N_);
    double delta_p = bt_p.delta(opt, american);
    double delta_m = bt_m.delta(opt, american);
    double dS = S_p - S_m;
    if (dS == 0.0) return 0.0;
    return (delta_p - delta_m) / dS;
}

} // namespace pricing