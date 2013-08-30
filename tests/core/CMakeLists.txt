
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/core)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/core)

# Each test here
add_executable(fakefs-test fakefs-test.cpp)
target_link_libraries(fakefs-test libcore)
add_executable(pathParser-test pathParser-test.cpp)
target_link_libraries(pathParser-test libcore)
add_executable(systemTime-test systemTime-test.cpp)
target_link_libraries(systemTime-test libcore)


