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

