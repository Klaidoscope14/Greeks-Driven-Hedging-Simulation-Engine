#ifndef INVENTORYMANAGER_HPP
#define INVENTORYMANAGER_HPP

#include <mutex>
using namespace std;

/*
 * InventoryManager: thread-safe holder for option and underlying positions.
 * Keeps counts and simple helpers. Using a mutex keeps this safe if we later
 * parallelize components.
 */

namespace mm {
    class InventoryManager {
    public:
        InventoryManager() = default;

        void add_underlying(double qty);
        void add_options(int qty);

        double underlying() const;
        int options() const;

        void set_underlying(double qty);
        void set_options(int qty);

    private:
        mutable std::mutex mtx_;
        double underlying_pos_ = 0.0;
        int option_pos_ = 0;
    };
} 

#endif 