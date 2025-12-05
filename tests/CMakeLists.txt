cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

option(ENABLE_STRESS_TESTS "Compile long-running stress tests" OFF)

# Collect test sources (all small unit tests)
file(GLOB TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Optionally add the heavy stress test only if requested
if(ENABLE_STRESS_TESTS)
  list(APPEND TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test_stress_simulator.cpp")
endif()

# Collect project source files (all src/*.cpp), but exclude main.cpp to avoid duplicate main symbol.
file(GLOB_RECURSE PROJECT_SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(FILTER PROJECT_SRCS EXCLUDE REGEX ".*/main\\.cpp$")

# Create test executable target
add_executable(unit_tests ${TEST_SRCS} ${PROJECT_SRCS})

target_include_directories(unit_tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(unit_tests PRIVATE gtest_main)

find_package(Threads REQUIRED)
target_link_libraries(unit_tests PRIVATE Threads::Threads)

add_test(NAME UnitTests COMMAND unit_tests)

set_target_properties(unit_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)