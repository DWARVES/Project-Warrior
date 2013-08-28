
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/core)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/core)

# Each test here
add_executable(fakefs.prog fakefs-test.cpp)
target_link_libraries(fakefs.prog libcore)
add_executable(pathParser-test pathParser-test.cpp)
target_link_libraries(pathParser-test libcore)


