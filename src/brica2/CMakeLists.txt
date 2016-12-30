add_subdirectory(core)
add_subdirectory(scheduler)

set(CORE_CPP_FILES core/port.cpp core/unit.cpp core/component.cpp
                   core/scheduler.cpp core/thread_pool.cpp)

set(SCHEDULER_CPP_FILES scheduler/virtual_time_sync_scheduler.cpp)

add_library(brica2 SHARED ${CORE_CPP_FILES} ${SCHEDULER_CPP_FILES})
