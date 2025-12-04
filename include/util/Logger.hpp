#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

/*
 * Minimal logger helper for debugging and event tracing.
 * Not intended to replace spdlog; 
 * keeps dependency free for starter code.
*/

namespace util {

inline void log_info(const std::string& msg) {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto itt = system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_MSC_VER)
    localtime_s(&tm, &itt);
#else
    localtime_r(&itt, &tm);
#endif
    std::cout << "[" << std::put_time(&tm, "%F %T") << "] [INFO] " << msg << std::endl;
}

inline void log_error(const std::string& msg) {
    std::cerr << "[ERROR] " << msg << std::endl;
}

} 

#endif 