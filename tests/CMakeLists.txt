# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.15)

# This CMakeLists is intended to be included via add_subdirectory(tests)
# from the top-level CMake. It will:
# - Fetch GoogleTest via FetchContent
# - Build a single test executable 'unit_tests' that links tests and
#   the project's src files (excluding main.cpp).
#
# It assumes the top-level include directory is ${CMAKE_SOURCE_DIR}/include.

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Collect test sources
file(GLOB TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Collect project source files (all src/*.cpp), but exclude main.cpp to avoid duplicate main symbol.
file(GLOB_RECURSE PROJECT_SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(FILTER PROJECT_SRCS EXCLUDE REGEX ".*/main\\.cpp$")

# Create test executable target
add_executable(unit_tests ${TEST_SRCS} ${PROJECT_SRCS})

# Include project headers
target_include_directories(unit_tests PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Link GoogleTest and pthread if needed
target_link_libraries(unit_tests PRIVATE gtest_main)

# On some systems pthread is required
find_package(Threads REQUIRED)
target_link_libraries(unit_tests PRIVATE Threads::Threads)

# Add test to CTest
add_test(NAME UnitTests COMMAND unit_tests)

# Optionally set properties for nicer output
set_target_properties(unit_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)