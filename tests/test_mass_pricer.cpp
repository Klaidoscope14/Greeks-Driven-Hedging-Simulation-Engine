#include <gtest/gtest.h>
#include "pricing/BlackScholes.hpp"
#include "pricing/OptionSpecification.hpp"
using namespace std;

TEST(MassPricer, ManyCalls) {
    pricing::OptionSpecification opt(core::OptionType::Call, 100.0, 0.5);
    double r = 0.01;
    double sigma = 0.2;
    double S = 100.0;

    for (int i = 0; i < 100000; ++i) {
        double price = pricing::BlackScholes::price(S + (i % 10), r, sigma, opt);
        ASSERT_GT(price, 0.0);
    }
}
