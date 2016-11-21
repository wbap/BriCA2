cmake_minimum_required(VERSION 2.8)

option(BRICA2_TEST "Build all tests." ON)
option(BRICA2_BENCH "Build all benchmarks." ON)

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

# Enable including/linking from CMAKE_PREFIX_PATH
if(DEFINED CMAKE_PREFIX_PATH)
  include_directories(${CMAKE_PREFIX_PATH}/include)
  link_directories(${CMAKE_PREFIX_PATH}/lib)
endif()

# Header only external software
include_directories(external/asio/asio/include)

# Include directories for BriCA2
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(tests)
