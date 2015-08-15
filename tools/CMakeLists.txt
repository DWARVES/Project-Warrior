
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/liblua/lua-5.2.3/src)
link_directories(${CMAKE_BINARY_DIR}/src)

# Each tool here
add_executable(makefont makefont.cpp)
target_link_libraries(makefont ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})


