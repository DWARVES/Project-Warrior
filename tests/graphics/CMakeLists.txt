
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/graphics)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/graphics)

# Each test here
add_executable(graphics-test graphics-test.cpp)
target_link_libraries(graphics-test libgraphics libcore libgeometry ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
add_executable(color-test color-test.cpp)
target_link_libraries(color-test libgraphics ${SDL2_LIBRARIES})


