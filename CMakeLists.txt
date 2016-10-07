cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
option(benchmark "Build all benchmarks." OFF)

set(PROJECT_NAME BriCA2)
project(${PROJECT_NAME})
set(BRICA2_VERSION_MAJOR 1)
set(BRICA2_VERSION_MINOR 0)
set(BRICA2_VERSION_PATCH 0)

add_definitions("-std=c++11")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(PythonLibs 2.7 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(external/asio/asio/include)

add_subdirectory(external)
add_subdirectory(brica2)
add_subdirectory(tests)
add_subdirectory(examples)
