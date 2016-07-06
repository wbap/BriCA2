cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
option(benchmark "Build all benchmarks." OFF)

set(PROJECT_NAME BriCA2)
project(${PROJECT_NAME})
set(BRICA2_VERSION_MAJOR 1)
set(BRICA2_VERSION_MINOR 0)
set(BRICA2_VERSION_PATCH 0)

add_definitions("-std=c++14")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(brica2)
add_subdirectory(tests)

