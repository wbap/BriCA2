if (test)
  find_package(Boost COMPONENTS python REQUIRED)
  include_directories(${Boost_INCLUDE_DIRS})

  find_package(PythonLibs 2.7 REQUIRED)
  include_directories(${PYTHON_INCLUDE_DIRS})

  file(COPY python DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

  add_subdirectory(../lib/gtest-1.7.0 "${CMAKE_CURRENT_BINARY_DIR}/lib")

  enable_testing()

  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

  add_executable(runUnitTests
    brica2/core/vector.cpp
    brica2/core/port.cpp
    brica2/core/unit.cpp

    brica2/core/component.cpp
    brica2/core/module.cpp
    brica2/core/agent.cpp

    brica2/components/const.cpp
    brica2/components/pipe.cpp
    brica2/components/null.cpp
    brica2/components/pycomponent.cpp

    brica2/schedulers/virtual_time_sync_scheduler.cpp
  )

  target_link_libraries(runUnitTests gtest gtest_main
                                     brica2-core brica2-schedulers brica2-components
                                     ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
  add_test(NAME brica2-test COMMAND runUnitTests)
endif()
