#include "pricing/BlackScholes.hpp"
#include <cmath>
#include <limits>
using namespace std;

namespace pricing {
    double BlackScholes::norm_pdf(double x) {
        static const double INV_SQRT_2PI = 0.3989422804014327;
        return INV_SQRT_2PI * exp(-0.5 * x * x);
    }

    /* Approximation of normal CDF using std::erf */
    double BlackScholes::norm_cdf(double x) {
        return 0.5 * (1.0 + erf(x / sqrt(2.0)));
    }

    double BlackScholes::price(double S, double r, double sigma, const OptionSpecification& opt) {
        double K = opt.strike;
        double T = opt.maturity;
        if (T <= 0.0) { // expired
            if (opt.type == core::OptionType::Call) return max(0.0, S - K);
            return max(0.0, K - S);
        }

        double sqrtT = sqrt(T);
        double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrtT);
        double d2 = d1 - sigma * sqrtT;

        if (opt.type == core::OptionType::Call) {
            return S * norm_cdf(d1) - K * exp(-r * T) * norm_cdf(d2);
        } 
        
        else return K * exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
    }

    double BlackScholes::delta(double S, double r, double sigma, const OptionSpecification& opt) {
        double K = opt.strike;
        double T = opt.maturity;

        if (T <= 0.0) {
            if (opt.type == core::OptionType::Call) return S > K ? 1.0 : 0.0;
            return S < K ? -1.0 : 0.0;
        }

        double sqrtT = sqrt(T);
        double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrtT);
        if (opt.type == core::OptionType::Call) {
            return norm_cdf(d1);
        } 
        
        else return norm_cdf(d1) - 1.0;
    }
} 