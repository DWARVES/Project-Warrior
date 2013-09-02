
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libphysics)

# List file here
add_library(${lib}
    Engine.cpp              Engine.hpp
    Entity.cpp              Entity.hpp
    unit_conversions.cpp    unit_conversions.hpp
	)

