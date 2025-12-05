#ifndef PRICEPATH_HPP
#define PRICEPATH_HPP

#include <vector>
using namespace std;

/*
 * PricePath: a small POD-like container for a generated underlying price path.
 * We keep it minimal: stores the vector of spot prices and a convenience accessor.
 */

namespace market {

class PricePath {
public:
    PricePath() = default;
    explicit PricePath(const std::vector<double>& p) : path_(p) {}

    const std::vector<double>& path() const { 
        return path_; 
    }

    std::vector<double>& path() { 
        return path_; 
    }

    size_t size() const { 
        return path_.size(); 
    }

    bool empty() const { 
        return path_.empty();
    }

    double at(size_t idx) const { 
        return path_.at(idx); 
    }

private:
    std::vector<double> path_;
};

} 

#endif 