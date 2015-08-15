
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests/gameplay)
link_directories(${CMAKE_BINARY_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/tests/gameplay)

# Each test here
add_executable(save-test save-test.cpp)
target_link_libraries(save-test libgameplay libcore ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
add_executable(character_preview-test character_preview-test.cpp)
target_link_libraries(character_preview-test liblua libgameplay liblua libgameplay libphysics libevents libgraphics libgeometry libcore liblua5.2 Box2D ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FFMPEG_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY})
add_executable(character-test character-test.cpp)
target_link_libraries(character-test liblua libgameplay liblua libgameplay libphysics libevents libgraphics libgeometry libcore liblua5.2 Box2D ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FFMPEG_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY})
add_executable(controler-test controler-test.cpp)
target_link_libraries(controler-test liblua libgameplay liblua libgameplay libphysics libevents libgraphics libgeometry libcore liblua5.2 Box2D ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FFMPEG_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY})


