
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests/lua)
link_directories(${CMAKE_BINARY_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/tests/lua)

# Each test here
add_executable(script-test script-test.cpp)
target_link_libraries(script-test libcore liblua liblua5.2)
add_executable(saveexposure-test saveexposure-test.cpp)
target_link_libraries(saveexposure-test libgameplay libcore liblua liblua5.2 ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
add_executable(graphicsexposure-test graphicsexposure-test.cpp)
target_link_libraries(graphicsexposure-test libevents libgraphics libgeometry libcore liblua liblua5.2 ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FFMPEG_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY})
add_executable(pathexposure-test pathexposure-test.cpp)
target_link_libraries(pathexposure-test libcore liblua libcore liblua5.2 ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})


