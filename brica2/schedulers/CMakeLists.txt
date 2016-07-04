set(CPP_FILES virtual_time_sync_scheduler.cpp)
set(HPP_FILES virtual_time_sync_scheduler.hpp)

add_library(brica2-schedulers SHARED ${CPP_FILES} ${HPP_FILES})
target_link_libraries(brica2-schedulers brica2-core)
