#ifndef OPTION_SPECIFICATION_HPP
#define OPTION_SPECIFICATION_HPP

#include "core/Types.hpp"

/*
 * OptionSpecification holds basic option instrument data:
 *  - type (call/put)
 *  - strike price
 *  - maturity (in years)
 */

namespace pricing {

struct OptionSpecification {
    core::OptionType type;
    double strike;
    double maturity; // years

    OptionSpecification(core::OptionType t = core::OptionType::Call,
                        double k = 100.0,
                        double T = 0.5)
        : type(t), strike(k), maturity(T) {}
};

} // namespace pricing

#endif // OPTION_SPECIFICATION_HPP