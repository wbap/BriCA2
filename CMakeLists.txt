cmake_minimum_required(VERSION 2.8)

option(test "Build all tests." OFF)
option(benchmark "Build all benchmarks." OFF)

set(PROJECT_NAME BriCA2)
project(${PROJECT_NAME})
set(BRICA2_VERSION_MAJOR 1)
set(BRICA2_VERSION_MINOR 0)
set(BRICA2_VERSION_PATCH 0)

set(CMAKE_CXX_FLAGS "-Wall")

################################
# Dependencies
################################


# If you want your own include/ directory, set this, and then you can do
# include_directories(${COMMON_INCLUDES}) in other CMakeLists.txt files.
include_directories(.)
#include_directories(/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/numpy/core/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

#find_library(python Python)

################################
# Normal Libraries & Executables
################################
add_library(brica2
  brica2/core/utils.cpp
  brica2/core/port.cpp
  brica2/core/unit.cpp
  brica2/core/component.cpp
  brica2/core/module.cpp
  brica2/core/agent.cpp
  brica2/core/scheduler.cpp

  brica2/components/const.cpp
  brica2/components/pipe.cpp
  brica2/components/null.cpp

  brica2/schedulers/virtual_time_sync_scheduler.cpp
)

################################
# Benchmarking
################################

if (benchmark)
  add_subdirectory(lib/benchmark-master)

  include_directories(${benchmark_SOURCE_DIR}/include ${benchmark_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

  ##############
  # Benchmarks
  ##############

  add_executable(runBenchmarks tests/brica2/core/vector_bm.cpp)

  target_link_libraries(runBenchmarks benchmark)
  target_link_libraries(runBenchmarks brica2)  
endif()

################################
# Testing
################################
if (test)
  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(lib/gtest-1.7.0)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

  ##############
  # Unit Tests
  ##############
  add_executable(runUnitTests
    tests/brica2/core/vector.cpp
    tests/brica2/core/port.cpp
    tests/brica2/core/unit.cpp
    tests/brica2/core/component.cpp
    tests/brica2/core/module.cpp
    tests/brica2/core/agent.cpp

    tests/brica2/components/const.cpp
    tests/brica2/components/pipe.cpp
    tests/brica2/components/null.cpp

    tests/brica2/schedulers/virtual_time_sync_scheduler.cpp
  )

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  # Link to library
  target_link_libraries(runUnitTests brica2)

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  #add_test(NAME spira-test COMMAND runUnitTests)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  #add_test(that-other-test-I-made runUnitTests)
endif()

install(
  DIRECTORY "${PROJECT_SOURCE_DIR}/include/brica2"
  DESTINATION include
  FILES_MATCHING PATTERN "*.*hpp")
