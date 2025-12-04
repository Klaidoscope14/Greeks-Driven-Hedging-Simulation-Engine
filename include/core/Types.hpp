#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

using namespace std;

namespace core {
    using Real = double;

    enum class OptionType {
        Call,
        Put
    };
} 

#endif 