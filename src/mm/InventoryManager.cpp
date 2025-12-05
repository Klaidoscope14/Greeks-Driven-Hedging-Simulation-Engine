#include "mm/InventoryManager.hpp"
using namespace std;

namespace mm {
    void InventoryManager::add_underlying(double qty) {
        lock_guard<mutex> lk(mtx_);
        underlying_pos_ += qty;
    }

    void InventoryManager::add_options(int qty) {
        lock_guard<mutex> lk(mtx_);
        option_pos_ += qty;
    }

    double InventoryManager::underlying() const {
        lock_guard<mutex> lk(mtx_);
        return underlying_pos_;
    }

    int InventoryManager::options() const {
        lock_guard<mutex> lk(mtx_);
        return option_pos_;
    }

    void InventoryManager::set_underlying(double qty) {
        lock_guard<mutex> lk(mtx_);
        underlying_pos_ = qty;
    }

    void InventoryManager::set_options(int qty) {
        lock_guard<mutex> lk(mtx_);
        option_pos_ = qty;
    }
} 