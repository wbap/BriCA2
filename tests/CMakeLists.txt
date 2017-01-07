if(BRICA2_TEST)
  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include)

  add_executable(core_tests
    brica2/core/cargo.cpp
    brica2/core/port.cpp
    brica2/core/unit.cpp
    brica2/core/component.cpp
    brica2/core/module.cpp
  )

  target_link_libraries(core_tests gtest gtest_main brica2-core)
  add_test(NAME core COMMAND core_tests)
endif()

if(BRICA2_BENCH)
  include_directories(${benchmark_SOURCE_DIR}/include)

  add_executable(cargo_bm cargo_bm.cpp)
  target_link_libraries(cargo_bm benchmark brica2-core)

  add_executable(thread_bm thread_bm.cpp)
  target_link_libraries(thread_bm benchmark brica2-core brica2-scheduler)
endif()
