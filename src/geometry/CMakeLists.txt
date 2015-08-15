
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
set(lib libgeometry)

# List file here
add_library(${lib}
    aabb.cpp    aabb.hpp
    circle.cpp  circle.hpp
    line.cpp    line.hpp
    point.cpp   point.hpp
    polygon.cpp polygon.hpp
	)

