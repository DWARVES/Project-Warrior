include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/physics/Entity-test)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/physics/Entity-test)

include_directories (
	${OPENGL_INCLUDE_DIR}
	${Box2D_SOURCE_DIR}
)

# Some flags for Freeglut and GLUI.
add_definitions( -DFREEGLUT_EXPORTS -DFREEGLUT_STATIC -D_CRT_SECURE_NO_WARNINGS )
# Define the framework files.
add_executable(Entity-test
	testbed/Main.cpp
	testbed/Render.cpp
	testbed/Render.h
	testbed/Test.cpp
	testbed/Test.h
	testbed/TestEntries.cpp
)

target_link_libraries (
	Entity-test
    libphysics
    libgeometry
    libcore
	Box2D
	glut
	glui
	GL
	GLU
	${ADDITIONAL_LIBRARIES}
	${OPENGL_LIBRARIES}
) 
