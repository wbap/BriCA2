set(CPP_FILES const.cpp pipe.cpp null.cpp pycomponent.cpp)
set(HPP_FILES const.hpp pipe.hpp null.hpp pycomponent.hpp)

find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(PythonLibs 2.7 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_library(brica2-components SHARED ${CPP_FILES} ${HPP_FILES})
target_link_libraries(brica2-components brica2-core ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
