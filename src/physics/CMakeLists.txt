
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libphysics)

# List file here
add_library(${lib}
    World.cpp               World.hpp
    CollisionManager.cpp    CollisionManager.hpp
    Entity.cpp              Entity.hpp
    Character.cpp           Character.hpp
    Attack.cpp              Attack.hpp
    Platform.cpp            Platform.hpp
    Obstacle.cpp            Obstacle.hpp
    unit_conversions.cpp    unit_conversions.hpp
	)

