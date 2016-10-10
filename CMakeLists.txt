cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
option(benchmark "Build all benchmarks." OFF)

set(PROJECT_NAME BriCA2)
project(${PROJECT_NAME})
set(BRICA2_VERSION_MAJOR 1)
set(BRICA2_VERSION_MINOR 0)
set(BRICA2_VERSION_PATCH 0)

# Enable C++11
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" STD_CXX11)
if(STD_CXX11)
  add_definitions("-std=c++11")
else()
  add_definitions("-std=c++0x")
endif()

# Include directories for BriCA2
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

# Header only external software
include_directories(external/asio/asio/include)
include_directories(external/interprocess/include)

add_subdirectory(external)
add_subdirectory(brica2)
add_subdirectory(tests)
