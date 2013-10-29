
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/lua)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/lua)

# Each test here
add_executable(script-test script-test.cpp)
target_link_libraries(script-test libcore liblua liblua5.2)
add_executable(saveexposure-test saveexposure-test.cpp)
target_link_libraries(saveexposure-test libgameplay libcore liblua liblua5.2 ${Boost_REGEX_LIBRARY})


