cmake_minimum_required(VERSION 3.15)
project(options_market_maker_sim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(options-mm
    src/main.cpp
    src/core/Types.cpp
    src/core/Time.cpp
    src/util/CSVWriter.cpp
    src/util/Logger.cpp
    src/util/Random.cpp
    src/pricing/OptionSpecification.cpp
    src/pricing/BlackScholes.cpp
    src/pricing/BinomialTree.cpp
    src/greeks/GreeksCalculator.cpp
    src/market/UnderlyingSimulator.cpp
    src/execution/Order.cpp
    src/execution/ExecutionEngine.cpp
    src/hedging/DeltaHedger.cpp
    src/mm/MarketMaker.cpp
    src/sim/Simulator.cpp
)

# ------------------------------
# Stress-run tool (tools/stress_run.cpp)
# ------------------------------
file(GLOB_RECURSE PROJECT_SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")
# Exclude main.cpp if present (so we don't get duplicate main)
list(FILTER PROJECT_SRCS EXCLUDE REGEX ".*/main\\.cpp$")

# Create stress_run executable by compiling project sources plus the tool
add_executable(stress_run tools/stress_run.cpp ${PROJECT_SRCS})
target_include_directories(stress_run PRIVATE "${CMAKE_SOURCE_DIR}/include")
set_target_properties(stress_run PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

# Optionally, limit optimization or enable LTO for this target if desired
# target_compile_options(stress_run PRIVATE -O3)

# Optionally add tests later
add_subdirectory(tests)