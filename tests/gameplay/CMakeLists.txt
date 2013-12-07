
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/gameplay)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/gameplay)

# Each test here
add_executable(save-test save-test.cpp)
target_link_libraries(save-test libgameplay libcore ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})


