
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)

# Each tool here
add_executable(makefont makefont.cpp)
target_link_libraries(makefont ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})


