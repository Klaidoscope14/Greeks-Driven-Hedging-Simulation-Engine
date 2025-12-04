#ifndef MARKETDATAFEED_HPP
#define MARKETDATAFEED_HPP

#include "PricePath.hpp"
#include <functional>
#include <optional>

/*
 * MarketDataFeed provides a simple iterator-style interface to consume
 * a PricePath one tick at a time. It also allows registering a "tick callback"
 * which will be invoked when a new price is emitted.
 *
 * This is a tiny abstraction useful for moving from the simulator to a
 * feed-driven architecture later.
 */

namespace market {

class MarketDataFeed {
public:
    using TickCallback = std::function<void(int step, double price)>;

    MarketDataFeed() = default;
    explicit MarketDataFeed(const PricePath& pp);

    void set_price_path(const PricePath& pp);
    void reset();

    /* Return next price if available, otherwise std::nullopt */
    std::optional<double> next();

    /* register callback; overwritten if called multiple times */
    void on_tick(TickCallback cb);

    int step() const { return step_; }
    int total_steps() const { return static_cast<int>(path_.size()); }

private:
    PricePath path_;
    int step_ = 0;
    TickCallback callback_ = nullptr;
};

} // namespace market

#endif // MARKETDATAFEED_HPP