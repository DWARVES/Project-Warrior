
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
set(lib libphysics)

# List file here
add_library(${lib}
    World.cpp               World.hpp
    Entity.cpp              Entity.hpp
    Character.cpp           Character.hpp
    Attack.cpp              Attack.hpp
    Platform.cpp            Platform.hpp
    Obstacle.cpp            Obstacle.hpp
    unit_conversions.cpp    unit_conversions.hpp
    debugDraw.cpp           debugDraw.hpp
	)

