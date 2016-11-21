set(CPP_FILES port.cpp unit.cpp component.cpp scheduler.cpp thread_pool.cpp)

add_library(brica2-core SHARED ${CPP_FILES})

if(UNIX AND NOT APPLE)
  target_link_libraries(brica2-core rt)
endif()
