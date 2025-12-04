#ifndef QUOTEGENERATOR_HPP
#define QUOTEGENERATOR_HPP

/*
 * QuoteGenerator: create bid/ask around a mid model price.
 * This is separated from MarketMaker to keep quoting logic modular.
 */

namespace mm {

struct SimpleQuote {
    double bid;
    double ask;
};

class QuoteGenerator {
public:
    explicit QuoteGenerator(double spread_pct = 0.01) : spread_pct_(spread_pct) {}

    SimpleQuote make_quote(double model_price) const {
        double half = model_price * spread_pct_ * 0.5;
        return SimpleQuote{model_price - half, model_price + half};
    }

    double spread_pct() const { return spread_pct_; }
    void set_spread_pct(double p) { spread_pct_ = p; }

private:
    double spread_pct_;
};

} // namespace mm

#endif // QUOTEGENERATOR_HPP