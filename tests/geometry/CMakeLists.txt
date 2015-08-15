
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests/core)
link_directories(${CMAKE_BINARY_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/tests/core)

# Each test here
add_executable(geometry-test geometry-test.cpp)
target_link_libraries(geometry-test libgeometry)

