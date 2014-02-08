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

add_executable(move_chara-test move_chara-test.cpp)
target_link_libraries(move_chara-test liblua libgameplay liblua libgameplay libevents libgraphics libgeometry libcore liblua5.2 libphysics Box2D ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FFMPEG_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY})

