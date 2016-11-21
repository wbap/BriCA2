if(BRICA2_TEST AND NOT TARGET gtest)
  add_subdirectory(googletest)
endif()

if(BRICA2_BENCH AND NOT TARGET benchmark)
  add_subdirectory(benchmark)
endif()

include_directories(asio/asio/include)
