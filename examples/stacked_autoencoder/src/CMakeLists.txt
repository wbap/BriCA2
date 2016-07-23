add_executable(stacked_autoencoder main.cpp)
target_link_libraries(stacked_autoencoder ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} brica2-core brica2-schedulers brica2-components)
