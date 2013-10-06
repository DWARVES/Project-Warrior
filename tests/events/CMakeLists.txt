
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/core)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/core)

# Each test here
add_executable(events-test events-test.cpp)
target_link_libraries(events-test libevents libgraphics libcore libgeometry ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FFMPEG_LIBRARIES} ${GLEW_LIBRARIES})

