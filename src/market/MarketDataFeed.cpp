#include "market/MarketDataFeed.hpp"

using namespace std;

namespace market {

MarketDataFeed::MarketDataFeed(const PricePath& pp) : path_(pp), step_(0) {}

void MarketDataFeed::set_price_path(const PricePath& pp) {
    path_ = pp;
    step_ = 0;
}

void MarketDataFeed::reset() {
    step_ = 0;
}

optional<double> MarketDataFeed::next() {
    if (path_.empty() || step_ >= static_cast<int>(path_.size())) return nullopt;
    double p = path_.at(step_);
    if (callback_) callback_(step_, p);
    ++step_;
    return p;
}

void MarketDataFeed::on_tick(TickCallback cb) {
    callback_ = cb;
}

} // namespace market