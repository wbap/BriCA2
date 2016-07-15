set(CPP_FILES const.cpp pipe.cpp null.cpp wait.cpp pycomponent.cpp)
set(HPP_FILES const.hpp pipe.hpp null.hpp wait.hpp pycomponent.hpp)

add_library(brica2-components SHARED ${CPP_FILES} ${HPP_FILES})
target_link_libraries(brica2-components brica2-core ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
