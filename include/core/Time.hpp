#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>
using namespace std;

/*
 Time is expressed in years for pricing.
*/

namespace core {
    using Clock = chrono::steady_clock;
    using TimePoint = Clock::time_point;

    inline double days_to_years(double days) {
        return days / 365.0;
    }
}

#endif 