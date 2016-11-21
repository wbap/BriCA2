set(CPP_FILES virtual_time_sync_scheduler.cpp)

add_library(brica2-scheduler SHARED ${CPP_FILES})
target_link_libraries(brica2-scheduler brica2-core)
