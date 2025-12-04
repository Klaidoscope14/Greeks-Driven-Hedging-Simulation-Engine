#ifndef TRANSACTIONCOSTMODEL_HPP
#define TRANSACTIONCOSTMODEL_HPP

/*
 * TransactionCostModel: encapsulates fixed and proportional costs.
 * Keeps cost logic separate in case you want to swap models later.
 *
 * cost = fixed + pct * notional
 */

namespace execution {

class TransactionCostModel {
public:
    TransactionCostModel(double fixed = 0.0, double pct = 0.0)
        : fixed_(fixed), pct_(pct) {}

    double fixed() const { return fixed_; }
    double pct() const { return pct_; }

    /* compute total cost given notional */
    double compute(double notional) const {
        if (notional < 0.0) notional = -notional;
        return fixed_ + pct_ * notional;
    }

private:
    double fixed_;
    double pct_;
};

} // namespace execution

#endif // TRANSACTIONCOSTMODEL_HPP