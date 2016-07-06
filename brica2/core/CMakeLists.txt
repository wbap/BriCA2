set(CPP_FILES agent.cpp port.cpp scheduler.cpp unit.cpp utils.cpp)
set(HPP_FILES agent.hpp component.hpp module.hpp port.hpp scheduler.hpp
              unit.hpp utils.hpp vector.hpp vector_base.hpp)

add_definitions("-std=c++14")
add_library(brica2-core SHARED ${CPP_FILES} ${HPP_FILES})


