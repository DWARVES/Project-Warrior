include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/physics)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/physics)

add_executable(physics-test
	testbed/Main.cpp
	testbed/Render.cpp
	testbed/Render.h
	testbed/Test.cpp
	testbed/Test.h
	testbed/TestEntries.cpp
)

target_link_libraries (
    physics-test
    libphysics
    libgeometry
    libcore
	Box2D
	glut
    glui
    ${OPENGL_LIBRARIES}
) 
