#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <cmath>

/*
 * Tiny math utilities used across the project.
 * Keep the functions simple and header-only for convenience.
 */

namespace core {
    inline double clamp(double x, double a, double b) {
        if (x < a) return a;
        if (x > b) return b;
        return x;
    }

    /* approximate equality with relative tolerance */
    inline bool approx_equal(double a, double b, double reltol = 1e-9, double abstol = 1e-12) {
        double diff = std::fabs(a - b);
        if (diff <= abstol) return true;
        return diff <= reltol * std::fmax(std::fabs(a), std::fabs(b));
    }
} 

#endif 