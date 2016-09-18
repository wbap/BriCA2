if (test)
  file(COPY python DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

  add_subdirectory(../lib/gtest-1.7.0 "${CMAKE_CURRENT_BINARY_DIR}/lib/gtest-1.7.0")

  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

  add_executable(runUnitTests
    brica2/core/vector.cpp
    brica2/core/port.cpp
    brica2/core/unit.cpp

    brica2/core/component.cpp
    brica2/core/module.cpp
    brica2/core/agent.cpp

    brica2/core/thread_pool.cpp

    brica2/components/const.cpp
    brica2/components/pipe.cpp
    brica2/components/null.cpp
    brica2/components/pycomponent.cpp

    brica2/schedulers/virtual_time_sync_scheduler.cpp
  )

  target_link_libraries(runUnitTests gtest gtest_main
                                     brica2-core brica2-schedulers brica2-components)
  add_test(NAME brica2-test COMMAND runUnitTests)
endif()

if (benchmark)
  add_subdirectory(../lib/benchmark-master "${CMAKE_CURRENT_BINARY_DIR}/lib/benchmark-master")

  include_directories(${benchmark_SOURCE_DIR}/include ${benchmark_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

  ##############
  # Benchmarks
  ##############

  add_executable(vector_bm brica2/core/vector_bm.cpp)
  add_executable(main_bm benchmark.cpp)
  add_executable(thread_bm thread.cpp)

  target_link_libraries(vector_bm benchmark brica2-core brica2-schedulers brica2-components)
  target_link_libraries(main_bm benchmark brica2-core brica2-schedulers brica2-components)
  target_link_libraries(thread_bm benchmark brica2-core brica2-schedulers brica2-components)
endif()
